#include "Zone5DmsProxyImpl.hpp"
#include "Zone5DmsConverter.hpp"
#include "thread"
#include "Log.hpp"

namespace zone5
{
namespace dms
{
namespace control
{

inline static void convert_Zone5DmsStatus2Soa(const zone5::dms::control::EcuZ5DmsStatus &status, bool &powerOn, DmsDriverInside_e &inside)
{
	powerOn = (status.powerOn != 0) ? true : false;
	inside = Convert_DriverInside_Zone2SOA(status.subElement3);
}

inline static void convert_Zone5DriverStatus2Soa(const zone5::dms::control::EcuZ5DmsDriverStatus &driver_status, DmsFatigue_e &fatigue, DmsGazingDir_e &dir, DmsOnPhone_e &phone, DmsSmoking_e &smoking)
{
	fatigue = Convert_Fatigue_Zone2SOA(driver_status.fatigue);
	dir = Convert_GazingDir_Zone2SOA(driver_status.gazingDir);
	phone = Convert_OnPhone_Zone2SOA(driver_status.onPhone);
	smoking = Convert_Smoke_Zone2SOA(driver_status.smoking);
}

Zone5DmsProxyImpl *Zone5DmsProxyImpl::s_mInst = nullptr;

Zone5DmsProxyImpl *Zone5DmsProxyImpl::GetInstance()
{
	if (Zone5DmsProxyImpl::s_mInst == nullptr)
	{
		Zone5DmsProxyImpl::s_mInst = new Zone5DmsProxyImpl();
	}

	return Zone5DmsProxyImpl::s_mInst;
}

void Zone5DmsProxyImpl::DeleteInstance()
{
	if (Zone5DmsProxyImpl::s_mInst != nullptr)
	{
		delete Zone5DmsProxyImpl::s_mInst;
		Zone5DmsProxyImpl::s_mInst = nullptr;
	}
}

Zone5DmsProxyImpl::Zone5DmsProxyImpl()
{
	this->mServiceFound = false;
	this->mProxy = nullptr;
	this->mFindHandle = nullptr;
	this->mListenerList.clear();
}

Zone5DmsProxyImpl::~Zone5DmsProxyImpl()
{
	if (this->mProxy)
	{
		this->mProxy->StopFindService(*(this->mFindHandle));
		this->mProxy.reset();
	}

	this->mListenerList.clear();
}

void Zone5DmsProxyImpl::setEventListener(const std::shared_ptr<zone5::dms::control::IZone5DmsListener> _listener)
{
	for (auto item: this->mListenerList)
	{
		if (item.get() == _listener.get())
		{
			return;
		}
	}

	this->mListenerList.push_back(_listener);
}

bool Zone5DmsProxyImpl::init()
{
	LOG_INFO() << "(+)\n";

	ara::core::InstanceSpecifier specifier("SOA_DMS/AA/RPort_Zone5Dms");

	auto callback = [&](auto container, auto findHandle) {
		FindServiceCallback(container, findHandle);
	};

	auto result = proxy::Zone5DmsProxy::StartFindService(callback, specifier);

	if (!result.HasValue())
	{
		LOG_INFO() << "Zone5DmsProxy StartFindService() Failed\n";
		return false;
	}

#if 0
	for (int i = 0; i < 20; i++)
	{
		if (this->mServiceFound)
		{
			LOG_INFO() << "Zone5DmsProxy Start Find Service Success\n";
			SubscribeEvent();
			SubscribeField();
			return true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
#endif

	LOG_INFO() << "(-)\n";
	return false;
}

void Zone5DmsProxyImpl::FindServiceCallback(
		ara::com::ServiceHandleContainer<zone5::dms::control::proxy::Zone5DmsProxy::HandleType> container,
		ara::com::FindServiceHandle findHandle)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy != nullptr)
	{
		LOG_WARNING() << "proxy exists: remove the old proxy\n";
		this->UnsubscribeEvent();
		this->UnsubscribeField();

		this->mFindHandle = nullptr;
		this->mProxy = nullptr;
	}

	if (container.empty())
	{
		LOG_ERROR() << "container.empty() ... \n";
		if (this->mProxy != nullptr)
		{
			LOG_WARNING() << "proxy exists: remove the old proxy\n";
			this->UnsubscribeEvent();
			this->UnsubscribeField();

			this->mFindHandle = nullptr;
			this->mProxy = nullptr;
		}
	}
	else
	{
		for (auto& handle : container)
		{
			LOG_DEBUG() <<  "Zone5DmsProxyImpl::Find::Searched Instance::ServiceId =" <<
										handle.GetServiceHandle().serviceId <<
										", InstanceId =" <<
										handle.GetServiceHandle().instanceId << "\n";
		}

		this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
		this->mProxy = std::make_shared<proxy::Zone5DmsProxy>(container.at(0));

		LOG_INFO() << "Zone5DmsProxy Find-Service Success\n";
		SubscribeEvent();
		SubscribeField();
		this->mServiceFound = true;
	}

	LOG_INFO() << "(-)\n";
}

void Zone5DmsProxyImpl::SetPower(bool on)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->SetDmsPower((on ? (uint8_t)1: (uint8_t)0));
	LOG_INFO() << "(-)\n";
}

bool Zone5DmsProxyImpl::getterDeviceNormal(DeviceNormal_e &isNormal)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone5DmsDeviceNormal.Get();
	auto status = future.wait_for(std::chrono::milliseconds(250));
	if (status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			std::uint8_t ret_val = static_cast<std::uint8_t>(result.Value());
			isNormal = Convert_DeviceNormal_Zone2SOA(ret_val);
			LOG_INFO() << "(-) isNormal=" << (int)isNormal << "\n";
			return true;
		}
		else
		{
		   LOG_ERROR() << "Return error with\n";// << result.Error().Message();
		}
	}
	else
	{
	   LOG_ERROR() << "Timeout to call zone5DmsDeviceNormal Getter\n";//
	}

