#include "intelligentwiper/aa/logic.h"
#include <chrono>

namespace intelligentwiper
{
    namespace aa
    {

        Logic::Logic(intelligentwiper::aa::port::RPort_Wiper *wiperPort,
                     intelligentwiper::aa::port::RPort_VehicleInfo *vehicleInfoPort)
            : m_logger(ara::log::CreateLogger("LOGC", "LOGC", ara::log::LogLevel::kVerbose)),
              m_wiperPort(wiperPort),
              m_vehicleInfoPort(vehicleInfoPort),
              m_lastVelocity(0.0),
              m_refWiperSpeed(1.0),
              m_refWiperInterval(1.0)
        {
            m_wiperPort->RegistFieldHandlersoaWiperStatus([this](const eevp::control::SoaWiperStatus &status)
                                                { m_logger.LogInfo() << "Logic::WiperStatus Changed: " << static_cast<int>(status.mode); });
            m_vehicleInfoPort->RegistFieldHandlersoaVehicleInfo([this](const eevp::control::VehicleInfo &vInfo)
                                                    { m_logger.LogInfo() << "Logic::VehicleInfo Changed: Speed=" << vInfo.speed << ", GearState=" << static_cast<int>(vInfo.gearState); });
        }

        double Logic::getCurrentVehicleSpeed()
        {
            if (!m_vehicleInfoPort)
                return 0.0;

            std::promise<double> promise;
            auto future = promise.get_future();

            // handler 등록 → vehicle info가 오면 promise에 set
            m_vehicleInfoPort->RegistFieldHandlersoaVehicleInfo([&](const eevp::control::VehicleInfo &vInfo)
                                                                { promise.set_value(static_cast<double>(vInfo.speed)); });

            m_vehicleInfoPort->GetsoaVehicleInfo();

            // 최대 100ms 대기
            if (future.wait_for(std::chrono::milliseconds(100)) == std::future_status::ready)
                return future.get();

            return 0.0; // 값 못받으면 0
        }

        int Logic::getCurrentGearState()
        {
            if (!m_vehicleInfoPort)
                return 0;

            std::promise<int> promise;
            auto future = promise.get_future();

            m_vehicleInfoPort->RegistFieldHandlersoaVehicleInfo([&](const eevp::control::VehicleInfo &vInfo)
                                                                { promise.set_value(static_cast<int>(vInfo.gearState)); });

            m_vehicleInfoPort->GetsoaVehicleInfo();

            if (future.wait_for(std::chrono::milliseconds(100)) == std::future_status::ready)
                return future.get();

            return 0; // 값 못받으면 기본 0
        }

        bool Logic::getBrakePedalState()
        {
            // VehicleInfo에 없음 → 기본 false
            return false;
        }

        bool Logic::isSpeedZeroForThreeSeconds()
        {
            const double duration = 3.0;
            const double checkInterval = 0.1;
            double timeZero = 0.0;

            while (timeZero < duration)
            {
                double velocity = getCurrentVehicleSpeed();
                if (velocity == 0.0)
                {
                    timeZero += checkInterval;
                }
                else
                {
                    timeZero = 0.0;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(checkInterval * 1000)));
            }
            return true;
        }

        void Logic::executeParkingCase()
        {
            int gear = getCurrentGearState();
            if (gear == static_cast<int>(eevp::control::SoaGearState::kGEAR_P))
            {
                m_wiperPort->RequestRequestWiperOperation(eevp::control::SoaWiperMode::kOFF);
            }
            else
            {
                m_wiperPort->RequestRequestWiperOperation(eevp::control::SoaWiperMode::kON);
            }
        }

        void Logic::executeDefaultCase()
        {
            bool stopStatus = isSpeedZeroForThreeSeconds();
            bool drivingIntention = (getCurrentGearState() == static_cast<int>(eevp::control::SoaGearState::kGEAR_D)) &&
                                    !getBrakePedalState();

            if ((stopStatus && !drivingIntention))
            {
                m_wiperPort->RequestRequestWiperOperation(eevp::control::SoaWiperMode::kOFF);
            }
            else
            {
                m_wiperPort->RequestRequestWiperOperation(eevp::control::SoaWiperMode::kON);
            }
        }

        void Logic::setWiperValue(double vehVelocityValue, double refVehVelocityValue, double refWiperSpeed, double refWiperInterval)
        {
            const int divider = 5;
            double diff = vehVelocityValue - refVehVelocityValue;
            double diffDivided = diff / divider;
            int output1 = (diffDivided < 0) ? -1 : ((diffDivided == 0) ? 0 : 1);
            double output2 = std::floor(std::fabs(diffDivided)) * 0.1;
            double output3 = output1 * output2 + 1.0;

            // 속도와 인터벌 기반으로 와이퍼 모드 결정
            eevp::control::SoaWiperMode mode;
            if (output3 <= 1.0)
                mode = eevp::control::SoaWiperMode::kOFF;
            else if (output3 <= 1.5)
                mode = eevp::control::SoaWiperMode::kINT1;
            else if (output3 <= 2.0)
                mode = eevp::control::SoaWiperMode::kINT2;
            else if (output3 <= 2.5)
                mode = eevp::control::SoaWiperMode::kINT3;
            else
                mode = eevp::control::SoaWiperMode::kON;

            m_wiperPort->RequestRequestWiperOperation(mode);
        }

        void Logic::wiperControlRefGen(double vehVelocityValue)
        {
            // 차량 속도에 따라 와이퍼 모드 결정
            eevp::control::SoaWiperMode mode;
            if (vehVelocityValue <= 0.5)
                mode = eevp::control::SoaWiperMode::kOFF;
            else if (vehVelocityValue <= 5.0)
                mode = eevp::control::SoaWiperMode::kINT1;
            else if (vehVelocityValue <= 15.0)
                mode = eevp::control::SoaWiperMode::kINT2;
            else if (vehVelocityValue <= 25.0)
                mode = eevp::control::SoaWiperMode::kINT3;
            else
                mode = eevp::control::SoaWiperMode::kON;

            m_wiperPort->RequestRequestWiperOperation(mode);
        }

        void Logic::Execute()
        {

            double velocity = getCurrentVehicleSpeed();
            int gear = getCurrentGearState();

            if (gear == static_cast<int>(eevp::control::SoaGearState::kGEAR_P))
            {
                m_logger.LogInfo() << "[Logic] In Parking Case, Request wiper OFF";
                m_wiperPort->RequestRequestWiperOperation(eevp::control::SoaWiperMode::kOFF);
            }
            else
            {
                bool stopStatus = isSpeedZeroForThreeSeconds();
                bool drivingIntention = (gear == static_cast<int>(eevp::control::SoaGearState::kGEAR_D)) &&
                                        !getBrakePedalState();

                if (stopStatus && !drivingIntention)
                {
                    m_logger.LogInfo() << "[Logic] In Stop Case, Request wiper OFF";
                    m_wiperPort->RequestRequestWiperOperation(eevp::control::SoaWiperMode::kOFF);
                }
                else
                    wiperControlRefGen(velocity); // 속도 기반 모드 설정
            }
        }

    } // namespace aa
} // namespace intelligentwiper
