#include "skeleton/WiperSkeletonImpl.h"
#include "ServiceCreator.h"

using namespace ara::core;
using namespace eevp::simulation;

namespace eevp
{
    namespace simulation
    {

        WiperSkeletonImpl::WiperSkeletonImpl(
            ara::core::InstanceSpecifier instanceSpec,
            ara::com::MethodCallProcessingMode mode) : BCM_WiperWashSkeleton(instanceSpec, mode),
                                                       mLogger(ara::log::CreateLogger("PWPR", "PWPR", ara::log::LogLevel::kInfo))
        {
            mLogger.LogInfo() << __func__;

            auto wiperLevel_get_handler = [this]()
            {
                return this->wiperLevelGetter();
            };

            auto wiperInterval_get_handler = [this]()
            {
                return this->wiperIntervalGetter();
            };
            updateWiperLevelContext();
            wiperLevel.RegisterGetHandler(wiperLevel_get_handler);
            wiperInterval.RegisterGetHandler(wiperInterval_get_handler);
        }

        bool WiperSkeletonImpl::updateWiperLevelContext()
        {
            eevp::simulation::BCM_WipingLevel wipingLevel = eevp::simulation::BCM_WipingLevel::STOP;
            this->mWiperLevel = wipingLevel;
            this->mWiperInterval = 0U;
            return true;
        }

        WiperSkeletonImpl::~WiperSkeletonImpl()
        {
            StopOfferService();
        }

        void
        WiperSkeletonImpl::setEventListener(std::shared_ptr<IWiperListener> _listener)
        {
            listener = _listener;
        }

        // proxy 에서 호출
        Future<skeleton::BCM_WiperWashSkeleton::stopWipingOutput> WiperSkeletonImpl::stopWiping()
        {
            // mLogger.LogInfo() << __func__;
            ara::core::Promise<skeleton::BCM_WiperWashSkeleton::stopWipingOutput> promise;
            listener->stopWiping();
            BCM_WiperWashSkeleton::stopWipingOutput output;
            output.returnCode = eevp::simulation::BCM_ReturnCode::SUCCESS;
            promise.set_value(output);
            return promise.get_future();
        }

        Future<skeleton::BCM_WiperWashSkeleton::startWipingOutput> WiperSkeletonImpl::startWiping()
        {
            // mLogger.LogInfo() << __func__;
            ara::core::Promise<skeleton::BCM_WiperWashSkeleton::startWipingOutput> promise;
            listener->startWiping();
            BCM_WiperWashSkeleton::startWipingOutput output;
            output.returnCode = eevp::simulation::BCM_ReturnCode::SUCCESS;
            promise.set_value(output);
            return promise.get_future();
        }

        Future<skeleton::BCM_WiperWashSkeleton::setWipingLevelOutput> WiperSkeletonImpl::setWipingLevel(const eevp::simulation::BCM_WipingLevel &wipingLevel)
        {
            // mLogger.LogInfo() << __func__;
            ara::core::Promise<skeleton::BCM_WiperWashSkeleton::setWipingLevelOutput> promise;
            listener->setWipingLevel(wipingLevel);
            BCM_WiperWashSkeleton::setWipingLevelOutput output;
            output.BCM_ReturnCode = eevp::simulation::BCM_ReturnCode::SUCCESS;
            promise.set_value(output);
            return promise.get_future();
        }

        Future<skeleton::BCM_WiperWashSkeleton::setWipingIntervalOutput> WiperSkeletonImpl::setWipingInterval(const std::uint16_t &wipingInterval)
        {
            // mLogger.LogInfo() << __func__;
            ara::core::Promise<skeleton::BCM_WiperWashSkeleton::setWipingIntervalOutput> promise;
            listener->setWipingInterval(wipingInterval);
            BCM_WiperWashSkeleton::setWipingIntervalOutput output;
            output.BCM_ReturnCode = eevp::simulation::BCM_ReturnCode::SUCCESS;
            promise.set_value(output);
            return promise.get_future();
        }

        Future<simulation::skeleton::fields::wiperInterval::FieldType>
        WiperSkeletonImpl::wiperIntervalGetter()
        {
            // mLogger.LogInfo() << __func__;
            ara::core::Promise<simulation::skeleton::fields::wiperInterval::FieldType> promise;
            // mLogger.LogInfo() << __func__ << "(" << static_cast<uint16_t>(this->mWiperInterval) << ")";
            promise.set_value(this->mWiperInterval);
            return promise.get_future();
        }

        void
        WiperSkeletonImpl::updateWipingInterval(const std::uint16_t &wipingInterval)
        {
            // mLogger.LogInfo() << __func__;
            if (this->mWiperInterval != wipingInterval)
            {
                this->mWiperInterval = wipingInterval;
                auto updateResult = wiperInterval.Update(this->mWiperInterval);
                if (!updateResult.HasValue())
                {
                    mLogger.LogError() << "Update failed: " << updateResult.Error().Message();
                }
                // mLogger.LogInfo() << __func__ << "(" << static_cast<uint16_t>(this->mWiperInterval) << ")";
            }
        }

        Future<simulation::skeleton::fields::wiperLevel::FieldType>
        WiperSkeletonImpl::wiperLevelGetter()
        {
            // mLogger.LogInfo() << __func__;
            ara::core::Promise<simulation::skeleton::fields::wiperLevel::FieldType> promise;
            // mLogger.LogInfo() << __func__ << "(" << static_cast<uint8_t>(this->mWiperLevel) << ")";
            promise.set_value(this->mWiperLevel);
            return promise.get_future();
        }

        void
        WiperSkeletonImpl::updateWipingLevel(const eevp::simulation::BCM_WipingLevel &wipingLevel)
        {
            // mLogger.LogInfo() << __func__;
            if (this->mWiperLevel != wipingLevel)
            {
                this->mWiperLevel = wipingLevel;
                auto updateResult = wiperLevel.Update(this->mWiperLevel);
                if (!updateResult.HasValue())
                {
                    mLogger.LogError() << "Update failed: " << updateResult.Error().Message();
                }
                // mLogger.LogInfo() << __func__ << "(" << static_cast<uint8_t>(wipingLevel) << ")";
            }
        }
    }
}