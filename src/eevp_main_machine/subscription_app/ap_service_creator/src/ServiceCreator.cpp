#include "ara/exec/execution_client.h"
#include "ServiceCreator.h"
#include "ara/log/logger.h"

namespace eevp
{
    namespace simulation
    {
        pthread_t ServiceCreator::thread_socket_recv;
        pthread_t ServiceCreator::thread_socket_send;
        pthread_t ServiceCreator::thread_socket_km;
        std::string ServiceCreator::wiperLevel[] = {"LOW", "MEDIUM", "HIGH", "STOP"};

        std::atomic_bool ServiceCreator::mRunning(false);

        // WiperListener
        class WiperListener : public eevp::simulation::IWiperListener
        {
        public:
            WiperListener(ServiceCreator *svc) : serviceCreator(svc) {}

            void stopWiping() { return serviceCreator->stopWiping(); }
            void startWiping() { return serviceCreator->startWiping(); }
            void setWipingInterval(const std::uint16_t &wipingInterval) { return serviceCreator->setWipingInterval(wipingInterval); }
            void setWipingLevel(const eevp::simulation::BCM_WipingLevel &wipingLevel) { return serviceCreator->setWipingLevel(wipingLevel); }

        private:
            ServiceCreator *serviceCreator;
        };

        // TMoodLampListener
        class TMoodLampListener : public eevp::simulation::ITMoodLampListener
        {
        public:
            TMoodLampListener(ServiceCreator *svc) : serviceCreator(svc) {}

            void RequestMImSetBrightness(const std::uint16_t &Brightness) { return serviceCreator->RequestMImSetBrightness(Brightness); }
            void RequestMImSetMode(const lmp::mode::SoaMImMoodeMode &mood) { return serviceCreator->RequestMImSetMode(mood); }
            void RequestMImSetRGBColor(const std::uint8_t &ColorIndex) { return serviceCreator->RequestMImSetRGBColor(ColorIndex); }

        private:
            ServiceCreator *serviceCreator;
        };

        // MsgInfoListener
        class MsgInfoListener : public eevp::simulation::IMsgInfoListener
        {
        public:
            MsgInfoListener(ServiceCreator *svc) : serviceCreator(svc) {}

            void notifyMsgInfo(const bms::output::OutputData &output) { return serviceCreator->notifyMsgInfo(output); }

        private:
            ServiceCreator *serviceCreator;
        };

        // AccrPedalListener
        class AccrPedalListener : public eevp::simulation::IAccrPedalListener
        {
        public:
            AccrPedalListener(ServiceCreator *svc) : serviceCreator(svc) {}

            void notifyAccrPedal(eevp::simulation::type::VCS_AccrPedal &accrPedal) { return serviceCreator->notifyAccrPedal(accrPedal); }

        private:
            ServiceCreator *serviceCreator;
        };

        // EnvMonitorListener
        class EnvMonitorListener : public eevp::simulation::IEnvMonitorListener
        {
        public:
            EnvMonitorListener(ServiceCreator *svc) : serviceCreator(svc) {}

            void notifyHumidity(eevp::simulation::type::TMS_EnvZoneHumidityAry &envZoneHumidityAry) { return serviceCreator->notifyHumidity(envZoneHumidityAry); }

        private:
            ServiceCreator *serviceCreator;
        };

        // GearListener
        class GearListener : public eevp::simulation::IGearListener
        {
        public:
            GearListener(ServiceCreator *svc) : serviceCreator(svc) {}

            void notifyGear(eevp::simulation::type::VCS_Gear &gear) { return serviceCreator->notifyGear(gear); }

        private:
            ServiceCreator *serviceCreator;
        };

        // BrakePedalListener
        class BrakePedalListener : public eevp::simulation::IBrakePedalListener
        {
        public:
            BrakePedalListener(ServiceCreator *svc) : serviceCreator(svc) {}

            void notifyPosn(eevp::simulation::type::VCS_BrakePosn &brakePosn) { return serviceCreator->notifyPosn(brakePosn); }
            void notifyBrakeSwitch(eevp::simulation::type::VCS_BrakeSwitch &brakeSwitch) { return serviceCreator->notifyBrakeSwitch(brakeSwitch); }

        private:
            ServiceCreator *serviceCreator;
        };

        // VehSpdListener
        class VehSpdListener : public eevp::simulation::IVehSpdListener
        {
        public:
            VehSpdListener(ServiceCreator *svc) : serviceCreator(svc) {}

            void notifyVehSpd(eevp::simulation::type::VCS_VehSpd &vehSpd) { return serviceCreator->notifyVehSpd(vehSpd); }

        private:
            ServiceCreator *serviceCreator;
        };

        // SnsrUssListener
        class SnsrUssListener : public eevp::simulation::ISnsrUssListener
        {
        public:
            SnsrUssListener(ServiceCreator *svc) : serviceCreator(svc) {}

            void ntfSonarInfo(eevp::simulation::type::USSSonarInfo &ussSonarInfo) { return serviceCreator->ntfSonarInfo(ussSonarInfo); }
            bool isDetect() { return serviceCreator->isDetect(); }

        private:
            ServiceCreator *serviceCreator;
        };

        // LotteListener
        class LotteListener : public eevp::simulation::ILotteListener
        {
        public:
            LotteListener(ServiceCreator *svc) : serviceCreator(svc) {}

