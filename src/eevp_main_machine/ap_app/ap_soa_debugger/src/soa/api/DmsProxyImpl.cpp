#include <Common.h>
#include "DmsProxyImpl.h"
#include "Log.hpp"
#include <CommonEnumConverter.h>
#include <DmsDataTypeConverter.h>
#include <Debug_DataType2String.hpp>


#define MAX_SERVICE_SEARCH_TRIAL    50 // 100ms * 50 = 5sec

using namespace ara::core;
using namespace eevp::control;
using namespace eevp::control::dms;

namespace eevp {
namespace control {
namespace dms {

inline static void convert_ApiDmsStatus2Soa(const eevp::control::SoaDmsStatus &status, bool &powerOn, DmsDriverInside_e &inside)
{
	powerOn = status.powerOn;
	inside = Convert_DmsDriverInside_API2SOA(status.driverInside);
}

inline static void convert_ApiDriverStatus2Soa(const eevp::control::soaDmsDriverStatus &driver_status, DmsFatigue_e &fatigue, DmsGazingDir_e &dir, DmsOnPhone_e &phone, DmsSmoking_e &smoking)
{
    fatigue = Convert_DmsFatigue_API2SOA(driver_status.fatigue);
    dir = Convert_DmsGazingDir_API2SOA(driver_status.gazingDir);
    phone = Convert_DmsOnPhone_API2SOA(driver_status.onPhone);
    smoking = Convert_DmsSmoke_API2SOA(driver_status.smoking);
}

DmsProxyImpl *DmsProxyImpl::s_mInst = nullptr;

DmsProxyImpl *DmsProxyImpl::GetInstance()
{
    if (DmsProxyImpl::s_mInst == nullptr)
    {
        DmsProxyImpl::s_mInst = new DmsProxyImpl();
    }

    return DmsProxyImpl::s_mInst;
}

void DmsProxyImpl::DeleteInstance()
{
    if (DmsProxyImpl::s_mInst != nullptr)
    {
        delete DmsProxyImpl::s_mInst;
        DmsProxyImpl::s_mInst = nullptr;
    }
}

DmsProxyImpl::DmsProxyImpl()
{
    this->mProxy = nullptr;
    this->mFindHandle = nullptr;
    this->mListener = nullptr;
}

DmsProxyImpl::~DmsProxyImpl()
{
    if (this->mProxy)
    {
        mProxy->StopFindService(*(this->mFindHandle));
        mProxy.reset();
    }
}

void DmsProxyImpl::setEventListener(std::shared_ptr<IDmsListener> _listener)
{
    this->mListener = _listener;
}

bool DmsProxyImpl::init()
{
    LOG_DEBUG() << "(+)\n";

    ara::core::InstanceSpecifier specifier("SOA_Debugger/AA/RPort_SOADBG_Dms");

    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    auto result = proxy::SoaDmsProxy::StartFindService(callback, specifier);
    if (!result.HasValue())
    {
        LOG_INFO() << "SoaDmsProxy StartFindService() Failed\n";
        return false;
    }

    for (int i = 0; i < 50; i++)
    {
        if ((this->mFindHandle) && (this->mProxy))
        {
            LOG_INFO() << "SoaDmsProxy Start Find Service Success\n";
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    LOG_INFO() << "(-)\n";
    return false;
}

void DmsProxyImpl::SetPower(bool on)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy != nullptr)
    {
        this->mProxy->SetDmsPower(on);
    }
    else
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
    }

    LOG_DEBUG() << "(-)\n";
}

bool DmsProxyImpl::getterIsDeviceNormal(DeviceNormal_e &isDeviceNormal)
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-) ret=false\n";
        return false;
    }

    auto future = this->mProxy->soaDmsDeviceNormal.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready)
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
       LOG_ERROR() << "Timeout to call soaRctnStatus's Getter\n";//
    }

    LOG_DEBUG() << "(-) ret=false\n";
    return false;
}

bool DmsProxyImpl::getterDmsStatus(bool &powerOn, DmsDriverInside_e &inside)
{
    LOG_INFO() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_INFO() << "(-) mProxy is null\n";
        return false;
    }

    auto future = this->mProxy->soaDmsStatus.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == ara::core::future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            eevp::control::SoaDmsStatus status = static_cast<eevp::control::SoaDmsStatus>(result.Value());
            convert_ApiDmsStatus2Soa(status, powerOn, inside);
            LOG_INFO() << "(-) powerOn = " << (int)powerOn << ", inside = " << (int)inside << "\n";
            return true;
        }
        else
        {
           LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
        }
    }
    else
    {
       LOG_ERROR() << "Timeout to call zone5DmsDriverStatus Getter\n";//
    }

    LOG_INFO() << "(-)\n";
    return false;
}

