#include <Common.h>
#include "RearCurtainProxyImpl.h"
#include "Log.hpp"

using namespace ara::core;
using namespace eevp::control;
using namespace eevp::control::rearcurtain;

namespace eevp {
namespace control {
namespace rearcurtain {

RearCurtainProxyImpl *RearCurtainProxyImpl::s_mInst = nullptr;

RearCurtainProxyImpl *RearCurtainProxyImpl::GetInstance()
{
    if (RearCurtainProxyImpl::s_mInst == nullptr)
    {
        RearCurtainProxyImpl::s_mInst = new RearCurtainProxyImpl();
    }

    return RearCurtainProxyImpl::s_mInst;
}

void RearCurtainProxyImpl::DeleteInstance()
{
    if (s_mInst != nullptr)
    {
        delete s_mInst;
        s_mInst = nullptr;
    }
}

RearCurtainProxyImpl::RearCurtainProxyImpl()
{
    this->mProxy = nullptr;
    this->mFindHandle = nullptr;
    this->listener = nullptr;
}

RearCurtainProxyImpl::~RearCurtainProxyImpl()
{
    if (mProxy) {
        mProxy->StopFindService(*mFindHandle);
        mProxy.reset();
    }
}

void
RearCurtainProxyImpl::setEventListener(std::shared_ptr<IRearCurtainListener> _listener) {
    listener = _listener;
}

bool
RearCurtainProxyImpl::init() {
    LOG_INFO() << "(+)\n";

    ara::core::InstanceSpecifier specifier("SOA_Debugger/AA/RPort_SOADBG_RearCurtain");

    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    auto result = proxy::SoaRcurtainProxy::StartFindService(callback, specifier);

    if (!result.HasValue())
    {
        LOG_INFO() << "SoaRcurtainProxy StartFindService() Failed\n";
        return false;
    }

    for (int i = 0; i < 50; i++)
    {
        if (this->mFindHandle)
        {
            LOG_INFO() << "SoaRcurtainProxy Start Find Service Success\n";
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    LOG_INFO() << "(-)\n";
    return false;
}

eevp::control::SoaErrorState
RearCurtainProxyImpl::requestRearCurtainOperation(const eevp::control::SoaRctnMotorDir& motorDir) {
    LOG_INFO() << "(+)\n";

    if (mProxy == nullptr) {
        LOG_INFO() << "(-)\n";
        return eevp::control::SoaErrorState::kERROR;
    }

    auto future = mProxy->RequestRearCurtainOperation(motorDir);
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready) {
        auto result = future.GetResult();
        if (result.HasValue()) {
            LOG_INFO() << "(-)\n";
            auto value = result.Value();
            return value.err;
        } else {
           LOG_ERROR() << "Return error with \n";// << result.Error().Message();
        }
    } else {
       LOG_ERROR() << "Timeout to call RequestRearCurtainOperation\n";
    }


    LOG_INFO() << "(-)\n";
    return eevp::control::SoaErrorState::kOK; // eevp::control::SoaErrorState::kERROR;

}

bool
RearCurtainProxyImpl::getterSoaRctnStatus(eevp::control::SoaRctnStatus& soaRctnStatus) {
    LOG_INFO() << "(+)\n";

    if (mProxy == nullptr) {
        return false;
    }

    auto future = mProxy->soaRctnStatus.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready) {
        auto result = future.GetResult();
        if (result.HasValue()) {
            soaRctnStatus = static_cast<eevp::control::SoaRctnStatus>(result.Value());
            return true;
        } else {
           LOG_ERROR() << "Return error with\n";// << result.Error().Message();
        }
    } else {
       LOG_ERROR() << "Timeout to call soaRctnStatus's Getter\n";//
    }

    LOG_INFO() << "(-)\n";
    return false;
}

void
RearCurtainProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaRcurtainProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle) {
    LOG_INFO() << "(+)\n";

    if (container.empty()) {
        mProxy = nullptr;
        return;
    }

    mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
    mProxy = std::make_shared<proxy::SoaRcurtainProxy>(container.at(0));

    SubscribeField();
    LOG_INFO() << "(-)\n";
}

void
RearCurtainProxyImpl::SubscribeEvent() {
    LOG_INFO() << "(+)\n";
    // not used
    LOG_INFO() << "(-)\n";
}

void
RearCurtainProxyImpl::SubscribeField() {
    LOG_INFO() << "(+)\n";

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->soaRctnStatus.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaPowerDeviceNormal subscribed...\n";
    }
    else
    {
        auto result = mProxy->soaRctnStatus.SetReceiveHandler(std::bind(&RearCurtainProxyImpl::cbSoaRctnStatus, this));
        if (!result.HasValue()) {
            LOG_WARNING() << "Failed to SetReceiveHandler for cbSoaRctnStatus with \n";// << result.Error().Message();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaRctnStatus.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaRctnStatus.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mProxy->soaRctnStatus.Unsubscribe();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#else
        mProxy->soaRctnStatus.Subscribe(1);
#endif
    }

    LOG_INFO() << "(-)\n";
}

void
RearCurtainProxyImpl::cbSoaRctnStatus() {
    LOG_INFO() << "(+)\n";

    eevp::control::SoaRctnStatus fieldValue;

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->soaRctnStatus.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed) {
        return;
    }

    mProxy->soaRctnStatus.GetNewSamples([&](auto msg){
        fieldValue = static_cast<eevp::control::SoaRctnStatus>(*msg);   // fieldValue = *msg
        // LOG_INFO() << "cbSoaRctnStatus : " << fieldValue;
        if (listener != nullptr) {
            listener->notifySoaRctnStatus(fieldValue);
        }
    });

    LOG_INFO() << "(-)\n";
}

std::string RearCurtainProxyImpl::GetSubscriptStatusString()
{
    std::stringstream ss;

    ss << "soaRctnStatus: ";
    ss << ((this->mProxy->soaRctnStatus.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    return ss.str();
}

} /// namespace rearcurtain
} /// namespace control
} /// namespace eevp
