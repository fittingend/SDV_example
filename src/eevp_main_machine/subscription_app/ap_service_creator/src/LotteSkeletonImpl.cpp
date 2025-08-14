#include "skeleton/LotteSkeletonImpl.h"

namespace eevp
{
    namespace simulation
    {
        uint16_t countF = 3000; // 30second scenario
        LotteSkeletonImpl::LotteSkeletonImpl(
            ara::core::InstanceSpecifier instanceSpec,
            ara::com::MethodCallProcessingMode mode) : listener{nullptr},
                                                       LotteServiceSkeleton(instanceSpec, mode),
                                                       mLogger(ara::log::CreateLogger("LTSV", "LTSV", ara::log::LogLevel::kInfo))

        {
            mLogger.LogInfo() << __func__;
        }

        LotteSkeletonImpl::~LotteSkeletonImpl()
        {
            StopOfferService();
        }

        void
        LotteSkeletonImpl::setEventListener(std::shared_ptr<ILotteListener> _listener)
        {
            listener = _listener;
        }

        Future<skeleton::LotteServiceSkeleton::DmsCurrOutput>
        LotteSkeletonImpl::DmsCurr()
        {
            // mLogger.LogInfo() << __func__;

            skeleton::LotteServiceSkeleton::DmsCurrOutput response;
            Promise<DmsCurrOutput> promise;

            uint16_t time = --countF;

            if (time > 2900)
            {
                response.result.gazeZone = lotte::type::GazaZone::FRONT_WINDSHIELD;
            }
            else if (time > 2700)
            {
                response.result.gazeZone = lotte::type::GazaZone::DRIVER_SIDE_MIRROR;
            }
            else if (time > 2200)
            {
                response.result.gazeZone = lotte::type::GazaZone::FRONT_WINDSHIELD;
            }
            else if (time > 2000)
            {
                response.result.gazeZone = lotte::type::GazaZone::PASSENGER_SIDE_MIRROR;
            }
            else if (time > 1500)
            {
                response.result.gazeZone = lotte::type::GazaZone::FRONT_WINDSHIELD;
            }
            else if (time > 1400)
            {
                response.result.gazeZone = lotte::type::GazaZone::DRIVER_SIDE_MIRROR;
            }
            else if (time > 1300)
            {
                response.result.gazeZone = lotte::type::GazaZone::PASSENGER_SIDE_MIRROR;
            }
            else if (time > 700)
            {
                response.result.gazeZone = lotte::type::GazaZone::FRONT_WINDSHIELD;
            }
            else if (time > 600)
            {
                response.result.gazeZone = lotte::type::GazaZone::BACK_MIRROR;
            }
            else
            {
                response.result.gazeZone = lotte::type::GazaZone::FRONT_WINDSHIELD;
            }

            promise.set_value(response);
            return promise.get_future();
        }

        Future<skeleton::LotteServiceSkeleton::DmsStatusOutput> LotteSkeletonImpl::DmsStatus()
        {
            // mLogger.LogInfo() << __func__;

            skeleton::LotteServiceSkeleton::DmsStatusOutput response;
            Promise<DmsStatusOutput> promise;

            if (listener != nullptr)
            {
                listener->NotifyDmsStatus(response.result);
            }

            promise.set_value(response);
            return promise.get_future();
        }

        Future<skeleton::LotteServiceSkeleton::SmartFilmControlOutput>
        LotteSkeletonImpl::SmartFilmControl(const std::uint8_t &windowLoc, const std::uint8_t &transparence)
        {

            skeleton::LotteServiceSkeleton::SmartFilmControlOutput response;
            Promise<SmartFilmControlOutput> promise;

            if (listener != nullptr)
            {
                listener->SmartFilmControl(windowLoc, transparence);
            }

            response.result = true;

            promise.set_value(response);
            return promise.get_future();
        }

        Future<skeleton::LotteServiceSkeleton::SmartFilmCurrOutput>
        LotteSkeletonImpl::SmartFilmCurr(const std::uint8_t &windowLoc)
        {
            // mLogger.LogInfo() << __func__;

            skeleton::LotteServiceSkeleton::SmartFilmCurrOutput response;
            Promise<SmartFilmCurrOutput> promise;

            if (listener != nullptr)
            {
                response.result.currTransparency = 80;
                response.result.zone = windowLoc;
                // listener->NotifySmartFilmCurr(response.result);
            }

            promise.set_value(response);
            return promise.get_future();
        }

        Future<skeleton::LotteServiceSkeleton::SmartFilmStatusOutput>
        LotteSkeletonImpl::SmartFilmStatus()
        {
            // mLogger.LogInfo() << __func__;

            skeleton::LotteServiceSkeleton::SmartFilmStatusOutput response;
            Promise<SmartFilmStatusOutput> promise;

            if (listener != nullptr)
            {
                listener->NotifySmartFilmStatus(response.result);
            }

            promise.set_value(response);
            return promise.get_future();
        }
    }
}