bool DmsProxyImpl::getterDmsDriverStatus(DmsFatigue_e &fatigue, DmsGazingDir_e &dir, DmsOnPhone_e &phone, DmsSmoking_e &smoking)
{
    LOG_INFO() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_INFO() << "(-) mProxy is null\n";
        return false;
    }

    auto future = this->mProxy->soaDmsDriverStatus.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == ara::core::future_status::ready)
    {
        auto result = future.GetResult();
        if (result.HasValue())
        {
            eevp::control::soaDmsDriverStatus driver_status = static_cast<eevp::control::soaDmsDriverStatus>(result.Value());
            convert_ApiDriverStatus2Soa(driver_status, fatigue, dir, phone, smoking);
            LOG_INFO() << "(-) fatigue = " << (int)fatigue << ", gazingDir = " << (int)dir << ", phone = " << (int)phone  << ", smoking = " << (int)smoking << "\n";
            return true;
        }
        else
        {
           LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
        }
    }
    else
    {
       LOG_ERROR() << "Timeout to call zone5DmsDriverStatus Getter\n";//
    }

    LOG_INFO() << "(-)\n";
    return false;
}

void DmsProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaDmsProxy::HandleType> container,
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
    this->mProxy = std::make_shared<proxy::SoaDmsProxy>(container.at(0));

    SubscribeEvent();
    SubscribeField();

    LOG_DEBUG() << "(-)\n";
}

void DmsProxyImpl::SubscribeEvent()
{
    LOG_DEBUG() << "(+)\n";
    // not used
    // onDriverNotFocusing
    if (this->mProxy->onDriverNotFocusing.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "onDriverNotFocusing subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->onDriverNotFocusing.SetReceiveHandler(std::bind(&DmsProxyImpl::cbOnDriverNotFocusing, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for onDriverNotFocusing with " << result.Error().Message() << "\n";
        }

        mProxy->onDriverNotFocusing.Subscribe(1);

        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to Subscribe for onDriverNotFocusing with " << result.Error().Message() << "\n";
        }
    }

    // onDriverNotWaken
    if (this->mProxy->onDriverNotWaken.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "onDriverNotWaken subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->onDriverNotWaken.SetReceiveHandler(std::bind(&DmsProxyImpl::cbOnDriverNotWaken, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for onDriverNotWaken with " << result.Error().Message() << "\n";
        }

        mProxy->onDriverNotWaken.Subscribe(1);

        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to Subscribe for onDriverNotWaken with " << result.Error().Message() << "\n";
        }
    }

    // onDriverNotWatchingFront
    if (this->mProxy->onDriverNotWatchingFront.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "onDriverNotWatchingFront subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->onDriverNotWatchingFront.SetReceiveHandler(std::bind(&DmsProxyImpl::cbOnDriverNotWatchingFront, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for onDriverNotWatchingFront with " << result.Error().Message() << "\n";
        }

        mProxy->onDriverNotWatchingFront.Subscribe(1);

        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to Subscribe for onDriverNotWatchingFront with " << result.Error().Message() << "\n";
        }
    }


    LOG_DEBUG() << "(-)\n";
}

void DmsProxyImpl::SubscribeField()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    if (this->mProxy->soaDmsDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaDmsDeviceNormal subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->soaDmsDeviceNormal.SetReceiveHandler(std::bind(&DmsProxyImpl::cbIsDeviceNormal, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for cbIsDeviceNormal with " << result.Error().Message() << "\n";
        }

        mProxy->soaDmsDeviceNormal.Subscribe(1);

        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to Subscribe for soaDmsDeviceNormal with " << result.Error().Message() << "\n";
        }
    }

    if (this->mProxy->soaDmsStatus.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaDmsDeviceNormal subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->soaDmsStatus.SetReceiveHandler(std::bind(&DmsProxyImpl::cbDmsStatus, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for cbIsDeviceNormal with " << result.Error().Message() << "\n";
        }

        mProxy->soaDmsStatus.Subscribe(1);

        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to Subscribe for soaDmsStatus with " << result.Error().Message() << "\n";
        }
    }

    if (this->mProxy->soaDmsDriverStatus.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaDmsDriverStatus subscribed...\n";
    }
    else
    {
        auto result = this->mProxy->soaDmsDriverStatus.SetReceiveHandler(std::bind(&DmsProxyImpl::cbDriverStatus, this));
        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to SetReceiveHandler for cbDriverStatus with " << result.Error().Message() << "\n";
        }

        mProxy->soaDmsDriverStatus.Subscribe(1);

        if (!result.HasValue())
        {
            LOG_WARNING() << "Failed to Subscribe for soaDmsDriverStatus with " << result.Error().Message() << "\n";
        }
    }

    LOG_DEBUG() << "(-)\n";
}

