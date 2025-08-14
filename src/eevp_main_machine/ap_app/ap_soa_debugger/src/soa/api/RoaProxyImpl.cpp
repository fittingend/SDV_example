#include <Common.h>
#include "RoaProxyImpl.h"
#include "Log.hpp"

using namespace ara::core;
using namespace eevp::control;
using namespace eevp::control::roa;

namespace eevp {
namespace control {
namespace roa {

RoaProxyImpl *RoaProxyImpl::s_mInst = nullptr;

RoaProxyImpl *RoaProxyImpl::GetInstance()
{
    if (RoaProxyImpl::s_mInst == nullptr)
    {
        RoaProxyImpl::s_mInst = new RoaProxyImpl();
    }

    return RoaProxyImpl::s_mInst;
}

void RoaProxyImpl::DeleteInstance()
{
    if (s_mInst != nullptr)
    {
        delete s_mInst;
        s_mInst = nullptr;
    }
}

RoaProxyImpl::RoaProxyImpl()
{
    this->mProxy = nullptr;
    this->mFindHandle = nullptr;
    this->listener = nullptr;
}

RoaProxyImpl::~RoaProxyImpl()
{
    if (this->mProxy) {
        this->mProxy->StopFindService(*mFindHandle);
        this->mProxy.reset();
    }
}

void RoaProxyImpl::setEventListener(std::shared_ptr<IRoaListener> _listener) {
    this->listener = _listener;
}

bool RoaProxyImpl::init() {
    LOG_INFO() << "(+)\n";

    ara::core::InstanceSpecifier specifier("SOA_Debugger/AA/RPort_SOADBG_ROA");

    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    auto result = proxy::SoaRoaProxy::StartFindService(callback, specifier);

    if (!result.HasValue())
    {
        LOG_INFO() << "SoaRoaProxy StartFindService() Failed\n";
        return false;
    }

    for (int i = 0; i < 50; i++)
    {
        if (this->mFindHandle)
        {
            LOG_INFO() << "SoaRoaProxy Start Find Service Success\n";
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    LOG_INFO() << "(-)\n";
    return false;
}

bool RoaProxyImpl::StartRoa()
{
    if (this->mProxy == nullptr)
    {
        return false;
    }

    this->mProxy->StartRoa();
    return true;
}

bool RoaProxyImpl::StopRoa()
{
    if (this->mProxy == nullptr)
    {
        return false;
    }

    this->mProxy->StopRoa();
    return true;
}

bool RoaProxyImpl::ResetRoaDetectInfo()
{
    if (this->mProxy == nullptr)
    {
        return false;
    }

    this->mProxy->ResetRoaDetectInfo();
    return true;
}

bool RoaProxyImpl::getterIsDeviceNormal(eevp::control::SoaDeviceIsNormal& isDeviceNormal)
{
    LOG_INFO() << "(+)\n";

    if (mProxy == nullptr)
    {
        LOG_INFO() << "(-)\n";
        return false;
    }

    auto future = mProxy->soaRoaDeviceNormal.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            isDeviceNormal = static_cast<eevp::control::SoaDeviceIsNormal>(result.Value());
            LOG_INFO() << "(-)\n";
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

void RoaProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaRoaProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle)
{
    LOG_INFO() << "(+)\n";

    if (container.empty()) {
        mProxy = nullptr;
        return;
    }

    mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
    mProxy = std::make_shared<proxy::SoaRoaProxy>(container.at(0));

    SubscribeField();
    LOG_INFO() << "(-)\n";
}

void RoaProxyImpl::SubscribeEvent()
{
    LOG_INFO() << "(+)\n";
    // not used
    LOG_INFO() << "(-)\n";
}

void RoaProxyImpl::SubscribeField()
{
     LOG_INFO() << "(+)\n";

    if (mProxy == nullptr)
    {
        return;
    }

    // soaRoaDeviceNormal
    if (mProxy->soaRoaDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaRoaDeviceNormal subscribed...\n";
    }
    else
    {
        auto result = mProxy->soaRoaDeviceNormal.SetReceiveHandler(std::bind(&RoaProxyImpl::cbFieldDeviceNormal, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for cbIsDeviceNormal with \n";// << result.Error().Message();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaRoaDeviceNormal.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaRoaDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mProxy->soaRoaDeviceNormal.Unsubscribe();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#else
        mProxy->soaRoaDeviceNormal.Subscribe(1);
#endif
    }

    // soaRoaSwVersion
    if (mProxy->soaRoaSwVersion.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaRoaSwVersion subscribed...\n";
    }
    else
    {
        auto result = mProxy->soaRoaSwVersion.SetReceiveHandler(std::bind(&RoaProxyImpl::cbFieldSwVersion, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for cbIsDeviceNormal with \n";// << result.Error().Message();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaRoaSwVersion.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaRoaSwVersion.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mProxy->soaRoaSwVersion.Unsubscribe();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#else
        mProxy->soaRoaSwVersion.Subscribe(1);
#endif
    }

    // soaRoaDetectCount
    if (mProxy->soaRoaDetectCount.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaRoaDetectCount subscribed...\n";
    }
    else
    {
        auto result = mProxy->soaRoaDetectCount.SetReceiveHandler(std::bind(&RoaProxyImpl::cbFieldDetectCount, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for soaRoaDetectCount with \n";// << result.Error().Message();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaRoaDetectCount.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaRoaDetectCount.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mProxy->soaRoaDetectCount.Unsubscribe();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#else
        mProxy->soaRoaDetectCount.Subscribe(1);
#endif
    }

    // soaRoaDetectState
    if (mProxy->soaRoaDetectState.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaRoaDetectState subscribed...\n";
    }
    else
    {
        auto result = mProxy->soaRoaDetectState.SetReceiveHandler(std::bind(&RoaProxyImpl::cbFieldDetectState, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for soaRoaDetectCount with \n";// << result.Error().Message();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaRoaDetectState.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaRoaDetectState.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mProxy->soaRoaDetectState.Unsubscribe();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#else
        mProxy->soaRoaDetectState.Subscribe(1);
#endif
    }

    // soaRoaMode
    if (mProxy->soaRoaMode.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaRoaMode subscribed...\n";
    }
    else
    {
        auto result = mProxy->soaRoaMode.SetReceiveHandler(std::bind(&RoaProxyImpl::cbFieldRoaMode, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for soaRoaDetectCount with \n";// << result.Error().Message();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaRoaMode.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaRoaMode.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mProxy->soaRoaMode.Unsubscribe();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#else
        mProxy->soaRoaMode.Subscribe(1);
#endif
    }

    // soaRoaRunningState
    if (mProxy->soaRoaRunningState.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaRoaMode soaRoaRunningState...\n";
    }
    else
    {
        auto result = mProxy->soaRoaRunningState.SetReceiveHandler(std::bind(&RoaProxyImpl::cbFieldRunningState, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for soaRoaDetectCount with \n";// << result.Error().Message();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaRoaRunningState.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaRoaRunningState.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mProxy->soaRoaRunningState.Unsubscribe();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#else
        mProxy->soaRoaRunningState.Subscribe(1);
#endif
    }


    // soaRoaSensorError
    if (mProxy->soaRoaSensorError.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaRoaSensorError soaRoaRunningState...\n";
    }
    else
    {
        auto result = mProxy->soaRoaSensorError.SetReceiveHandler(std::bind(&RoaProxyImpl::cbFieldSensorError, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for soaRoaDetectCount with \n";// << result.Error().Message();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaRoaSensorError.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaRoaSensorError.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mProxy->soaRoaSensorError.Unsubscribe();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#else
        mProxy->soaRoaSensorError.Subscribe(1);
#endif
    }

    LOG_INFO() << "(-)\n";
}

void RoaProxyImpl::cbFieldDeviceNormal() {
    LOG_INFO() << "(+)\n";

    eevp::control::SoaDeviceIsNormal fieldValue;

    if (this->mProxy == nullptr)
    {
        return;
    }

    if (this->mProxy->soaRoaDeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        return;
    }

    this->mProxy->soaRoaDeviceNormal.GetNewSamples([&](auto msg) {
        fieldValue = static_cast<eevp::control::SoaDeviceIsNormal>(*msg);   // fieldValue = *msg
        // LOG_INFO() << "cbSoaRctnStatus : " << fieldValue;
        if (listener != nullptr) {
            this->listener->notifyIsDeviceNormal(fieldValue);
        }
    });

    LOG_INFO() << "(-)\n";
}

void RoaProxyImpl::cbFieldSwVersion()
{
    LOG_INFO() << "(+)\n";

    proxy::fields::soaRoaSwVersion::FieldType fieldValue;

    if (this->mProxy == nullptr)
    {
        return;
    }

    if (this->mProxy->soaRoaSwVersion.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        return;
    }

    this->mProxy->soaRoaSwVersion.GetNewSamples([&](auto msg) {
        fieldValue = static_cast<proxy::fields::soaRoaSwVersion::FieldType>(*msg);   // fieldValue = *msg
        // LOG_INFO() << "cbSoaRctnStatus : " << fieldValue;
    });

    LOG_INFO() << "(-)\n";
}

void RoaProxyImpl::cbFieldDetectCount() // soaRoaDetectCount
{
    LOG_INFO() << "(+)\n";

    proxy::fields::soaRoaDetectCount::FieldType fieldValue;

    if (this->mProxy == nullptr)
    {
        return;
    }

    if (this->mProxy->soaRoaDetectCount.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        return;
    }

    this->mProxy->soaRoaDetectCount.GetNewSamples([&](auto msg) {
        fieldValue = static_cast<proxy::fields::soaRoaDetectCount::FieldType>(*msg);   // fieldValue = *msg
        //LOG_INFO() << "soaRoaDetectCount : " << fieldValue;
    });

    LOG_INFO() << "(-)\n";
}

void RoaProxyImpl::cbFieldDetectState() // soaRoaDetectState
{
    LOG_INFO() << "(+)\n";

    proxy::fields::soaRoaDetectState::FieldType fieldValue;

    if (this->mProxy == nullptr)
    {
        return;
    }

    if (this->mProxy->soaRoaDetectState.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        return;
    }

    this->mProxy->soaRoaDetectState.GetNewSamples([&](auto msg) {
        fieldValue = static_cast<proxy::fields::soaRoaDetectState::FieldType>(*msg);   // fieldValue = *msg
        //LOG_INFO() << "soaRoaDetectState : " << fieldValue;
    });

    LOG_INFO() << "(-)\n";
}

void RoaProxyImpl::cbFieldRoaMode() // soaRoaMode
{
    LOG_INFO() << "(+)\n";

    proxy::fields::soaRoaSwVersion::FieldType fieldValue;

    if (this->mProxy == nullptr)
    {
        return;
    }

    if (this->mProxy->soaRoaSwVersion.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        return;
    }

    this->mProxy->soaRoaSwVersion.GetNewSamples([&](auto msg) {
        fieldValue = static_cast<proxy::fields::soaRoaSwVersion::FieldType>(*msg);   // fieldValue = *msg
        // LOG_INFO() << "cbSoaRctnStatus : " << fieldValue;
    });

    LOG_INFO() << "(-)\n";
}

void RoaProxyImpl::cbFieldRunningState() // soaRoaRunningState
{
    LOG_INFO() << "(+)\n";

    proxy::fields::soaRoaRunningState::FieldType fieldValue;

    if (this->mProxy == nullptr)
    {
        return;
    }

    if (this->mProxy->soaRoaRunningState.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        return;
    }

    this->mProxy->soaRoaRunningState.GetNewSamples([&](auto msg) {
        fieldValue = static_cast<proxy::fields::soaRoaRunningState::FieldType>(*msg);   // fieldValue = *msg
        // LOG_INFO() << "cbSoaRctnStatus : " << fieldValue;
    });

    LOG_INFO() << "(-)\n";
}

void RoaProxyImpl::cbFieldSensorError() // soaRoaSensorError
{
    LOG_INFO() << "(+)\n";

    proxy::fields::soaRoaSensorError::FieldType fieldValue;

    if (this->mProxy == nullptr)
    {
        return;
    }

    if (this->mProxy->soaRoaSensorError.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        return;
    }

    this->mProxy->soaRoaSensorError.GetNewSamples([&](auto msg) {
        fieldValue = static_cast<proxy::fields::soaRoaSensorError::FieldType>(*msg);   // fieldValue = *msg
        // LOG_INFO() << "cbSoaRctnStatus : " << fieldValue;
    });

    LOG_INFO() << "(-)\n";
}

std::string RoaProxyImpl::GetSubscriptStatusString()
{
    std::stringstream ss;

    ss << "soaRoaDeviceNormal: ";
    ss << ((this->mProxy->soaRoaDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    ss << "soaRoaDetectCount: ";
    ss << ((this->mProxy->soaRoaDetectCount.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    ss << "soaRoaMode: ";
    ss << ((this->mProxy->soaRoaMode.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    ss << "soaRoaRunningState: ";
    ss << ((this->mProxy->soaRoaRunningState.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    ss << "soaRoaSensorError: ";
    ss << ((this->mProxy->soaRoaSensorError.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    return ss.str();
}

} /// namespace Roa
} /// namespace control
} /// namespace eevp