            void NotifyDmsCurr(lotte::type::DmsGzDtctn &dmsGzDtctn) { return serviceCreator->NotifyDmsCurr(dmsGzDtctn); }
            void NotifyDmsStatus(bool &result) { return serviceCreator->NotifyDmsStatus(result); }
            void NotifySmartFilmCurr(lotte::type::SideTransparency &result) { return serviceCreator->NotifySmartFilmCurr(result); }
            void NotifySmartFilmStatus(bool &result) { return serviceCreator->NotifySmartFilmStatus(result); }
            void SmartFilmControl(const std::int32_t &windowLoc, const std::int32_t &transparence) { return serviceCreator->SmartFilmControl(windowLoc, transparence); }

        private:
            ServiceCreator *serviceCreator;
        };

        // SESLServiceListener
        class SESLServiceListener : public eevp::simulation::ISESLServiceListener
        {
        public:
            SESLServiceListener(ServiceCreator *svc) : serviceCreator(svc) {}

            void SESL_Receive(ara::SESL::Vehicle_Data &Receive_Argument) { return serviceCreator->SESL_Receive(Receive_Argument); }
            void SESL_Send(const ara::SESL::Vehicle_Data &Send_Argument) { return serviceCreator->SESL_Send(Send_Argument); }

        private:
            ServiceCreator *serviceCreator;
        };

        ServiceCreator::ServiceCreator() : mLogger(ara::log::CreateLogger("SVCC", "SVCC", ara::log::LogLevel::kInfo))
        {
            mLogger.LogInfo() << __func__;
            std::signal(SIGTERM, SignalHandler);
        }

        ServiceCreator::~ServiceCreator()
        {
        }

        void

        ServiceCreator::SignalHandler(std::int32_t /*signal*/)
        {
            mRunning = false;
        }

        bool
        ServiceCreator::Start()
        {
            mLogger.LogInfo() << __func__;
            mRunning = true;

            if (!setRunningState())
            {
                return false;
            }

            if (!initialize())
            {
                return false;
            }

            if (!startServiceCreatorStub())
            {
                return false;
            }

            if (!startMsgInfoProxy())
            {
                return false;
            }

            if (!startSocketClient())
            {

                return false;
            }

            return true;
        }

