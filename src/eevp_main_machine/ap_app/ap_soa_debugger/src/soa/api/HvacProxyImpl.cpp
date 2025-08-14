#include <Common.h>
#include "HvacProxyImpl.h"
#include "Log.hpp"
#include <SoaDataType.h>
#include <CommonEnumConverter.h>
#include <HvacDataTypeConverter.h>
#include <Debug_DataType2String.hpp>

#define MAX_SERVICE_SEARCH_TRIAL    50 // 100ms * 50 = 5sec

using namespace ara::core;
using namespace eevp::control;
using namespace eevp::control::hvac;

namespace eevp {
namespace control {
namespace hvac {

HvacProxyImpl *HvacProxyImpl::s_mInst = nullptr;

HvacProxyImpl *HvacProxyImpl::GetInstance()
{
    if (HvacProxyImpl::s_mInst == nullptr)
    {
        HvacProxyImpl::s_mInst = new HvacProxyImpl();
    }

    return HvacProxyImpl::s_mInst;
}

void HvacProxyImpl::DeleteInstance()
{
    if (HvacProxyImpl::s_mInst != nullptr)
    {
        delete HvacProxyImpl::s_mInst;
        HvacProxyImpl::s_mInst = nullptr;
    }
}

HvacProxyImpl::HvacProxyImpl()
{
    this->mProxy = nullptr;
    this->mFindHandle = nullptr;
    this->mListener = nullptr;
}

HvacProxyImpl::~HvacProxyImpl()
{
    if (this->mProxy)
    {
        mProxy->StopFindService(*(this->mFindHandle));
        mProxy.reset();
    }
}

void HvacProxyImpl::setEventListener(std::shared_ptr<IHvacListener> _listener)
{
    this->mListener = _listener;
}

bool HvacProxyImpl::init()
{
    LOG_DEBUG() << "(+)\n";

    ara::core::InstanceSpecifier specifier("SOA_Debugger/AA/RPort_SOADBG_Hvac");

    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    auto result = proxy::SoaHvacProxy::StartFindService(callback, specifier);
    if (!result.HasValue())
    {
        LOG_INFO() << "SoaHvacProxy StartFindService() Failed\n";
        return false;
    }

    for (int i = 0; i < 50; i++)
    {
        if ((this->mFindHandle) && (this->mProxy))
        {
            LOG_INFO() << "SoaHvacProxy Start Find Service Success\n";
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    LOG_INFO() << "(-)\n";
    return false;
}

void HvacProxyImpl::SetHvacPower(const bool& on)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetHvacPower(on);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::SetAcOnOff(const bool& on)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetAcOnOff(on);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::SetAutoTemp(const bool& on)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetAutoTemp(on);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::SetSync(const bool& on)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetSync(on);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::SetDriverTemp(const std::int8_t& temperature)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetDriverTemp(temperature);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::SetPassengerTemp(const std::int8_t& temperature)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetPassengerTemp(temperature);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::SetBlowingForce(const std::uint8_t& force)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetBlowingForce(force);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::SetBlowingDirection(const eevp::control::SoaHvacBlowDir& dir)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetBlowingDirection(dir);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::SetAirSource(const eevp::control::SoaHvacAirSource& src)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetAirSource(src);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::SetForwardDefrostOn(const bool& on)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetForwardDefrostOn(on);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::SetBackwardDefrostOn(const bool& on)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetBackwardDefrostOn(on);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

bool HvacProxyImpl::getterIsDeviceNormal(DeviceNormal_e &isDeviceNormal)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-) ret=false\n";
        return false;
    }

    auto future = this->mProxy->soaHvacDeviceNormal.Get();
    auto read_status = future.wait_for(std::chrono::milliseconds(10));
    if (read_status == future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            isDeviceNormal = Convert_DeviceNormal_API2SOA(static_cast<eevp::control::SoaDeviceIsNormal>(result.Value()));
            LOG_DEBUG() << "(-) ret=true\n";
            return true;
        }
        else
        {
           LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
        }
    }
    else
    {
       LOG_ERROR() << "Timeout to call soaHvacDeviceNormal Getter\n";//
    }