void DmsProxyImpl::cbOnDriverNotFocusing()
{
    LOG_DEBUG() << "(+)\n";

    LOG_DEBUG() << "(-)\n";
}

void DmsProxyImpl::cbOnDriverNotWaken()
{
    LOG_DEBUG() << "(+)\n";

    LOG_DEBUG() << "(-)\n";
}

void DmsProxyImpl::cbOnDriverNotWatchingFront()
{
    LOG_DEBUG() << "(+)\n";

    LOG_DEBUG() << "(-)\n";
}

void DmsProxyImpl::cbIsDeviceNormal()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
    }

    if (this->mProxy->soaDmsDeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaDmsDeviceNormal not subscribed...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaDmsDeviceNormal.GetNewSamples([&](auto msg){
        DeviceNormal_e isDeviceNormal = Convert_DeviceNormal_API2SOA(static_cast<eevp::control::SoaDeviceIsNormal>(*msg));
        if (this->mListener != nullptr)
        {
            this->mListener->notifyIsDeviceNormal(isDeviceNormal);
        }
    });

    LOG_DEBUG() << "(-)\n";
}

void DmsProxyImpl::cbDmsStatus()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
    }

    if (this->mProxy->soaDmsStatus.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaDmsStatus not subscribed...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaDmsStatus.GetNewSamples([&](auto msg){
        eevp::control::SoaDmsStatus status = static_cast<eevp::control::SoaDmsStatus>(*msg);
        bool powerOn;
        DmsDriverInside_e inside;
        convert_ApiDmsStatus2Soa(status, powerOn, inside);
        if (this->mListener != nullptr)
        {
            this->mListener->notifyDmsStatus(powerOn, inside);
        }

        LOG_DEBUG() << "powerOn=" << getStringEnum_bool(powerOn) << ", inside=" << getStringEnum_DmsDriverInside_e(inside) << "\n";
    });

    LOG_DEBUG() << "(-)\n";
}

void DmsProxyImpl::cbDriverStatus()
{
    LOG_DEBUG() << "(+)\n";

    if (this->mProxy == nullptr)
    {
        LOG_ERROR() << "this->mProxy == nullptr\n";
        LOG_DEBUG() << "(-)\n";
    }

    if (this->mProxy->soaDmsDriverStatus.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
    {
        LOG_ERROR() << "soaDmsDriverStatus not subscribed...\n";
        LOG_DEBUG() << "(-)\n";
        return;
    }

    this->mProxy->soaDmsDriverStatus.GetNewSamples([&](auto msg){
        eevp::control::soaDmsDriverStatus driver_status = static_cast<eevp::control::soaDmsDriverStatus>(*msg);
        DmsFatigue_e fatigue;
        DmsGazingDir_e dir;
        DmsOnPhone_e phone;
        DmsSmoking_e smoking;
        convert_ApiDriverStatus2Soa(driver_status, fatigue, dir, phone, smoking);
        if (this->mListener != nullptr)
        {
            this->mListener->notifyDriverStatus(fatigue, dir, phone, smoking);
        }

        LOG_DEBUG() << "fatigue=" << getStringEnum_DmsFatigue_e(fatigue) << ", dir=" << getStringEnum_DmsGazingDir_e(dir) << ", phone="<< getStringEnum_DmsOnPhone_e(phone) << ", smoking=" << getStringEnum_DmsSmoking_e(smoking)<< "\n";
    });

    LOG_DEBUG() << "(-)\n";
}

#include <sstream>
std::string DmsProxyImpl::GetSubscriptStatusString()
{
    std::stringstream ss;

    ss << "soaDmsDeviceNormal: ";
    ss << ((this->mProxy->soaDmsDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) ? "Subscribed" : "Not Subscribed") << "\n";

    return ss.str();
}

} /// namespace dms
} /// namespace control
} /// namespace eevp