        void
        ServiceCreator::Run()
        {
            mLogger.LogInfo() << __func__;
            while (mRunning)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1000));
                mLogger.LogInfo() << "ServiceCreator is running";
            }
        }

        void
        ServiceCreator::Terminate()
        {
            mLogger.LogInfo() << __func__;
            mRunning = false;
        }

        // Wiper Start

        void ServiceCreator::stopWiping()
        {
            eevp::simulation::BCM_WipingLevel tempWipingLevel = eevp::simulation::BCM_WipingLevel::STOP;
            wipingLevelSend = tempWipingLevel;
            wipingIntervalSend = 0U;
            requestwipinglevel = true;
            requestwipinginterval = true;
            wiperSkeletonImpl->updateWipingLevel(wipingLevelSend);
            wiperSkeletonImpl->updateWipingInterval(wipingIntervalSend);
            return;
        }

        void ServiceCreator::startWiping()
        {
            eevp::simulation::BCM_WipingLevel tempWipingLevel = eevp::simulation::BCM_WipingLevel::LOW;
            wipingLevelSend = tempWipingLevel;
            requestwipinglevel = true;
            wiperSkeletonImpl->updateWipingLevel(wipingLevelSend);
            return;
        }

        void ServiceCreator::setWipingLevel(const eevp::simulation::BCM_WipingLevel &wipingLevel)
        {
            mLogger.LogInfo() << __func__;
            wipingLevelSend = wipingLevel;
            requestwipinglevel = true;
            wiperSkeletonImpl->updateWipingLevel(wipingLevelSend);
            return;
        }

        void
        ServiceCreator::setWipingInterval(const std::uint16_t &wipingInterval)
        {
            mLogger.LogInfo() << __func__;
            wipingIntervalSend = wipingInterval;
            requestwipinglevel = true;
            wiperSkeletonImpl->updateWipingInterval(wipingIntervalSend);
            return;
        }

        void
        ServiceCreator::getWiperRecv()
        {
            switch (wipingLevelReceive)
            {
            case BCM_WipingLevel::HIGH:
                mLogger.LogInfo() << "wiperRecv: [HIGH, " << wipingIntervalReceive << "]";
                break;
            case BCM_WipingLevel::LOW:
                mLogger.LogInfo() << "wiperRecv: [LOW, " << wipingIntervalReceive << "]";
                break;
            case BCM_WipingLevel::MEDIUM:
                mLogger.LogInfo() << "wiperRecv: [MEDIUM, " << wipingIntervalReceive << "]";
                break;
            case BCM_WipingLevel::STOP:
                mLogger.LogInfo() << "wiperRecv: [STOP, " << wipingIntervalReceive << "]";
                break;
            default:
                break;
            }
        }
        void
        ServiceCreator::getWiperSend()
        {
            switch (wipingLevelSend)
            {
            case BCM_WipingLevel::HIGH:
                mLogger.LogInfo() << "wiperSend: [HIGH, " << wipingIntervalSend << "]";
                break;
            case BCM_WipingLevel::LOW:
                mLogger.LogInfo() << "wiperSend: [LOW, " << wipingIntervalSend << "]";
                break;
            case BCM_WipingLevel::MEDIUM:
                mLogger.LogInfo() << "wiperSend: [MEDIUM, " << wipingIntervalSend << "]";
                break;
            case BCM_WipingLevel::STOP:
                mLogger.LogInfo() << "wiperSend: [STOP, " << wipingIntervalSend << "]";
                break;
            default:
                break;
            }
        }
        // Wiper End

        // TMoodLamp Start

        void
        ServiceCreator::RequestMImSetBrightness(const std::uint16_t &Brightness)
        {
            // mLogger.LogInfo() << __func__ << Brightness;
            this->BrightnessSend = Brightness;
            return;
        }
        void
        ServiceCreator::RequestMImSetMode(const lmp::mode::SoaMImMoodeMode &mood)
        {
            // mLogger.LogInfo() << __func__;
            this->moodSend = mood;
            return;
        }
        void
        ServiceCreator::RequestMImSetRGBColor(const std::uint8_t &ColorIndex)
        {
            // mLogger.LogInfo() << __func__ << ColorIndex;
            this->ColorIndexSend = ColorIndex;
            return;
        }

        // TMoodLamp End

        // BmsInfoSrv Start

        void
        ServiceCreator::notifyBmsInfo(bms::input::InputData &info)
        {
            // mLogger.LogInfo() << __func__;
            bmsInfoSkeletonImpl->sendEventBmsInfo(info);
            return;
        }

        // BmsInfoSrv End

        // MsgInfoSrv Start

        void
        ServiceCreator::notifyMsgInfo(const bms::output::OutputData &output)
        {
            // mLogger.LogInfo() << __func__;
            std::string binFilePath = "/home/popcornsar/src/FTP/msg.bin";
            std::string preFilePath = "/home/popcornsar/src/FTP/msg.pre";

            // FTP 전송용 파일 만들기
            // 1. "msg.bin" 파일을 생성하고 데이터 기록
            std::ofstream binFile(binFilePath, std::ios::binary);
            if (!binFile)
            {
                mLogger.LogError() << "Failed to create msg.bin";
                return;
            }
            // InterfaceIDforDbg(4bytes) 제외하고 나머지 데이터의 크기를 계산
            size_t outputSize = sizeof(OutputData) - sizeof(output.InterfaceIDforDbg);

            // OutputData에서 InterfaceIDforDbg를 제외한 나머지 데이터를 메모리에 복사
            binFile.write(reinterpret_cast<const char *>(&output) + sizeof(output.InterfaceIDforDbg), outputSize);
            if (binFile.fail())
            {
                mLogger.LogInfo() << "파일 쓰기 실패";
            }
            else
            {
                // mLogger.LogInfo() << "msg.bin 생성";
            }
            binFile.close();

            // 2. "msg.pre" 삭제
            // mLogger.LogInfo() << "msg.pre 삭제 시도";
            std::remove(preFilePath.c_str());

            // 3. "msg.bin" -> "msg.pre"로 이름 변경
            // mLogger.LogInfo() << "msg.bin -> msg.pre 확장자 변경";
            std::rename(binFilePath.c_str(), preFilePath.c_str());

            return;
        }

        // MsgInfoSrv End

        // AccrPedal Start

        void
        ServiceCreator::notifyAccrPedal(eevp::simulation::type::VCS_AccrPedal &accrPedal)
        {
            // mLogger.LogInfo() << __func__;
            accrPedal = this->accrPedal;
            return;
        }

        // AccrPedal End

        // EnvMonitor Start

        void
        ServiceCreator::notifyHumidity(eevp::simulation::type::TMS_EnvZoneHumidityAry &envZoneHumidityAry)
        {
            mLogger.LogInfo() << __func__;
            envZoneHumidityAry = this->envZoneHumidityAry;
            return;
        }

        // EnvMonitor End

        // Gear Start

        void
        ServiceCreator::notifyGear(eevp::simulation::type::VCS_Gear &gear)
        {
            // mLogger.LogInfo() << __func__;
            gear = this->gear;
            return;
        }

        // Gear End

        // BrakePedal Start

        void
        ServiceCreator::notifyPosn(eevp::simulation::type::VCS_BrakePosn &brakePosn)
        {
            // mLogger.LogInfo() << __func__;
            brakePosn = this->brakePosn;
            return;
        }

        void
        ServiceCreator::notifyBrakeSwitch(eevp::simulation::type::VCS_BrakeSwitch &brakeSwitch)
        {
            // mLogger.LogInfo() << __func__;
            brakeSwitch = this->brakeSwitch;
            return;
        }

        // BrakePedal End

        // VehSpd Start

        void
        ServiceCreator::notifyVehSpd(eevp::simulation::type::VCS_VehSpd &vehSpd)
        {
            // mLogger.LogInfo() << __func__;
            vehSpd = this->vehSpd;
            return;
        }

        // VehSpd End

        // SnsrUss Start

        void ServiceCreator::ntfSonarInfo(eevp::simulation::type::USSSonarInfo &ussSonarInfo)
        {
            mLogger.LogInfo() << __func__;
            ussSonarInfo = this->ussSonarInfo;
            return;
        }

        bool ServiceCreator::isDetect()
        {
            // mLogger.LogInfo() << __func__;
            return detect;
        }

        // SnsrUss End

        // Lotte Start

        void ServiceCreator::NotifyDmsCurr(lotte::type::DmsGzDtctn &dmsGzDtctn)
        {
            mLogger.LogInfo() << __func__;
            return;
        }

        void ServiceCreator::NotifyDmsStatus(bool &result)
        {
            // mLogger.LogInfo() << __func__;
            result = this->dmsStatus;
            return;
        }

        void ServiceCreator::NotifySmartFilmCurr(lotte::type::SideTransparency &result)
        {
            // mLogger.LogInfo() << __func__;
            return;
        }

        void ServiceCreator::NotifySmartFilmStatus(bool &result)
        {
            // mLogger.LogInfo() << __func__;
            result = this->smartFilmStatus;
            return;
        }

        void ServiceCreator::SmartFilmControl(const std::int32_t &windowLoc, const std::int32_t &transparence)
        {
            // mLogger.LogInfo() << __func__;
            this->windowLocSend = windowLoc;
            this->transparenceSend = transparence;
            return;
        }

        // Lotte End

        // SESL Start

        void ServiceCreator::SESL_Receive(ara::SESL::Vehicle_Data &Receive_Argument)
        {
            // mLogger.LogInfo() << __func__;
            Receive_Argument = this->vehicle_Data_receive;
            return;
        }

        void ServiceCreator::SESL_Send(const ara::SESL::Vehicle_Data &Send_Argument)
        {
            // mLogger.LogInfo() << __func__;
            this->vehicle_Data_send = Send_Argument;
            return;
        }

        // SESL End

        bool
        ServiceCreator::setRunningState()
        {
            ara::exec::ExecutionClient executionClient;
            auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);
            if (exec.HasValue())
            {
                mLogger.LogInfo() << "ServiceCreator app in Running State";
            }
            else
            {
                mLogger.LogError() << exec.Error().Message();
                return false;
            }
            return true;
        }

        bool ServiceCreator::initialize() // 변수 초기화
        {
            mLogger.LogInfo() << __func__;

            // Wiper
            this->wipingLevelSend = eevp::simulation::BCM_WipingLevel::STOP;
            this->wipingIntervalSend = 0U;

            // AccrPedal
            this->accrPedal.valid = true;

            // BrakePedal
            this->brakePosn.stroke = 19U;
            this->brakeSwitch.switchStatus = static_cast<eevp::simulation::type::SwitchStatus>(1U);

            // Gear
            this->gear.GearStatus = static_cast<eevp::simulation::type::GearStatus>(2U);

            // VehSpd
            this->vehSpd.absoluteValue = 15U;

            // TMoodLamp
            this->BrightnessSend = 0;
            this->moodSend = lmp::mode::SoaMImMoodeMode::kCARE_MOOD;
            this->ColorIndexSend = 0;

            // Lotte
            this->windowLocSend = 0;
            this->transparenceSend = 0;

            // SESL
            this->vehicle_Data_send.Accel = false;
            this->vehicle_Data_send.Brake = 0;
            this->vehicle_Data_send.Light = 0;
            this->vehicle_Data_send.Beep = false;
            this->vehicle_Data_send.Beep_Large = false;

            return true;
        }

        bool ServiceCreator::startServiceCreatorStub()
        {
            mLogger.LogInfo() << __func__;

            ara::core::InstanceSpecifier specifier_WiperWash("ServiceCreator/AA/PPort_BCM_WiperWash");
            ara::core::InstanceSpecifier specifier_TMoodLamp("ServiceCreator/AA/PPort_EevpControlSoaMIm");
            ara::core::InstanceSpecifier specifier_BmsInfo("ServiceCreator/AA/PPort_BmsInfo");
            ara::core::InstanceSpecifier specifier_AccrPedal("ServiceCreator/AA/PPort_VCS_AccrPedal");
            ara::core::InstanceSpecifier specifier_EnvMonitor("ServiceCreator/AA/PPort_TMS_EnvMonitor");
            ara::core::InstanceSpecifier specifier_Gear("ServiceCreator/AA/PPort_VCS_Gear");
            ara::core::InstanceSpecifier specifier_BrakePedal("ServiceCreator/AA/PPort_VCS_BrakePedal");
            ara::core::InstanceSpecifier specifier_VehSpd("ServiceCreator/AA/PPort_VCS_VehSpd");
            ara::core::InstanceSpecifier specifier_SnsrUss("ServiceCreator/AA/PPort_Snsr_USS");
            ara::core::InstanceSpecifier specifier_Lotte("ServiceCreator/AA/PPort_LotteService");
            ara::core::InstanceSpecifier specifier_SESL("ServiceCreator/AA/PPort_SESLService");

            wiperSkeletonImpl = std::make_shared<eevp::simulation::WiperSkeletonImpl>(specifier_WiperWash);
            auto wiperListener = std::make_shared<WiperListener>(this);
            wiperSkeletonImpl->setEventListener(wiperListener);
            wiperSkeletonImpl->OfferService();

            tEevpControlSoaMImSkeletonImpl = std::make_shared<eevp::simulation::TEevpControlSoaMImSkeletonImpl>(specifier_TMoodLamp);
            auto tMoodLampListener = std::make_shared<TMoodLampListener>(this);
            tEevpControlSoaMImSkeletonImpl->setEventListener(tMoodLampListener);
            tEevpControlSoaMImSkeletonImpl->OfferService();

            seslServiceSkeletonImpl = std::make_shared<eevp::simulation::SESLServiceSkeletonImpl>(specifier_SESL);
            auto seslListener = std::make_shared<SESLServiceListener>(this);
            seslServiceSkeletonImpl->setEventListener(seslListener);
            seslServiceSkeletonImpl->OfferService();

            bmsInfoSkeletonImpl = std::make_shared<eevp::simulation::BmsInfoSkeletonImpl>(specifier_BmsInfo);
            bmsInfoSkeletonImpl->OfferService();

            accrPedalSkeletonImpl = std::make_shared<eevp::simulation::AccrPedalSkeletonImpl>(specifier_AccrPedal);
            auto accrPedalListener = std::make_shared<AccrPedalListener>(this);
            accrPedalSkeletonImpl->setEventListener(accrPedalListener);
            accrPedalSkeletonImpl->OfferService();

            envMonitorSkeletonImpl = std::make_shared<eevp::simulation::EnvMonitorSkeletonImpl>(specifier_EnvMonitor);
            auto envMonitorListener = std::make_shared<EnvMonitorListener>(this);
            envMonitorSkeletonImpl->setEventListener(envMonitorListener);
            envMonitorSkeletonImpl->OfferService();

            gearSkeletonImpl = std::make_shared<eevp::simulation::GearSkeletonImpl>(specifier_Gear);
            auto gearListener = std::make_shared<GearListener>(this);
            gearSkeletonImpl->setEventListener(gearListener);
            gearSkeletonImpl->OfferService();

            brakePedalSkeletonImpl = std::make_shared<eevp::simulation::BrakePedalSkeletonImpl>(specifier_BrakePedal);
            auto brakePedalListener = std::make_shared<BrakePedalListener>(this);
            brakePedalSkeletonImpl->setEventListener(brakePedalListener);
            brakePedalSkeletonImpl->OfferService();

            vehSpdSkeletonImpl = std::make_shared<eevp::simulation::VehSpdSkeletonImpl>(specifier_VehSpd);
            auto vehSpdListener = std::make_shared<VehSpdListener>(this);
            vehSpdSkeletonImpl->setEventListener(vehSpdListener);
            vehSpdSkeletonImpl->OfferService();

            snsrUssSkeletonImpl = std::make_shared<eevp::simulation::SnsrUssSkeletonImpl>(specifier_SnsrUss);
            auto snsrUssListener = std::make_shared<SnsrUssListener>(this);
            snsrUssSkeletonImpl->setEventListener(snsrUssListener);
            snsrUssSkeletonImpl->OfferService();

            lotteSkeletonImpl = std::make_shared<eevp::simulation::LotteSkeletonImpl>(specifier_Lotte);
            auto lotteListener = std::make_shared<LotteListener>(this);
            lotteSkeletonImpl->setEventListener(lotteListener);
            lotteSkeletonImpl->OfferService();

            return true;
        }

        bool ServiceCreator::startMsgInfoProxy()
        {
            mLogger.LogInfo() << __func__;
            msgInfoProxyImpl = std::make_shared<eevp::simulation::MsgInfoProxyImpl>();
            auto msgInfoListener = std::make_shared<MsgInfoListener>(this);
            msgInfoProxyImpl->setEventListener(msgInfoListener);
            msgInfoProxyImpl->init();

            return true;
        }

        bool ServiceCreator::startSocketClient()
        {
            mLogger.LogInfo() << __func__;
            if (pthread_create(&thread_socket_km, nullptr, ServiceCreator::socket_km, this) != 0)
            {
                mLogger.LogInfo() << "KM 스레드 생성 실패";
                return false;
            }
            if (pthread_create(&thread_socket_recv, nullptr, ServiceCreator::socket_recv, this) != 0)
            {
                mLogger.LogInfo() << "Recv 스레드 생성 실패";
                return false;
            }
            if (pthread_create(&thread_socket_send, nullptr, ServiceCreator::socket_send, this) != 0)
            {
                mLogger.LogInfo() << "Send 스레드 생성 실패";
                return false;
            }
            return true;
        }

        void *ServiceCreator::socket_km(void *inst)
        {
            ServiceCreator *instance = static_cast<ServiceCreator *>(inst);
            instance->mLogger.LogInfo() << __func__;

            ///////////////////// 값을 소켓으로 받아서 recv데이터로 대입 /////////////////////

            int sock;
            char recvMessage[BUF_SIZE];
            std::string recvMessage_s;
            int str_len;
            struct sockaddr_in serv_adr;

            sock = socket(AF_INET, SOCK_STREAM, 0);

            if (sock == -1)
            {
                instance->mLogger.LogInfo() << "km_socket() error";
                return nullptr;
            }

            // memset(&serv_adr, 0, sizeof(serv_adr));
            serv_adr.sin_family = AF_INET;
            serv_adr.sin_addr.s_addr = inet_addr(KM_IP);
            serv_adr.sin_port = htons(KM_PORT);
            // 클라이언트가 서버와의 연결을 위해 연결요청을 한다.
            if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
                instance->mLogger.LogInfo() << "km_socket() error!";
            else
                instance->mLogger.LogInfo() << "Connected...........";

            while (mRunning)
            {
                str_len = read(sock, recvMessage, BUF_SIZE);
                if (str_len == -1)
                {
                    instance->mLogger.LogInfo() << "km_socket is broken";
                    break;
                }
                recvMessage[str_len] = 0;
                // instance->mLogger.LogInfo() << "Message from km: " << recvMessage;

                try
                {
                    json recvData = json::parse(recvMessage);

                    if (recvData.contains("CyclingMode"))
                        instance->extractCyclingmode(recvData["CyclingMode"]);
                    if (recvData.contains("SmartFilm"))
                        instance->extractSmartFilm(recvData["SmartFilm"]);
                    if (recvData.contains("MoodLamp"))
                        instance->extractMoodLamp(recvData["MoodLamp"]);
                }
                catch (const json::parse_error &e)
                {
                    instance->mLogger.LogInfo() << "JSON parse error: " << e.what();
                }
                // instance->mLogger.LogInfo() << "데이터 추출 완료";
            }
            return nullptr;
        }

        void ServiceCreator::extractCyclingmode(const json &cyclingData)
        {
            cyclingMode = cyclingData.value("mode", static_cast<std::uint8_t>(2));
            return;
        }

        void ServiceCreator::extractSmartFilm(const json &sfData)
        {
            int index = 0;
            for (const auto &item : sfData)
            {
                if (index >= 10)
                    break;
                sf[index].isOn = item.value("isOn", false);
                sf[index].transparentRatio = item.value("transparentRatio", 0);
            }
        }

        void ServiceCreator::extractMoodLamp(const json &mlData)
        {
            int index = 0;
            for (const auto &item : mlData)
            {
                if (index >= 15)
                    break;
                ml[index].isOn = item["isOn"];
                ml[index].isOn = item.value("isOn", false);
                ml[index].brightness = item.value("brightness", 0);
                ml[index].color = item.value("color", 0x000000);
            }
        }

        void *ServiceCreator::socket_send(void *inst)
        {
            ServiceCreator *instance = static_cast<ServiceCreator *>(inst);
            instance->mLogger.LogInfo() << __func__;

            int sock;
            char sendMessage[BUF_SIZE];
            std::string sendMessage_s;
            int str_len;
            struct sockaddr_in serv_adr;

            sock = socket(AF_INET, SOCK_STREAM, 0);

            if (sock == -1)
            {
                instance->mLogger.LogInfo() << "send_socket() error";
                return nullptr;
            }

            // memset(&serv_adr, 0, sizeof(serv_adr));
            serv_adr.sin_family = AF_INET;
            serv_adr.sin_addr.s_addr = inet_addr(SOCKET_IP);
            serv_adr.sin_port = htons(SOCKET_PORT);

            // 클라이언트가 서버와의 연결을 위해 연결요청을 한다.
            if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
                instance->mLogger.LogInfo() << "send_connect() error!";
            else
                instance->mLogger.LogInfo() << "send_Connected...........";

            while (mRunning)
            {
                json data = instance->prepareData(); //데이터 준비
                sendMessage_s = data.dump();
                strcpy(sendMessage, sendMessage_s.c_str());
                str_len = write(sock, sendMessage, strlen(sendMessage)); // 데이터 전송
                if (str_len == -1)
                {
                    instance->mLogger.LogInfo() << "send_Connect is broken";
                    break;
                }
                sendMessage[str_len] = 0;
                // instance->mLogger.LogInfo() << "Send to Server: " << sendMessage;
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }

            // 서버 소켓과 연결 후 송신해야 하는 이벤트 발생 시 데이터 송신
            return nullptr;
        }

        void *ServiceCreator::socket_recv(void *inst)
        {
            ServiceCreator *instance = static_cast<ServiceCreator *>(inst);
            instance->mLogger.LogInfo() << __func__;

            ///////////////////// 값을 소켓으로 받아서 recv데이터로 대입 /////////////////////

            int sock;
            char recvMessage[BUF_SIZE];
            std::string recvMessage_s;
            int str_len;
            struct sockaddr_in serv_adr;

            sock = socket(AF_INET, SOCK_STREAM, 0);

            if (sock == -1)
            {
                instance->mLogger.LogInfo() << "recv_socket() error";
                return nullptr;
            }

            // memset(&serv_adr, 0, sizeof(serv_adr));
            serv_adr.sin_family = AF_INET;
            serv_adr.sin_addr.s_addr = inet_addr(SOCKET_IP);
            serv_adr.sin_port = htons(SOCKET_PORT);
            int a = 0;
            // 클라이언트가 서버와의 연결을 위해 연결요청을 한다.
            if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
                instance->mLogger.LogInfo() << "recv_connect() error!";
            else
                instance->mLogger.LogInfo() << "Connected...........";

            // 소켓 read 루프 => read 후 다른 서비스로 스켈레톤을 통해 전송 구현 필요
            while (mRunning)
            {
                str_len = read(sock, recvMessage, BUF_SIZE); // 데이터 수신
                if (str_len == -1)
                {
                    instance->mLogger.LogInfo() << "recv_Connect is broken";
                    break;
                }
                recvMessage[str_len] = 0;
                // instance->mLogger.LogInfo() << "Message from server: " << recvMessage;
                a++;
                try
                {
                    // JSON 객체로 변환
                    json recvData = json::parse(recvMessage); // 데이터 파싱
                    instance->mLogger.LogInfo() << a << "번째 데이터 추출";

                    // JSON 객체에서 데이터 추출
                    if (recvData.contains("Wiper"))
                        instance->extractWiperData(recvData["Wiper"]);

                    if (recvData.contains("Battery"))
                        instance->extractBatteryData(recvData["Battery"]);

                    if (recvData.contains("AccrPedal"))
                        instance->extractAccrPedalData(recvData["AccrPedal"]);

                    if (recvData.contains("EnvMonitor"))
                        instance->extractEnvMonitorData(recvData["EnvMonitor"]);

                    if (recvData.contains("Gear"))
                        instance->extractGearData(recvData["Gear"]);

                    if (recvData.contains("BrakePedal"))
                        instance->extractBrakePedalData(recvData["BrakePedal"]);

                    if (recvData.contains("VehSpd"))
                        instance->extractVehSpdData(recvData["VehSpd"]);

                    if (recvData.contains("SonarInfo"))
                        instance->extractSnsrUssData(recvData["SonarInfo"]);

                    if (recvData.contains("Lotte"))
                        instance->extractLotteData(recvData["Lotte"]);

                    if (recvData.contains("SESL"))
                        instance->extractSESLData(recvData["SESL"]);

                    // 받은 데이터 로그 출력 (디버깅용)
                    // instance->getWiperRecv();

                    // 각 구독앱에 Update
                    // Wiper
                    instance->wiperSkeletonImpl->updateWipingLevel(instance->wipingLevelReceive);
                    instance->wiperSkeletonImpl->updateWipingInterval(instance->wipingIntervalReceive);

                    // BmsInfo
                    instance->notifyBmsInfo(instance->bmsInfo);
                }
                catch (const json::parse_error &e)
                {
                    instance->mLogger.LogInfo() << "JSON parse error: " << e.what();
                }
                // instance->mLogger.LogInfo() << "데이터 추출 완료";
            }
            return nullptr;
        }

        // Wiper 데이터 추출
        void ServiceCreator::extractWiperData(const json &wiperData)
        {
            wipingLevelReceive = static_cast<eevp::simulation::BCM_WipingLevel>(wiperData.value("wipingLevel", 0));
            wipingIntervalReceive = wiperData.value("wipingInterval", 0);
            if (!requestwipinglevel)
                wipingLevelSend = wipingLevelReceive;
            if (!requestwipinginterval)
                wipingIntervalSend = wipingIntervalReceive;
        }

        // Battery 데이터 추출
        void ServiceCreator::extractBatteryData(const json &batteryData)
        {
            bmsInfo.InterfaceIDforDbg = batteryData.value("InterfaceIDforDbg", 0);
            bmsInfo.DataSerialNumber = batteryData.value("DataSerialNumber", 0);
            bmsInfo.VehicleUniqueSnr = batteryData.value("VehicleUniqueSnr", 0);
            bmsInfo.EMS_BatteryAvgTemperature = batteryData.value("EMS_BatteryAvgTemperature", 0);
            bmsInfo.EMS_ChargerOutputVolt = batteryData.value("EMS_ChargerOutputVolt", 0);
            bmsInfo.EMS_ChargerOutputCurr = batteryData.value("EMS_ChargerOutputCurr", 0);
            bmsInfo.EMS_ReturnCode = batteryData.value("EMS_ReturnCode", 0);
            bmsInfo.EMS_ConnectionStatus = batteryData.value("EMS_ConnectionStatus", 0);
            bmsInfo.EMS_TargetVolt = batteryData.value("EMS_TargetVolt", 0);
            bmsInfo.EMS_TargetCurr = batteryData.value("EMS_TargetCurr", 0);
            bmsInfo.EMS_RemainHour = batteryData.value("EMS_RemainHour", 0);
            bmsInfo.EMS_RemainMin = batteryData.value("EMS_RemainMin", 0);
            bmsInfo.EMS_HVBattSOC = batteryData.value("EMS_HVBattSOC", 0);
            bmsInfo.EMS_HVBattSOE = batteryData.value("EMS_HVBattSOE", 0);
            bmsInfo.EMS_HVBattSOH = batteryData.value("EMS_HVBattSOH", 0);
            bmsInfo.EMS_BattVolt = batteryData.value("EMS_BattVolt", 0);
            bmsInfo.EMS_BattCurr = batteryData.value("EMS_BattCurr", 0);
            bmsInfo.EMS_Obc_dcVolt = batteryData.value("EMS_Obc_dcVolt", 0);
            bmsInfo.EMS_Obc_dcCurr = batteryData.value("EMS_Obc_dcCurr", 0);
            bmsInfo.EMS_Avn_ChargeCurrSetSts = batteryData.value("EMS_Avn_ChargeCurrSetSts", 0);
            bmsInfo.EMS_HVBatt_MainRlyCloseReq = batteryData.value("EMS_HVBatt_MainRlyCloseReq", 0);
            bmsInfo.EMS_HVBatt_preChrgRlyStatus = batteryData.value("EMS_HVBatt_preChrgRlyStatus", 0);
            bmsInfo.EMS_HVBatt_posRlyStatus = batteryData.value("EMS_HVBatt_posRlyStatus", 0);
            bmsInfo.EMS_HVBatt_negRlyStatus = batteryData.value("EMS_HVBatt_negRlyStatus", 0);
            bmsInfo.EMS_Avn_DcChargingTargetSetSts = batteryData.value("EMS_Avn_DcChargingTargetSetSts", 0);
            bmsInfo.EMS_Avn_AcChargingTargetSetSts = batteryData.value("EMS_Avn_AcChargingTargetSetSts", 0);
        }

        // AccrPedal 데이터 추출
        void ServiceCreator::extractAccrPedalData(const json &accrPedalData)
        {
            accrPedal.valid = accrPedalData.value("valid", false);
            accrPedal.switchStatus = static_cast<eevp::simulation::type::SwitchStatus>(accrPedalData.value("switchStatus", 0));
            accrPedal.position = accrPedalData.value("position", static_cast<std::uint16_t>(0));
            accrPedal.kickdownStatus = static_cast<eevp::simulation::type::KickdownStatus>(accrPedalData.value("kickdownStatus", 0));
            accrPedal.faultStatus = accrPedalData.value("faultStatus", static_cast<std::uint8_t>(0));
            accrPedal.positionRate = accrPedalData.value("positionRate", 0);
        }

        // EnvMonitor 데이터 추출
        void ServiceCreator::extractEnvMonitorData(const json &envMonitorData)
        {
            envZoneHumidityAry[0].humidity = envMonitorData.value("interiorhumidity", 0);
            envZoneHumidityAry[1].humidity = envMonitorData.value("exteriorhumidity", 0);
        }

        // Gear 데이터 추출
        void ServiceCreator::extractGearData(const json &gearData)
        {
            gear.GearStatus = static_cast<eevp::simulation::type::GearStatus>(gearData.value("status", 0));
            gear.valid = gearData.value("valid", false);
        }

        // BrakePedal 데이터 추출
        void ServiceCreator::extractBrakePedalData(const json &brakePedalData)
        {
            brakePosn.stroke = brakePedalData.value("stroke", static_cast<std::int16_t>(0));
            brakePosn.strokeValid = brakePedalData.value("strokeValid", false);
            brakePosn.position = brakePedalData.value("position", static_cast<std::int16_t>(0));
            brakePosn.positionValid = brakePedalData.value("positionValid", false);
            brakePosn.strokeRate = brakePedalData.value("strokeRate", static_cast<std::int16_t>(0));
            brakePosn.strokeRateValid = brakePedalData.value("strokeRateValid", false);
            brakeSwitch.switchStatus = static_cast<eevp::simulation::type::SwitchStatus>(brakePedalData.value("switchStatus", 0));
            brakeSwitch.switchValid = brakePedalData.value("switchValid", false);
        }

        // VehSpd 데이터 추출
        void ServiceCreator::extractVehSpdData(const json &vehSpdData)
        {
            vehSpd.absoluteValue = vehSpdData.value("absoluteValue", 0);
            vehSpd.actualValue = vehSpdData.value("actualValue", 0);
            vehSpd.direction = static_cast<eevp::simulation::type::DirectionVeh>(vehSpdData.value("direction", 0));
            vehSpd.directionValid = vehSpdData.value("directionValid", false);
            vehSpd.valid = vehSpdData.value("valid", false);
        }

        // SnsrUss 데이터 추출
        void ServiceCreator::extractSnsrUssData(const json &sonarInfoData)
        {
            ussSonarInfo.pasSonarEchoTof1 = sonarInfoData.value("pasSonarEchoTof1", static_cast<std::uint16_t>(0));
            ussSonarInfo.pasSonarEchoWidth1 = sonarInfoData.value("pasSonarEchoWidth1", static_cast<std::uint16_t>(0));
            ussSonarInfo.pasSonarEchoPeak1 = sonarInfoData.value("pasSonarEchoPeak1", static_cast<std::uint16_t>(0));
            ussSonarInfo.pasSonarEchoTof2 = sonarInfoData.value("pasSonarEchoTof2", static_cast<std::uint16_t>(0));
            ussSonarInfo.pasSonarEchoWidth2 = sonarInfoData.value("pasSonarEchoWidth2", static_cast<std::uint16_t>(0));
            ussSonarInfo.pasSonarEchoPeak2 = sonarInfoData.value("pasSonarEchoPeak2", static_cast<std::uint16_t>(0));
            ussSonarInfo.pasSonarEmit = sonarInfoData.value("pasSonarEmit", static_cast<std::uint8_t>(0));
            ussSonarInfo.sonarRingingTimer = sonarInfoData.value("sonarRingingTimer", static_cast<std::uint16_t>(0));
        }

        void ServiceCreator::extractLotteData(const json &lotteData)
        {
            dmsStatus = lotteData.value("dmsStatus", false);
            smartFilmStatus = lotteData.value("smartFilmStatus", false);
        }

        void ServiceCreator::extractSESLData(const json &seslData)
        {
            vehicle_Data_receive.Fwd_Distance = seslData.value("Fwd_Distance", static_cast<std::double_t>(0));
            vehicle_Data_receive.Rear_Distance = seslData.value("Rear_Distance", static_cast<std::double_t>(0));
        }

        // 데이터를 JSON으로 묶어서 반환
        json ServiceCreator::prepareData()
        {
            json sendData;

            // 데이터 송신 시 잠금해제
            requestwipinginterval = false;
            requestwipinglevel = false;

            // MoodLamp 데이터
            sendData["MoodLamp"] = {
                {"Brightness", this->BrightnessSend},
                {"mood", static_cast<int>(this->moodSend)},
                {"ColorIndex", this->ColorIndexSend}};

            // Wiper 데이터
            sendData["Wiper"] = {
                {"wipingLevel", this->wipingLevelSend},
                {"wipingInterval", this->wipingIntervalSend}};

            // Lotte 데이터
            sendData["Lotte"] = {
                {"windowLoc", this->windowLocSend},
                {"transparence", this->transparenceSend}};

            sendData["SESL"] = {
                {"Accel", this->vehicle_Data_send.Accel},
                {"Brake", this->vehicle_Data_send.Brake},
                {"Light", this->vehicle_Data_send.Light},
                {"Beep", this->vehicle_Data_send.Beep},
                {"Beep_Large", this->vehicle_Data_send.Beep_Large}};

            return sendData;
        }

    }
}