#include <Common.h>
#include "PowerProxyImpl.h"
#include "Log.hpp"
#include <source_location>

#define MAX_SERVICE_SEARCH_TRIAL    50 // 100ms * 50 = 5sec

using namespace ara::core;
using namespace eevp::control;
using namespace eevp::control::power;

namespace eevp {
namespace control {
namespace power {

PowerProxyImpl *PowerProxyImpl::s_mInst = nullptr;

PowerProxyImpl *PowerProxyImpl::GetInstance()
{
    if (PowerProxyImpl::s_mInst == nullptr)
    {
        PowerProxyImpl::s_mInst = new PowerProxyImpl();
    }

    return PowerProxyImpl::s_mInst;
}

void PowerProxyImpl::DeleteInstance()
{
    if (PowerProxyImpl::s_mInst != nullptr)
    {
        delete PowerProxyImpl::s_mInst;
        PowerProxyImpl::s_mInst = nullptr;
    }
}

PowerProxyImpl::PowerProxyImpl()
{
    this->mProxy = nullptr;
    this->mFindHandle = nullptr;
    this->mListener = nullptr;
}

PowerProxyImpl::~PowerProxyImpl()
{
    if (this->mProxy)
    {
        mProxy->StopFindService(*(this->mFindHandle));
        mProxy.reset();
    }
}

void PowerProxyImpl::setEventListener(std::shared_ptr<IPowerListener> _listener)
{
    this->mListener = _listener;
}

bool PowerProxyImpl::init()
{
    LOG_DEBUG() << "(+)\n";

    ara::core::InstanceSpecifier specifier("SOA_Debugger/AA/RPort_SOADBG_Power");

    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    auto result = proxy::SoaPowerProxy::StartFindService(callback, specifier);
    if (!result.HasValue())
    {
        LOG_INFO() << "SoaPowerProxy StartFindService() Failed\n";
        return false;
    }

    for (int i = 0; i < 50; i++)
    {
        if ((this->mFindHandle) && (this->mProxy))
        {
            LOG_INFO() << "SoaPowerProxy Start Find Service Success\n";
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    LOG_INFO() << "(-)\n";
    return false;
}

bool PowerProxyImpl::getterSoaPowerDeviceNormal(eevp::control::SoaDeviceIsNormal& isNormal)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-) ret=false\n";
        return false;
    }

    auto future = this->mProxy->soaPowerDeviceNormal.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            isNormal = static_cast<eevp::control::SoaDeviceIsNormal>(result.Value());
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

void PowerProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaPowerProxy::HandleType> container,
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
    this->mProxy = std::make_shared<proxy::SoaPowerProxy>(container.at(0));

    SubscribeField();

    LOG_DEBUG() << "(-)\n";
}

void PowerProxyImpl::SubscribeEvent()
{
    LOG_DEBUG() << "(+)\n";
    // not used
    LOG_DEBUG() << "(-)\n";
}

void PowerProxyImpl::SubscribeField()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    if (this->mProxy->soaPowerDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaPowerDeviceNormal subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->soaPowerDeviceNormal.SetReceiveHandler(std::bind(&PowerProxyImpl::cbFieldDeviceNormal, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for cbFieldDeviceNormal with \n";// << result.Error().Message();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaPowerDeviceNormal.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaPowerDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mProxy->soaPowerDeviceNormal.Unsubscribe();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#else
        mProxy->soaPowerDeviceNormal.Subscribe(1);
#endif
    }

    LOG_DEBUG() << "(-)\n";
}

void PowerProxyImpl::cbFieldDeviceNormal()
{
    LOG_DEBUG() << "(+)\n";

    eevp::control::SoaDeviceIsNormal fieldValue;

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
    }

    if (this->mProxy->soaPowerDeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "isDeviceNormal not subscribed...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaPowerDeviceNormal.GetNewSamples([&](auto msg){
        fieldValue = static_cast<eevp::control::SoaDeviceIsNormal>(*msg);   // fieldValue = *msg
        // LOG_DEBUG() << "cbSoaRctnStatus : " << fieldValue;
        if (this->mListener != nullptr)
        {
            this->mListener->notifyDeviceNormal(fieldValue);
        }
    });

    LOG_DEBUG() << "(-)\n";
}

std::string PowerProxyImpl::GetSubscriptStatusString()
{
    std::stringstream ss;

    ss << "soaPowerDeviceNormal: ";
    ss << ((this->mProxy->soaPowerDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    return ss.str();
}


} /// namespace driverseat
} /// namespace control
} /// namespace eevp
