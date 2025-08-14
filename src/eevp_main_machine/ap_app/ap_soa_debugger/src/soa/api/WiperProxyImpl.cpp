#include <Common.h>
#include "WiperProxyImpl.h"
#include "Log.hpp"

#define MAX_SERVICE_SEARCH_TRIAL    50 // 100ms * 50 = 5sec

using namespace ara::core;
using namespace eevp::control;
using namespace eevp::control::wiper;

namespace eevp {
namespace control {
namespace wiper {

WiperProxyImpl *WiperProxyImpl::s_mInst = nullptr;

WiperProxyImpl *WiperProxyImpl::GetInstance()
{
    if (WiperProxyImpl::s_mInst == nullptr)
    {
        WiperProxyImpl::s_mInst = new WiperProxyImpl();
    }

    return WiperProxyImpl::s_mInst;
}

void WiperProxyImpl::DeleteInstance()
{
    if (s_mInst != nullptr)
    {
        delete s_mInst;
        s_mInst = nullptr;
    }
}

WiperProxyImpl::WiperProxyImpl()
{
    this->mProxy = nullptr;
    this->mFindHandle = nullptr;
    this->mListener = nullptr;
}

WiperProxyImpl::~WiperProxyImpl()
{
    if (this->mProxy) {
        this->mProxy->StopFindService(*mFindHandle);
        this->mProxy.reset();
    }
}

void WiperProxyImpl::setEventListener(std::shared_ptr<IWiperListener> _listener) {
    this->mListener = _listener;
}

bool WiperProxyImpl::init() {
    LOG_DEBUG() << "(+)\n";

    ara::core::InstanceSpecifier specifier("SOA_Debugger/AA/RPort_SOADBG_Wiper");

    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    auto result = proxy::SoaWiperProxy::StartFindService(callback, specifier);

    if (!result.HasValue())
    {
        LOG_INFO() << "SoaWiperProxy StartFindService() Failed\n";
        return false;
    }

    for (int i = 0; i < 50; i++)
    {
        if ((this->mFindHandle) && (this->mProxy))
        {
            LOG_INFO() << "SoaWiperProxy Start Find Service Success\n";
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    LOG_INFO() << "(-)\n";
    return false;
}

void WiperProxyImpl::RequestWiperOperation(eevp::control::SoaWiperMode wiperMode)
{
    LOG_INFO() << "(+)\n";
    if (this->mProxy != nullptr)
    {
        this->mProxy->RequestWiperOperation(wiperMode);
    }
    // not used
    LOG_INFO() << "(-)\n";
}

bool WiperProxyImpl::getterIsDeviceNormal(eevp::control::SoaDeviceIsNormal& isDeviceNormal)
{
    LOG_INFO() << "(+)\n";

    if (mProxy == nullptr)
    {
        return false;
    }

    auto future = mProxy->soaWiperDeviceNormal.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            LOG_INFO() << "(-)\n";
            isDeviceNormal = static_cast<eevp::control::SoaDeviceIsNormal>(result.Value());
            return true;
        }
        else
        {
           LOG_ERROR() << "Return error with \n";// << result.Error().Message();
        }
    }
    else
    {
        LOG_ERROR() << "Timeout to call isDeviceNormal Getter\n";
    }

    LOG_INFO() << "(-)\n";
    return false;
}

void WiperProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaWiperProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle)
{
    LOG_INFO() << "(+)\n";

    if (container.empty()) {
        mProxy = nullptr;
        return;
    }

    mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
    mProxy = std::make_shared<proxy::SoaWiperProxy>(container.at(0));

    SubscribeEvent();
    SubscribeField();

    LOG_INFO() << "(-)\n";
}

void WiperProxyImpl::SubscribeEvent()
{
    LOG_INFO() << "(+)\n";
    // not used
    LOG_INFO() << "(-)\n";
}

void WiperProxyImpl::SubscribeField()
{
    LOG_INFO() << "(+)\n";

    if (mProxy == nullptr)
    {
        LOG_INFO() << "(-)\n";
        return;
    }

    if (mProxy->soaWiperDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaWiperDeviceNormal subscribed...\n";
    }
    else
    {
        auto result = mProxy->soaWiperDeviceNormal.SetReceiveHandler(std::bind(&WiperProxyImpl::cbIsDeviceNormal, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for cbIsDeviceNormal with \n";// << result.Error().Message();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaWiperDeviceNormal.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaWiperDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mProxy->soaWiperDeviceNormal.Unsubscribe();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#else
        mProxy->soaWiperDeviceNormal.Subscribe(1);
#endif
    }

    if (mProxy->soaWiperStatus.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaWiperStatus subscribed...\n";
    }
    else
    {
        auto result = mProxy->soaWiperStatus.SetReceiveHandler(std::bind(&WiperProxyImpl::cbIsDeviceNormal, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for cbIsDeviceNormal with \n";// << result.Error().Message();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaWiperStatus.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaWiperStatus.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mProxy->soaWiperStatus.Unsubscribe();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#else
        mProxy->soaWiperStatus.Subscribe(1);
#endif
    }

    LOG_INFO() << "(-)\n";
}

void WiperProxyImpl::cbIsDeviceNormal() {
    LOG_INFO() << "(+)\n";

    eevp::control::SoaDeviceIsNormal fieldValue;

    if (this->mProxy == nullptr)
    {
        LOG_INFO() << "(-)\n";
        return;
    }

    if (this->mProxy->soaWiperDeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_INFO() << "(-)\n";
        return;
    }

    this->mProxy->soaWiperDeviceNormal.GetNewSamples([&](auto msg) {
        fieldValue = static_cast<eevp::control::SoaDeviceIsNormal>(*msg);   // fieldValue = *msg
        // LOG_INFO() << "cbSoaRctnStatus : " << fieldValue;
        if (this->mListener != nullptr) {
            this->mListener->notifyIsDeviceNormal(fieldValue);
        }
    });

    LOG_INFO() << "(-)\n";
}

std::string WiperProxyImpl::GetSubscriptStatusString()
{
    std::stringstream ss;

    ss << "soaWiperDeviceNormal: ";
    ss << ((this->mProxy->soaWiperDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    ss << "soaWiperStatus: ";
    ss << ((this->mProxy->soaWiperStatus.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    return ss.str();
}

} /// namespace wiper
} /// namespace control
} /// namespace eevp
