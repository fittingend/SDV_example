#include "intelligentwiper/aa/intelligentwiper.h"
#include <chrono>

namespace intelligentwiper
{
    namespace aa
    {

        IntelligentWiper::IntelligentWiper()
            : m_logger(ara::log::CreateLogger("INTW", "SWC", ara::log::LogLevel::kVerbose)), m_running(false), m_workers(4)
        {
        }

        IntelligentWiper::~IntelligentWiper()
        {
            Terminate();
        }

        bool IntelligentWiper::Initialize()
        {
            m_logger.LogVerbose() << "IntelligentWiper::Initialize";

            m_RPort_VehicleInfo = std::make_unique<intelligentwiper::aa::port::RPort_VehicleInfo>();
            m_RPort_Wiper = std::make_unique<intelligentwiper::aa::port::RPort_Wiper>();
            m_RPort_SubscriptionManagement = std::make_unique<intelligentwiper::aa::port::RPort_SubscriptionManagement>();

            m_logic = std::make_unique<intelligentwiper::aa::Logic>(m_RPort_Wiper.get(), m_RPort_VehicleInfo.get());

            return true;
        }

        void IntelligentWiper::Start()
        {
            m_logger.LogInfo() << "IntelligentWiper::Start";

            m_RPort_Wiper->Start();
            m_RPort_VehicleInfo->Start();
            m_RPort_SubscriptionManagement->Start();

            m_running = true;

            // Logic 실행 스레드 시작
            m_logicThread = std::thread([this]()
                                        {
        while (m_running)
        {
            m_logic->Execute();  // Logic 한 사이클 실행
            std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // 100ms 주기
        } });
        }

        void IntelligentWiper::Terminate()
        {
            m_logger.LogInfo() << "IntelligentWiper::Terminate";

            m_running = false;

            if (m_logicThread.joinable())
                m_logicThread.join();

            m_RPort_Wiper->Terminate();
            m_RPort_VehicleInfo->Terminate();
            m_RPort_SubscriptionManagement->Terminate();
        }

        void IntelligentWiper::Run()
        {
            m_logger.LogInfo() << "IntelligentWiper::Run";

            // Run에서는 Port 사이클 실행
            m_workers.Async([this]
                            { m_RPort_Wiper->ReceiveFieldsoaWiperDeviceNormalCyclic(); });
            m_workers.Async([this]
                            { m_RPort_Wiper->ReceiveFieldsoaWiperStatusCyclic(); });
            m_workers.Async([this]
                            { m_RPort_Wiper->ReceiveFieldsoaWiperSwVersionCyclic(); });
            m_workers.Async([this]
                            { m_RPort_VehicleInfo->ReceiveFieldsoaVehicleInfoCyclic(); });
            m_workers.Async([this]
                            { m_RPort_SubscriptionManagement->ReceiveEventnotifySubscriptionInfoCyclic(); });

            while (1)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                m_logger.LogInfo() << "IntelligentWiper::Test";
            }

            m_workers.Wait();
        }

    } // namespace aa
} // namespace intelligentwiper
