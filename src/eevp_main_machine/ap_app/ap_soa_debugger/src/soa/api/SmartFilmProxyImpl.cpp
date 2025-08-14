#include <Common.h>
#include "SmartFilmProxyImpl.h"
#include "Log.hpp"
#include <CommonEnumConverter.h>
#include <SmartFilmDataTypeConverter.h>
#include <Debug_DataType2String.hpp>

#define MAX_SERVICE_SEARCH_TRIAL    50 // 100ms * 50 = 5sec

using namespace ara::core;
using namespace eevp::control;
using namespace eevp::control::smartfilm;

namespace eevp {
namespace control {
namespace smartfilm {

SmartFilmProxyImpl *SmartFilmProxyImpl::s_mInst = nullptr;

SmartFilmProxyImpl *SmartFilmProxyImpl::GetInstance()
{
    if (SmartFilmProxyImpl::s_mInst == nullptr)
    {
        SmartFilmProxyImpl::s_mInst = new SmartFilmProxyImpl();
    }

    return SmartFilmProxyImpl::s_mInst;
}

void SmartFilmProxyImpl::DeleteInstance()
{
    if (SmartFilmProxyImpl::s_mInst != nullptr)
    {
        delete SmartFilmProxyImpl::s_mInst;
        SmartFilmProxyImpl::s_mInst = nullptr;
    }
}

SmartFilmProxyImpl::SmartFilmProxyImpl()
{
    this->mProxy = nullptr;
    this->mFindHandle = nullptr;
    this->mListener = nullptr;
}

SmartFilmProxyImpl::~SmartFilmProxyImpl()
{
    if (this->mProxy)
    {
        mProxy->StopFindService(*(this->mFindHandle));
        mProxy.reset();
    }
}

void SmartFilmProxyImpl::setEventListener(std::shared_ptr<ISmartFilmListener> _listener)
{
    this->mListener = _listener;
}

bool SmartFilmProxyImpl::init()
{
    LOG_DEBUG() << "(+)\n";

    ara::core::InstanceSpecifier specifier("SOA_Debugger/AA/RPort_SOADBG_SmartFilm");

    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    auto result = proxy::SoaSmartFilmProxy::StartFindService(callback, specifier);
    if (!result.HasValue())
    {
        LOG_INFO() << "SoaSmartFilmProxy StartFindService() Failed\n";
        return false;
    }

    for (int i = 0; i < 50; i++)
    {
        if ((this->mFindHandle) && (this->mProxy))
        {
            LOG_INFO() << "SoaSmartFilmProxy Start Find Service Success\n";
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    LOG_INFO() << "(-)\n";
    return false;
}

void SmartFilmProxyImpl::SetOpacity(SmtfilmPos_e pos, int opacities)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetOpacity(Convert_SmtfilmPos_SOA2API(pos), (std::uint8_t)opacities);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

void SmartFilmProxyImpl::SetAllOpacities(int opacities)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetAllOpacities((std::uint8_t)opacities);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

void SmartFilmProxyImpl::SetAuto(bool autoMode)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetAuto(autoMode);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

bool SmartFilmProxyImpl::getterIsDeviceNormal(DeviceNormal_e &isDeviceNormal)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-) ret=false\n";
        return false;
    }

    auto future = this->mProxy->soaFilmDeviceNormal.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            isDeviceNormal = Convert_DeviceNormal_API2SOA(static_cast<eevp::control::SoaDeviceIsNormal>(result.Value()));
            LOG_DEBUG() << "(-) ret=true, isDeviceNormal=" << getStringEnum_DeviceNormal_e(isDeviceNormal) << "\n";
            return true;
        }
        else
        {
           LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
        }
    }
    else
    {
       LOG_ERROR() << "Timeout to call soaRctnStatus's Getter\n";//
    }

    LOG_DEBUG() << "(-) ret=false\n";
    return false;
}

bool SmartFilmProxyImpl::getterSmartFilmAutoMode(bool &autoMode)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-) ret=false\n";
        return false;
    }

    auto future = this->mProxy->soaFilmAuto.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            autoMode = static_cast<bool>(result.Value());
            LOG_DEBUG() << "(-) ret=true, autoMode=" << getStringEnum_bool(autoMode) << "\n";
            return true;
        }
        else
        {
           LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
        }
    }
    else
    {
       LOG_ERROR() << "Timeout to call soaFilmAuto Getter\n";//
    }

    LOG_DEBUG() << "(-) ret=false\n";
    return false;
}

bool SmartFilmProxyImpl::getterSmartFilmOpacities(SmtfilmOpacities_t &opacities)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-) ret=false\n";
        return false;
    }

    auto future = this->mProxy->soaFilmOpacities.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            Convert_SmtfilmOpacity_API2SOA(static_cast<eevp::control::SoaFilmOpacityArray>(result.Value()), opacities);
            LOG_DEBUG() << "(-) ret=true, opacities=" << getString_SmtfilmOpacities_t(opacities) << "\n";
            return true;
        }
        else
        {
           LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
        }
    }
    else
    {
       LOG_ERROR() << "Timeout to call soaFilmOpacities Getter\n";//
    }

    LOG_DEBUG() << "(-) ret=false\n";
    return false;
}

