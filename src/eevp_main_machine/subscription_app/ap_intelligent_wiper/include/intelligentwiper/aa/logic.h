#pragma once
#include "intelligentwiper/aa/port/rport_wiper.h"
#include "intelligentwiper/aa/port/rport_vehicleinfo.h"

#include <cmath>
#include <thread>
#include <memory>

namespace intelligentwiper
{
    namespace aa
    {

        class Logic
        {
        public:
            Logic(intelligentwiper::aa::port::RPort_Wiper *wiperPort,
                  intelligentwiper::aa::port::RPort_VehicleInfo *vehicleInfoPort);

            // 차량 정보 읽기
            double getCurrentVehicleSpeed();
            int getCurrentGearState();
            bool getBrakePedalState();

            // 와이퍼 제어
            void executeParkingCase();
            void executeDefaultCase();
            void setWiperValue(double vehVelocityValue, double refVehVelocityValue,
                               double refWiperSpeed, double refWiperInterval);
            void wiperControlRefGen(double vehVelocityValue);

            // 속도 0 여부 확인 (3초)
            bool isSpeedZeroForThreeSeconds();

            // 전체 실행 루프(한 사이클)
            void Execute();

        private:
            intelligentwiper::aa::port::RPort_Wiper *m_wiperPort;
            intelligentwiper::aa::port::RPort_VehicleInfo *m_vehicleInfoPort;

            // 이전 속도 및 기준 와이퍼 값
            double m_lastVelocity;
            double m_refWiperSpeed;
            double m_refWiperInterval;
        };

    } // namespace aa
} // namespace intelligentwiper