    LOG_DEBUG() << "(-) ret=false\n";
    return false;
}

bool HvacProxyImpl::getterHvacSetting(HvacStatus_t &setting)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-) ret=false\n";
        return false;
    }

    auto future = this->mProxy->soaHvacSetting.Get();
    auto read_status = future.wait_for(std::chrono::milliseconds(10));
    if (read_status == future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            Convert_HvacStatus_API2SOA(static_cast<eevp::control::SoaHvacStatus>(result.Value()), setting);
            LOG_DEBUG() << "(-) ret=true\n";
            return true;
        }
        else
        {
           LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
        }
    }
    else
    {
       LOG_ERROR() << "Timeout to call soaHvacSetting Getter\n";//
    }

    LOG_DEBUG() << "(-) ret=false\n";
    return false;
}

bool HvacProxyImpl::getterHvacStatus(HvacStatus_t &status)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-) ret=false\n";
        return false;
    }

    auto future = this->mProxy->soaHvacStatus.Get();
    auto read_status = future.wait_for(std::chrono::milliseconds(10));
    if (read_status == future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            Convert_HvacStatus_API2SOA(static_cast<eevp::control::SoaHvacStatus>(result.Value()), status);
            LOG_DEBUG() << "(-) ret=true\n";
            return true;
        }
        else
        {
           LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
        }
    }
    else
    {
       LOG_ERROR() << "Timeout to call soaHvacStatus Getter\n";//
    }

    LOG_DEBUG() << "(-) ret=false\n";
    return false;
}

bool HvacProxyImpl::getterHvacTemps(HvacTemps_t &temps)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-) ret=false\n";
        return false;
    }

    auto future = this->mProxy->soaHvacTemps.Get();
    auto read_status = future.wait_for(std::chrono::milliseconds(10));
    if (read_status == future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            Convert_HvacTemps_API2SOA(static_cast<eevp::control::SoaHvacTemps>(result.Value()), temps);
            LOG_DEBUG() << "(-) ret=true\n";
            return true;
        }
        else
        {
           LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
        }
    }
    else
    {
       LOG_ERROR() << "Timeout to call soaHvacTemps Getter\n";//
    }

    LOG_DEBUG() << "(-) ret=false\n";
    return false;
}

bool HvacProxyImpl::getterHvacSwVersion(int &version)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-) ret=false\n";
        return false;
    }

    auto future = this->mProxy->SoaHvacSwVersion.Get();
    auto read_status = future.wait_for(std::chrono::milliseconds(10));
    if (read_status == future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            std::uint8_t value = static_cast<std::uint8_t>(result.Value());
            version = (int)value;
            LOG_DEBUG() << "(-) ret=true\n";
            return true;
        }
        else
        {
           LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
        }
    }
    else
    {
       LOG_ERROR() << "Timeout to call SoaHvacSwVersion Getter\n";//
    }

    LOG_DEBUG() << "(-) ret=false\n";
    return false;
}

void HvacProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaHvacProxy::HandleType> container,
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
    this->mProxy = std::make_shared<proxy::SoaHvacProxy>(container.at(0));

    SubscribeEvent();
    SubscribeField();

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::SubscribeEvent()
{
    LOG_DEBUG() << "(+)\n";
    // not used
    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::SubscribeField()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    // soaHvacDeviceNormal
    if (this->mProxy->soaHvacDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaHvacDeviceNormal subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->soaHvacDeviceNormal.SetReceiveHandler(std::bind(&HvacProxyImpl::cbIsDeviceNormal, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for soaHvacDeviceNormal with \n";// << result.Error().Message();
        }

        mProxy->soaHvacDeviceNormal.Subscribe(1);

        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to Subscribe for soaHvacDeviceNormal with \n";// << result.Error().Message();
        }
    }

    // soaHvacSetting
    if (this->mProxy->soaHvacSetting.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaHvacSetting subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->soaHvacSetting.SetReceiveHandler(std::bind(&HvacProxyImpl::cbHvacSetting, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for soaHvacSetting with " << result.Error().Message() << "\n";
        }

        mProxy->soaHvacSetting.Subscribe(1);

        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to Subscribe for soaHvacSetting with " << result.Error().Message() << "\n";
        }
    }

    // soaHvacStatus
    if (this->mProxy->soaHvacStatus.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaHvacStatus subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->soaHvacStatus.SetReceiveHandler(std::bind(&HvacProxyImpl::cbHvacStatus, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for soaHvacStatus with " << result.Error().Message() << "\n";
        }

        mProxy->soaHvacStatus.Subscribe(1);

        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to Subscribe for soaHvacStatus with " << result.Error().Message() << "\n";
        }
    }

    LOG_DEBUG() << "(-)\n";

    // soaHvacTemps
    if (this->mProxy->soaHvacTemps.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaHvacTemps subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->soaHvacTemps.SetReceiveHandler(std::bind(&HvacProxyImpl::cbHvacTemps, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for soaHvacTemps with " << result.Error().Message() << "\n";
        }

        mProxy->soaHvacTemps.Subscribe(1);

        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to Subscribe for soaHvacTemps with " << result.Error().Message() << "\n";
        }
    }

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::cbIsDeviceNormal()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
    }

    if (this->mProxy->soaHvacDeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaHvacDeviceNormal not subscribed...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaHvacDeviceNormal.GetNewSamples([&](auto msg){
        DeviceNormal_e isDeviceNormal = Convert_DeviceNormal_API2SOA(static_cast<eevp::control::SoaDeviceIsNormal>(*msg));
        if (this->mListener != nullptr)
        {
            this->mListener->notifyIsDeviceNormal(isDeviceNormal);
        }
    });

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::cbHvacSetting()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
    }

    if (this->mProxy->soaHvacSetting.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaHvacSetting not subscribed...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaHvacSetting.GetNewSamples([&](auto msg){
        HvacStatus_t setting;
        Convert_HvacStatus_API2SOA(static_cast<eevp::control::SoaHvacStatus>(*msg), setting);
        if (this->mListener != nullptr)
        {
            this->mListener->notifyHvacSetting(setting);
        }
    });

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::cbHvacStatus()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
    }

    if (this->mProxy->soaHvacStatus.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaHvacStatus not subscribed...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaHvacStatus.GetNewSamples([&](auto msg){
        HvacStatus_t status;
        Convert_HvacStatus_API2SOA(static_cast<eevp::control::SoaHvacStatus>(*msg), status);
        if (this->mListener != nullptr)
        {
            this->mListener->notifyHvacStatus(status);
        }
    });

    LOG_DEBUG() << "(-)\n";
}

void HvacProxyImpl::cbHvacTemps()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
    }

    if (this->mProxy->soaHvacTemps.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaHvacTemps not subscribed...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaHvacTemps.GetNewSamples([&](auto msg){
        HvacTemps_t temps;
        Convert_HvacTemps_API2SOA(static_cast<eevp::control::SoaHvacTemps>(*msg), temps);
        if (this->mListener != nullptr)
        {
            this->mListener->notifyHvacTemps(temps);
        }
    });

    LOG_DEBUG() << "(-)\n";
}

#include <sstream>
std::string HvacProxyImpl::GetSubscriptStatusString()
{
    std::stringstream ss;

    ss << "soaHvacDeviceNormal: ";
    ss << ((this->mProxy->soaHvacDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    return ss.str();
}

} /// namespace hvac
} /// namespace control
} /// namespace eevp
