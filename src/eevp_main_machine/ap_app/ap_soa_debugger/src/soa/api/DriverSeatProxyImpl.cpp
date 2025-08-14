#include <Common.h>
#include "DriverSeatProxyImpl.h"
#include "Log.hpp"
#include <source_location>

#define MAX_SERVICE_SEARCH_TRIAL    50 // 100ms * 50 = 5sec

using namespace ara::core;
using namespace eevp::control;
using namespace eevp::control::driverseat;

namespace eevp {
namespace control {
namespace driverseat {

DriverSeatProxyImpl *DriverSeatProxyImpl::s_mInst = nullptr;

DriverSeatProxyImpl *DriverSeatProxyImpl::GetInstance()
{
    if (DriverSeatProxyImpl::s_mInst == nullptr)
    {
        DriverSeatProxyImpl::s_mInst = new DriverSeatProxyImpl();
    }

    return DriverSeatProxyImpl::s_mInst;
}

void DriverSeatProxyImpl::DeleteInstance()
{
    if (DriverSeatProxyImpl::s_mInst != nullptr)
    {
        delete DriverSeatProxyImpl::s_mInst;
        DriverSeatProxyImpl::s_mInst = nullptr;
    }
}

DriverSeatProxyImpl::DriverSeatProxyImpl()
{
    this->mProxy = nullptr;
    this->mFindHandle = nullptr;
    this->mListener = nullptr;
}

DriverSeatProxyImpl::~DriverSeatProxyImpl()
{
    if (this->mProxy)
    {
        mProxy->StopFindService(*(this->mFindHandle));
        mProxy.reset();
    }
}

void DriverSeatProxyImpl::setEventListener(std::shared_ptr<IDriverSeatListener> _listener)
{
    this->mListener = _listener;
}

bool DriverSeatProxyImpl::init()
{
    LOG_DEBUG() << "(+)\n";

    ara::core::InstanceSpecifier specifier("SOA_Debugger/AA/RPort_SOADBG_DriverSeat");

    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    auto result = proxy::SoaDriverSeatProxy::StartFindService(callback, specifier);
    if (!result.HasValue())
    {
        LOG_INFO() << "SoaDriverSeatProxy StartFindService() Failed\n";
        return false;
    }

    for (int i = 0; i < 50; i++)
    {
        if ((this->mFindHandle) && (this->mProxy))
        {
            LOG_INFO() << "SoaDriverSeatProxy Start Find Service Success\n";
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    LOG_INFO() << "(-)\n";
    return false;
}

bool DriverSeatProxyImpl::getterIsDeviceNormal(eevp::control::SoaDeviceIsNormal& isDeviceNormal)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-) ret=false\n";
        return false;
    }

    auto future = this->mProxy->soaDrvSeatDeviceNormal.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            isDeviceNormal = static_cast<eevp::control::SoaDeviceIsNormal>(result.Value());
            LOG_DEBUG() << "(-) ret=true\n";
            return true;
        }
        else
        {
           LOG_ERROR() << "Return error with \n";// << result.Error().Message();
        }
    }
    else
    {
       LOG_ERROR() << "Timeout to call soaRctnStatus's Getter\n";//
    }

    LOG_DEBUG() << "(-) ret=false\n";
    return false;
}

void DriverSeatProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaDriverSeatProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle)
{
    LOG_DEBUG() << "(+)\n";

    if (container.empty())
    {
        this->mProxy = nullptr;
        LOG_ERROR() << "Container empty...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
    this->mProxy = std::make_shared<proxy::SoaDriverSeatProxy>(container.at(0));


    SubscribeEvent();
    SubscribeField();

    LOG_DEBUG() << "(-)\n";
}

void DriverSeatProxyImpl::SubscribeEvent()
{
    LOG_DEBUG() << "(+)\n";
    // not used
    LOG_DEBUG() << "(-)\n";
}

void DriverSeatProxyImpl::SubscribeField()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    if (this->mProxy->soaDrvSeatDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaDrvSeatDeviceNormal subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->soaDrvSeatDeviceNormal.SetReceiveHandler(std::bind(&DriverSeatProxyImpl::cbIsDeviceNormal, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for cbIsDeviceNormal with \n";// << result.Error().Message();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaDrvSeatDeviceNormal.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaDrvSeatDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mProxy->soaDrvSeatDeviceNormal.Unsubscribe();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));



        }
#else
        mProxy->soaDrvSeatDeviceNormal.Subscribe(1);
#endif

    if (!result.HasValue())
    {
        LOG_WARNING() << "Failed to Subscribe for isDeviceNormal with \n";// << result.Error().Message();
    }
    }

    if (this->mProxy->soaDrvSeatMotorDirection.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaDrvSeatMotorDirection subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->soaDrvSeatMotorDirection.SetReceiveHandler(std::bind(&DriverSeatProxyImpl::cbDrvSeatMotorDirection, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for cbDrvSeatMotorDirection with \n";// << result.Error().Message();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaDrvSeatMotorDirection.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaDrvSeatMotorDirection.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mProxy->soaDrvSeatMotorDirection.Unsubscribe();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#else
        mProxy->soaDrvSeatMotorDirection.Subscribe(1);
#endif
    }

    if (this->mProxy->soaDrvSeatMotorPosition.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaDrvSeatMotorPosition subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->soaDrvSeatMotorPosition.SetReceiveHandler(std::bind(&DriverSeatProxyImpl::cbDrvSeatMotorPosition, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for cbDrvSeatMotorPosition with \n";// << result.Error().Message();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaDrvSeatMotorPosition.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaDrvSeatMotorPosition.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }
        }
#else
        mProxy->soaDrvSeatMotorPosition.Subscribe(1);
#endif
    }

    LOG_DEBUG() << "(-)\n";
}

void DriverSeatProxyImpl::cbIsDeviceNormal()
{
    LOG_DEBUG() << "(+)\n";

    eevp::control::SoaDeviceIsNormal fieldValue;

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
    }

    if (this->mProxy->soaDrvSeatDeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "isDeviceNormal not subscribed...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaDrvSeatDeviceNormal.GetNewSamples([&](auto msg){
        fieldValue = static_cast<eevp::control::SoaDeviceIsNormal>(*msg);   // fieldValue = *msg
        // LOG_DEBUG() << "cbSoaRctnStatus : " << fieldValue;
        if (this->mListener != nullptr)
        {
            this->mListener->notifyIsDeviceNormal(fieldValue);
        }
    });

    LOG_DEBUG() << "(-)\n";
}

void DriverSeatProxyImpl::cbDrvSeatMotorDirection()
{
    LOG_DEBUG() << "(+)\n";

    eevp::control::SoaSeatMotorDirection fieldValue;

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
    }

    if (this->mProxy->soaDrvSeatMotorDirection.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaDrvSeatMotorDirection not subscribed...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaDrvSeatMotorDirection.GetNewSamples([&](auto msg){
        fieldValue = static_cast<eevp::control::SoaSeatMotorDirection>(*msg);   // fieldValue = *msg
        //LOG_DEBUG() << "soaDrvSeatMotorDirection : " << fieldValue;
        if (this->mListener != nullptr)
        {
            //this->mListener->notifyIsDeviceNormal(fieldValue);
        }
    });

    LOG_DEBUG() << "(-)\n";
}

void DriverSeatProxyImpl::cbDrvSeatMotorPosition()
{
    LOG_DEBUG() << "(+)\n";

    eevp::control::SoaSeatMotorPosition fieldValue;

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
    }

    if (this->mProxy->soaDrvSeatMotorPosition.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaDrvSeatMotorPosition not subscribed...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaDrvSeatMotorPosition.GetNewSamples([&](auto msg){
        fieldValue = static_cast<eevp::control::SoaSeatMotorPosition>(*msg);   // fieldValue = *msg
        //LOG_DEBUG() << "soaDrvSeatDeviceNormal : " << fieldValue;
        if (this->mListener != nullptr)
        {
            //this->mListener->notifyIsDeviceNormal(fieldValue);
        }
    });

    LOG_DEBUG() << "(-)\n";
}

void DriverSeatProxyImpl::RequestMoveDrvSeatHeight(SoaSeatHeightDirection dir)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy)
    {
        this->mProxy->RequestMoveDrvSeatHeight(dir);
    }

    LOG_DEBUG() << "(-)\n";
}

void DriverSeatProxyImpl::RequestMoveDrvSeatRecline(SoaSeatReclineDirection dir)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy)
    {
        this->mProxy->RequestMoveDrvSeatRecline(dir);
    }

    LOG_DEBUG() << "(-)\n";
}

void DriverSeatProxyImpl::RequestMoveDrvSeatSlide(SoaSeatSlideDirection dir)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy)
    {
        this->mProxy->RequestMoveDrvSeatSlide(dir);
    }

    LOG_DEBUG() << "(-)\n";
}

void DriverSeatProxyImpl::RequestMoveDrvSeatTilt(SoaSeatTiltDirection dir)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy)
    {
        this->mProxy->RequestMoveDrvSeatTilt(dir);
    }

    LOG_DEBUG() << "(-)\n";
}

#include <sstream>
std::string DriverSeatProxyImpl::GetSubscriptStatusString()
{
    std::stringstream ss;

    ss << "soaDrvSeatDeviceNormal: ";
    ss << ((this->mProxy->soaDrvSeatDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    ss << "soaDrvSeatMotorDirection: ";
    ss << ((this->mProxy->soaDrvSeatMotorDirection.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    ss << "soaDrvSeatMotorPosition: ";
    ss << ((this->mProxy->soaDrvSeatMotorPosition.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    return ss.str();
}

} /// namespace driverseat
} /// namespace control
} /// namespace eevp