	LOG_INFO() << "(-)\n";
	return false;
}

bool Zone5DmsProxyImpl::getterDmsStatus(bool &powerOn, DmsDriverInside_e &inside)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone5DmsStatus.Get();
	auto status = future.wait_for(std::chrono::milliseconds(250));
	if (status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			zone5::dms::control::EcuZ5DmsStatus status = static_cast<zone5::dms::control::EcuZ5DmsStatus>(result.Value());
			convert_Zone5DmsStatus2Soa(status, powerOn, inside);
			LOG_INFO() << "(-) powerOn = " << (int)status.powerOn << ", inside = " << (int)status.subElement3 << "\n";
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

bool Zone5DmsProxyImpl::getterDmsDriverStatus(DmsFatigue_e &fatigue, DmsGazingDir_e &dir, DmsOnPhone_e &phone, DmsSmoking_e &smoking)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone5DmsDriverStatus.Get();
	auto status = future.wait_for(std::chrono::milliseconds(250));
	if (status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			zone5::dms::control::EcuZ5DmsDriverStatus driver_status = static_cast<zone5::dms::control::EcuZ5DmsDriverStatus>(result.Value());
			convert_Zone5DriverStatus2Soa(driver_status, fatigue, dir, phone, smoking);
			LOG_INFO() << "(-) fatigue = " << (int)driver_status.fatigue << ", gazingDir = " << (int)driver_status.gazingDir << ", phone = " << (int)driver_status.onPhone   << ", smoking = " << (int)driver_status.smoking << "\n";
			return true;
		}
		else
		{
		   LOG_ERROR() << "Return error with\n";// << result.Error().Message();
		}
	}
	else
	{
	   LOG_ERROR() << "Timeout to call zone5DmsDriverStatus Getter\n";//
	}

	LOG_INFO() << "(-)\n";
	return false;
}

bool Zone5DmsProxyImpl::isServiceFound()
{
	return this->mServiceFound;
}