bool SmartFilmProxyImpl::getterSmartFilmSwVersion(int &version)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-) ret=false\n";
        return false;
    }

    auto future = this->mProxy->soaFilmSwVersion.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            version = (int)result.Value();
            LOG_DEBUG() << "(-) ret=true, version=" << version << "\n";
            return true;
        }
        else
        {
           LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
        }
    }
    else
    {
       LOG_ERROR() << "Timeout to call soaFilmDeviceNormal Getter\n";//
    }

    LOG_DEBUG() << "(-) ret=false\n";
    return false;
}

void SmartFilmProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaSmartFilmProxy::HandleType> container,
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
    this->mProxy = std::make_shared<proxy::SoaSmartFilmProxy>(container.at(0));

    SubscribeEvent();
    SubscribeField();

    LOG_DEBUG() << "(-)\n";
}

void SmartFilmProxyImpl::SubscribeEvent()
{
    LOG_DEBUG() << "(+)\n";
    // not used
    LOG_DEBUG() << "(-)\n";
}

void SmartFilmProxyImpl::SubscribeField()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    // soaFilmDeviceNormal
    if (this->mProxy->soaFilmDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaFilmDeviceNormal subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->soaFilmDeviceNormal.SetReceiveHandler(std::bind(&SmartFilmProxyImpl::cbIsDeviceNormal, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for soaFilmDeviceNormal with " << result.Error().Message() << "\n";
        }

        mProxy->soaFilmDeviceNormal.Subscribe(1);

        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to Subscribe for soaFilmDeviceNormal with " << result.Error().Message() << "\n";
        }
    }

    // soaFilmAuto
    if (this->mProxy->soaFilmAuto.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaFilmAuto subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->soaFilmAuto.SetReceiveHandler(std::bind(&SmartFilmProxyImpl::cbSmartFilmAutoMode, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for soaFilmAuto with " << result.Error().Message() << "\n";
        }

        mProxy->soaFilmAuto.Subscribe(1);

        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to Subscribe for soaFilmAuto with " << result.Error().Message() << "\n";
        }
    }

    // soaFilmOpacities
    if (this->mProxy->soaFilmOpacities.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaFilmOpacities subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->soaFilmOpacities.SetReceiveHandler(std::bind(&SmartFilmProxyImpl::cbSmartFilmOpacities, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for soaFilmOpacities with " << result.Error().Message() << "\n";
        }

        mProxy->soaFilmOpacities.Subscribe(1);

        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to Subscribe for soaFilmOpacities with " << result.Error().Message() << "\n";
        }
    }

    LOG_DEBUG() << "(-)\n";
}

void SmartFilmProxyImpl::cbIsDeviceNormal()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
    }

    if (this->mProxy->soaFilmDeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaFilmDeviceNormal not subscribed...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaFilmDeviceNormal.GetNewSamples([&](auto msg){
        DeviceNormal_e isDeviceNormal = Convert_DeviceNormal_API2SOA(static_cast<eevp::control::SoaDeviceIsNormal>(*msg));
        LOG_DEBUG() << "soaFilmDeviceNormal notified: value = " << getStringEnum_DeviceNormal_e(isDeviceNormal) << "\n";
        if (this->mListener != nullptr)
        {
            this->mListener->notifyIsDeviceNormal(isDeviceNormal);
        }
    });

    LOG_DEBUG() << "(-)\n";
}

void SmartFilmProxyImpl::cbSmartFilmAutoMode()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
    }

    if (this->mProxy->soaFilmAuto.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaFilmAuto not subscribed...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaFilmAuto.GetNewSamples([&](auto msg){
        bool autoMode = static_cast<bool>(*msg);
        LOG_DEBUG() << "soaFilmAuto notified: value = " << getStringEnum_bool(autoMode) << "\n";
        if (this->mListener != nullptr)
        {
            this->mListener->notifySmartFilmAutoMode(autoMode);
        }
    });

    LOG_DEBUG() << "(-)\n";
}

void SmartFilmProxyImpl::cbSmartFilmOpacities()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
    }

    if (this->mProxy->soaFilmOpacities.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaFilmOpacities not subscribed...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaFilmOpacities.GetNewSamples([&](auto msg){
        SmtfilmOpacities_t opacities;
        Convert_SmtfilmOpacity_API2SOA(static_cast<eevp::control::SoaFilmOpacityArray>(*msg), opacities);
        LOG_DEBUG() << "soaFilmOpacities notified: value = " << getString_SmtfilmOpacities_t(opacities) << "\n";
        if (this->mListener != nullptr)
        {
            this->mListener->notifySmartFilmOpacities(opacities);
        }
    });

    LOG_DEBUG() << "(-)\n";
}

#include <sstream>
std::string SmartFilmProxyImpl::GetSubscriptStatusString()
{
    std::stringstream ss;

    ss << "soaSmartFilmDeviceNormal: ";
    ss << ((this->mProxy->soaFilmDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    return ss.str();
}

} /// namespace smartfilm
} /// namespace control
} /// namespace eevp
