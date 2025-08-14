#include <Common.h>
#include "MoodLampProxyImpl.h"
#include "Log.hpp"

#define MAX_SERVICE_SEARCH_TRIAL    50 // 100ms * 50 = 5sec

using namespace ara::core;
using namespace eevp::control;
using namespace eevp::control::moodlamp;

namespace eevp {
namespace control {
namespace moodlamp {

MoodLampProxyImpl *MoodLampProxyImpl::s_mInst = nullptr;

MoodLampProxyImpl *MoodLampProxyImpl::GetInstance()
{
    if (MoodLampProxyImpl::s_mInst == nullptr)
    {
        MoodLampProxyImpl::s_mInst = new MoodLampProxyImpl();
    }

    return MoodLampProxyImpl::s_mInst;

}

void MoodLampProxyImpl::DeleteInstance()
{
    if (MoodLampProxyImpl::s_mInst != nullptr)
    {
        delete MoodLampProxyImpl::s_mInst;
        MoodLampProxyImpl::s_mInst = nullptr;
    }
}


MoodLampProxyImpl::MoodLampProxyImpl()
{
    this->mProxy = nullptr;
    this->mFindHandle = nullptr;
    this->mListener = nullptr;
}

MoodLampProxyImpl::~MoodLampProxyImpl()
{
    if (this->mProxy)
    {
        this->mProxy->StopFindService(*this->mFindHandle);
        this->mProxy.reset();
    }
}

void MoodLampProxyImpl::setEventListener(std::shared_ptr<IMoodLampListener> _listener)
{
    this->mListener = _listener;
}

bool MoodLampProxyImpl::init()
{
    LOG_DEBUG() << "(+)\n";

    ara::core::InstanceSpecifier specifier("SOA_Debugger/AA/RPort_SOADBG_MoodLamp");

    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    auto result = proxy::SoaMlmProxy::StartFindService(callback, specifier);

    if (!result.HasValue())
    {
        LOG_INFO() << "SoaMlmProxy StartFindService() Failed\n";
        return false;
    }

    for (int i = 0; i < 50; i++)
    {
        if ((this->mFindHandle) && (this->mProxy))
        {
            LOG_INFO() << "SoaMlmProxy Start Find Service Success\n";
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    LOG_INFO() << "(-)\n";
    return false;
}

void MoodLampProxyImpl::requestMlmSetRGBColor(const std::uint8_t& colorTableIndex)
{
    LOG_DEBUG() << "(+)\n";
                //return false;
    if (this->mProxy == nullptr)
            {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
        return;
            }

    this->mProxy->RequestMlmSetRgbColor(colorTableIndex);

    LOG_DEBUG() << "(-)\n";
}

void MoodLampProxyImpl::requestMlmSetMode(const eevp::control::SoaMlmMoodMode& mood)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->RequestMlmSetMode(mood);

    LOG_DEBUG() << "(-)\n";
}

void MoodLampProxyImpl::requestMlmSetBrightness(const std::int16_t& brightness)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->RequestMlmSetBrightness(brightness);

    LOG_DEBUG() << "(-)\n";
}

bool MoodLampProxyImpl::getterSoaMlmStatus(eevp::control::SoaMlmStatus& soaMlmStatus)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-) ret=false\n";
        return false;
    }

    auto future = this->mProxy->soaMlmStatus.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            soaMlmStatus = static_cast<eevp::control::SoaMlmStatus>(result.Value());
            LOG_DEBUG() << "(-) ret=true\n";
            return true;
        }
        else
        {
            LOG_ERROR() << "result.HasValue() != true : no value\n";// << result.Error().Message();
        }
    }
    else
    {
        LOG_ERROR() << "Timeout to call soaMlmStatus's Getter";
    }

    LOG_DEBUG() << "(-) ret=false\n";
    return false;
}

void MoodLampProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaMlmProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle)
{
    LOG_DEBUG() << "(+)\n";

    if (container.empty())
    {
        LOG_ERROR() << "container is empty\n";
        LOG_DEBUG() << "(-)\n";
        this->mProxy = nullptr;
        return;
    }

    this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
    this->mProxy = std::make_shared<proxy::SoaMlmProxy>(container.at(0));

    SubscribeEvent();
    SubscribeField();

    LOG_DEBUG() << "(-)\n";
}

void MoodLampProxyImpl::SubscribeEvent()
{
    LOG_DEBUG() << "(+)\n";
    // not used
    LOG_DEBUG() << "(-)\n";
}

void MoodLampProxyImpl::SubscribeField()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    if (this->mProxy->soaMlmStatus.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaMlmStatus is subscribed.\n";
    }
    else
    {
        auto result = this->mProxy->soaMlmStatus.SetReceiveHandler(std::bind(&MoodLampProxyImpl::cbSoaMlmStatus, this));
        if (!result.HasValue())
        {
            LOG_ERROR() << "Failed to SetReceiveHandler for cbSoaMlmStatus.\n" ;//<< result.Error().Message().string();
        }

#if defined(HOTFIX_SUBSCRIPTION)
        while (true)
        {
            mProxy->soaMlmStatus.Subscribe(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (this->mProxy->soaMlmStatus.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
            {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mProxy->soaMlmStatus.Unsubscribe();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#else
        mProxy->soaMlmStatus.Subscribe(1);
#endif
    }

    LOG_DEBUG() << "(-)\n";
}

void MoodLampProxyImpl::cbSoaMlmStatus()
{
    LOG_DEBUG() << "(+)\n";

    eevp::control::SoaMlmStatus fieldValue;

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    if (this->mProxy->soaMlmStatus.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaMlmStatus is not subscribed.\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaMlmStatus.GetNewSamples([&](auto msg) {
        fieldValue = static_cast<eevp::control::SoaMlmStatus>(*msg);    // fieldValue = *msg;

        if (this->mListener != nullptr)
        {
            this->mListener->notifySoaMlmStatus(fieldValue);
        }

    });
}

std::string MoodLampProxyImpl::GetSubscriptStatusString()
{
    std::stringstream ss;

    ss << "soaMlmStatus: ";
    ss << ((this->mProxy->soaMlmStatus.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    return ss.str();
}

} /// namespace moodlamp
} /// namespace control
} /// namespace eevp