void Zone5DmsProxyImpl::SubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone5DmsProxyImpl::SubscribeField()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	// zone5DmsDeviceNormal
	if (this->mProxy->zone5DmsDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kNotSubscribed)
	{
		auto result = this->mProxy->zone5DmsDeviceNormal.SetReceiveHandler(std::bind(&Zone5DmsProxyImpl::cbZone5DmsDeviceNormal, this));
		if (result.HasValue())
		{
			result = this->mProxy->zone5DmsDeviceNormal.Subscribe(10);
			if (result.HasValue())
			{
				LOG_INFO() << "zone5DmsDeviceNormal subsciption succeeded\n";
			}
			else
			{
				LOG_ERROR() << "Failed to Subscribe for zone5DmsDeviceNormal\n";
			}
		}
		else
		{
			LOG_ERROR() << "Failed to SetReceiveHandler for zone5DmsDeviceNormal\n";
		}
	}
	else
	{
		LOG_ERROR() << "zone5DmsDeviceNormal.GetSubscriptionState() != kNotSubscribed\n";
	}

	// zone5DmsDriverStatus
	if (this->mProxy->zone5DmsDriverStatus.GetSubscriptionState() == ara::com::SubscriptionState::kNotSubscribed)
	{
		auto result = this->mProxy->zone5DmsDriverStatus.SetReceiveHandler(std::bind(&Zone5DmsProxyImpl::cbZone5DmsDriverStatus, this));
		if (result.HasValue())
		{
			result = this->mProxy->zone5DmsDriverStatus.Subscribe(10);
			if (result.HasValue())
			{
				LOG_INFO() << "zone5DmsDriverStatus subsciption succeeded\n";
			}
			else
			{
				LOG_ERROR() << "Failed to Subscribe for zone5DmsDriverStatus\n";
			}
		}
		else
		{
			LOG_ERROR() << "Failed to SetReceiveHandler for zone5DmsDriverStatus\n";
		}
	}
	else
	{
		LOG_ERROR() << "zone5DmsDriverStatus.GetSubscriptionState() != kNotSubscribed\n";
	}

	// zone5DmsStatus
	if (this->mProxy->zone5DmsStatus.GetSubscriptionState() == ara::com::SubscriptionState::kNotSubscribed)
	{
		auto result = this->mProxy->zone5DmsStatus.SetReceiveHandler(std::bind(&Zone5DmsProxyImpl::cbZone5DmsStatus, this));
		if (result.HasValue())
		{
			result = this->mProxy->zone5DmsStatus.Subscribe(10);
			if (result.HasValue())
			{
				LOG_INFO() << "zone5DmsStatus subsciption succeeded\n";
			}
			else
			{
				LOG_ERROR() << "Failed to Subscribe for zone5DmsStatus\n";
			}
		}
		else
		{
			LOG_ERROR() << "Failed to SetReceiveHandler for zone5DmsStatus\n";
		}
	}
	else
	{
		LOG_ERROR() << "zone5DmsStatus.GetSubscriptionState() != kNotSubscribed\n";
	}

	LOG_INFO() << "(-)\n";
}

void Zone5DmsProxyImpl::UnsubscribeEvent()
{
	if (this->mProxy != nullptr)
	{
		// To Do:
	}
}

void Zone5DmsProxyImpl::UnsubscribeField()
{
	if (this->mProxy != nullptr)
	{
		// zone5DmsDeviceNormal
		this->mProxy->zone5DmsDeviceNormal.Unsubscribe();

		// zone5DmsDriverStatus
		this->mProxy->zone5DmsDriverStatus.Unsubscribe();

		// zone5DmsStatus
		this->mProxy->zone5DmsStatus.Unsubscribe();
	}
}

void Zone5DmsProxyImpl::cbZone5DmsDeviceNormal()
{
	LOG_INFO() << "(+)\n";

	std::uint8_t fieldValue;

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone5DmsDeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone5DmsDeviceNormal.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone5DmsDeviceNormal.GetNewSamples([&](auto msg) {
		fieldValue = static_cast<std::uint8_t>(*msg);   // fieldValue = *msg
		DeviceNormal_e isNormal = Convert_DeviceNormal_Zone2SOA(fieldValue);

		LOG_INFO() << "isNormal: " << (int)isNormal << "\n";

		for (auto item: this->mListenerList)
		{
			item->notifyDeviceNormal(isNormal);
		}
	});

	LOG_INFO() << "(-)\n";
}

void Zone5DmsProxyImpl::cbZone5DmsStatus()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone5DmsStatus.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone5DmsStatus.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone5DmsStatus.GetNewSamples([&](auto msg) {
		zone5::dms::control::EcuZ5DmsStatus status = static_cast<zone5::dms::control::EcuZ5DmsStatus>(*msg);   // fieldValue = *msg
		bool powerOn;
		DmsDriverInside_e inside;
		convert_Zone5DmsStatus2Soa(status, powerOn, inside);

		LOG_INFO() << "powerOn = " << (int)powerOn << ", inside = " << (int)inside << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifyDmsStatus(powerOn, inside);
		}
	});

	LOG_INFO() << "(-)\n";
}

void Zone5DmsProxyImpl::cbZone5DmsDriverStatus()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone5DmsDriverStatus.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone5DmsDriverStatus.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone5DmsDriverStatus.GetNewSamples([&](auto msg) {
		zone5::dms::control::EcuZ5DmsDriverStatus driver_status = static_cast<zone5::dms::control::EcuZ5DmsDriverStatus>(*msg);
		DmsFatigue_e fatigue;
		DmsGazingDir_e dir;
		DmsOnPhone_e phone;
		DmsSmoking_e smoking;
		convert_Zone5DriverStatus2Soa(driver_status, fatigue, dir, phone, smoking);
		LOG_INFO() << "fatigue = " << (int)fatigue << ", gazingDir = " << (int)dir << ", phone = " << (int)phone  << ", smoking = " << (int)smoking << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifyDriverStatus(fatigue, dir, phone, smoking);
		}
	});

	LOG_INFO() << "(-)\n";
}

}
}
}