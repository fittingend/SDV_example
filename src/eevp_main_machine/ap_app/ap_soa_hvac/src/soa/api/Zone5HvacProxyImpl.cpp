#include <Common.h>
#include <SoaDataType.h>
#include "Zone5HvacProxyImpl.hpp"
#include "thread"
#include <Zone5HvacConverter.hpp>
#include <Log.hpp>
#include <Debug_DataType2String.hpp>

namespace zone5
{
namespace hvac
{
namespace control
{

Zone5HvacProxyImpl *Zone5HvacProxyImpl::s_mInst = nullptr;

Zone5HvacProxyImpl *Zone5HvacProxyImpl::GetInstance()
{
	if (Zone5HvacProxyImpl::s_mInst == nullptr)
	{
		Zone5HvacProxyImpl::s_mInst = new Zone5HvacProxyImpl();
	}

	return Zone5HvacProxyImpl::s_mInst;
}

void Zone5HvacProxyImpl::DeleteInstance()
{
	if (Zone5HvacProxyImpl::s_mInst != nullptr)
	{
		delete Zone5HvacProxyImpl::s_mInst;
		Zone5HvacProxyImpl::s_mInst = nullptr;
	}
}

Zone5HvacProxyImpl::Zone5HvacProxyImpl()
{
	this->mServiceFound = false;
	this->mProxy = nullptr;
	this->mFindHandle = nullptr;
	this->mListenerList.clear();
}

Zone5HvacProxyImpl::~Zone5HvacProxyImpl()
{
	if (this->mProxy)
	{
		this->mProxy->StopFindService(*(this->mFindHandle));
		this->mProxy.reset();
	}

	this->mListenerList.clear();
}

void Zone5HvacProxyImpl::setEventListener(const std::shared_ptr<zone5::hvac::control::IZone5HvacListener> _listener)
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

bool Zone5HvacProxyImpl::init()
{
	LOG_INFO() << "(+)\n";

	ara::core::InstanceSpecifier specifier("SOA_HVAC/AA/RPort_Zone5_HVAC");

	auto callback = [&](auto container, auto findHandle) {
		FindServiceCallback(container, findHandle);
	};

	auto result = proxy::Zone5HvacProxy::StartFindService(callback, specifier);

	if (!result.HasValue())
	{
		LOG_INFO() << "Zone5HvacProxy StartFindService() Failed\n";
		return false;
	}

#if 0
	for (int i = 0; i < 20; i++)
	{
		if (this->mServiceFound)
		{
			LOG_INFO() << "Zone5HvacProxy Start Find Service Success\n";
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

void Zone5HvacProxyImpl::FindServiceCallback(
		ara::com::ServiceHandleContainer<zone5::hvac::control::proxy::Zone5HvacProxy::HandleType> container,
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
			LOG_DEBUG() <<  "Zone5HvacProxyImpl::Find::Searched Instance::ServiceId =" <<
										handle.GetServiceHandle().serviceId <<
										", InstanceId =" <<
										handle.GetServiceHandle().instanceId << "\n";
		}

		this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
		this->mProxy = std::make_shared<proxy::Zone5HvacProxy>(container.at(0));

		LOG_INFO() << "Zone5HvacProxy Find-Service Success\n";
		SubscribeEvent();
		SubscribeField();
		this->mServiceFound = true;
	}

	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::SetHvacPower(bool on)
{
	LOG_INFO() << "(+) on=" << getStringEnum_bool(on) << "\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->SetHvacPower((on ? (uint8_t)1: (uint8_t)0));
	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::SetAcOnOff(bool on)
{
	LOG_INFO() << "(+) on=" << getStringEnum_bool(on) << "\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->SetAcOnOff((on ? (uint8_t)1: (uint8_t)0));
	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::SetAutoTemp(bool on)
{
	LOG_INFO() << "(+) on=" << getStringEnum_bool(on) << "\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->SetAutoTemp((on ? (uint8_t)1: (uint8_t)0));
	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::SetSync(bool on)
{
	LOG_INFO() << "(+) on=" << getStringEnum_bool(on) << "\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->SetSync((on ? (uint8_t)1: (uint8_t)0));
	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::SetDriverTemp(int temperature)
{
	LOG_INFO() << "(+) temperature=" << temperature << "\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->SetDriverTemp((std::int8_t)temperature);
	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::SetPassengerTemp(int temperature)
{
	LOG_INFO() << "(+) temperature=" << temperature << "\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->SetPassengerTemp((std::int8_t)temperature);
	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::SetBlowingForce(int force)
{
	LOG_INFO() << "(+) force=" << force << "\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->SetBlowingForce((std::uint8_t)force);
	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::SetBlowingDirection(HvacBlowDir_e dir)
{
	LOG_INFO() << "(+) dir=" << getStringEnum_HvacBlowDir_e(dir) << "\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->SetBlowingDirection(Convert_BlowDir_SOA2Zone(dir));
	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::SetAirSource(HvacAirSource_e src)
{
	LOG_INFO() << "(+) src=" << getStringEnum_HvacAirSource_e(src) << "\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->SetAirSource(Convert_AirSource_SOA2Zone(src));
	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::SetForwardDefrostOn(bool on)
{
	LOG_INFO() << "(+) on=" << getStringEnum_bool(on) << "\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->SetForwardDefrostOn((on ? (uint8_t)1: (uint8_t)0));
	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::SetBackwardDefrostOn(bool on)
{
	LOG_INFO() << "(+) on=" << getStringEnum_bool(on) << "\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->SetBackwardDefrostOn((on ? (uint8_t)1: (uint8_t)0));
	LOG_INFO() << "(-)\n";
}

bool Zone5HvacProxyImpl::getterDeviceNormal(DeviceNormal_e &isNormal)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone5HvacDeviceNormal.Get();
	auto read_status = future.wait_for(std::chrono::milliseconds(250));
	if (read_status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			std::uint8_t ret_val = static_cast<std::uint8_t>(result.Value());
			DeviceNormal_e isNormal = Convert_DeviceNormal_Zone2SOA(ret_val);
			LOG_INFO() << "(-) ret_val=" << (int)ret_val << " -> isNormal=" << getStringEnum_DeviceNormal_e(isNormal) << "\n";
			return true;
		}
		else
		{
		   LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
		}
	}
	else
	{
	   LOG_ERROR() << "Timeout to call zone5HvacDeviceNormal Getter\n";//
	}

	LOG_INFO() << "(-)\n";
	return false;
}

bool Zone5HvacProxyImpl::getterHvacSetting(HvacStatus_t &setting)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone5HvacSetting.Get();
	auto read_status = future.wait_for(std::chrono::milliseconds(250));
	if (read_status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			zone5::hvac::control::EcuZ5HvacStatus ret_val = static_cast<zone5::hvac::control::EcuZ5HvacStatus>(result.Value());
			Convert_HvacStatus_Zone2SOA(ret_val, setting);
			return true;
		}
		else
		{
		   LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
		}
	}
	else
	{
	   LOG_ERROR() << "Timeout to call zone5HvacSetting Getter\n";//
	}

	LOG_INFO() << "(-)\n";
	return false;
}

bool Zone5HvacProxyImpl::getterHvacStatus(HvacStatus_t &status)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone5HvacStatus.Get();
	auto read_status = future.wait_for(std::chrono::milliseconds(250));
	if (read_status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			zone5::hvac::control::EcuZ5HvacStatus ret_val = static_cast<zone5::hvac::control::EcuZ5HvacStatus>(result.Value());
			Convert_HvacStatus_Zone2SOA(ret_val, status);
			return true;
		}
		else
		{
		   LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
		}
	}
	else
	{
	   LOG_ERROR() << "Timeout to call zone5HvacStatus Getter\n";//
	}

	LOG_INFO() << "(-)\n";
	return false;
}

bool Zone5HvacProxyImpl::getterHvacTemps(HvacTemps_t &temps)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone5HvacTemps.Get();
	auto read_status = future.wait_for(std::chrono::milliseconds(250));
	if (read_status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			zone5::hvac::control::EcuZ5HvacTemps ret_val = static_cast<zone5::hvac::control::EcuZ5HvacTemps>(result.Value());
			Convert_HvacTemps_Zone2SOA(ret_val, temps);
			return true;
		}
		else
		{
		   LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
		}
	}
	else
	{
	   LOG_ERROR() << "Timeout to call zone5HvacTemps Getter\n";//
	}

	LOG_INFO() << "(-)\n";
	return false;
}

bool Zone5HvacProxyImpl::isServiceFound()
{
	return this->mServiceFound;
}

void Zone5HvacProxyImpl::SubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::SubscribeField()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	// zone5DmsDeviceNormal
	if (this->mProxy->zone5HvacDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kNotSubscribed)
	{
		auto result = this->mProxy->zone5HvacDeviceNormal.SetReceiveHandler(std::bind(&Zone5HvacProxyImpl::cbZone5HvacDeviceNormal, this));
		if (result.HasValue())
		{
			result = this->mProxy->zone5HvacDeviceNormal.Subscribe(10);
			if (result.HasValue())
			{
				LOG_INFO() << "zone5HvacDeviceNormal subsciption succeeded\n";
			}
			else
			{
				LOG_ERROR() << "Failed to Subscribe for zone5HvacDeviceNormal\n";
			}
		}
		else
		{
			LOG_ERROR() << "Failed to SetReceiveHandler for zone5HvacDeviceNormal\n";
		}
	}
	else
	{
		LOG_ERROR() << "zone5HvacDeviceNormal.GetSubscriptionState() != kNotSubscribed\n";
	}

	// zone5HvacSetting
	if (this->mProxy->zone5HvacSetting.GetSubscriptionState() == ara::com::SubscriptionState::kNotSubscribed)
	{
		auto result = this->mProxy->zone5HvacSetting.SetReceiveHandler(std::bind(&Zone5HvacProxyImpl::cbZone5HvacSetting, this));
		if (result.HasValue())
		{
			result = this->mProxy->zone5HvacSetting.Subscribe(10);
			if (result.HasValue())
			{
				LOG_INFO() << "zone5HvacSetting subsciption succeeded\n";
			}
			else
			{
				LOG_ERROR() << "Failed to Subscribe for zone5HvacSetting\n";
			}
		}
		else
		{
			LOG_ERROR() << "Failed to SetReceiveHandler for zone5HvacSetting\n";
		}
	}
	else
	{
		LOG_ERROR() << "zone5HvacSetting.GetSubscriptionState() != kNotSubscribed\n";
	}

	// zone5HvacStatus
	if (this->mProxy->zone5HvacStatus.GetSubscriptionState() == ara::com::SubscriptionState::kNotSubscribed)
	{
		auto result = this->mProxy->zone5HvacStatus.SetReceiveHandler(std::bind(&Zone5HvacProxyImpl::cbZone5HvacStatus, this));
		if (result.HasValue())
		{
			result = this->mProxy->zone5HvacStatus.Subscribe(10);
			if (result.HasValue())
			{
				LOG_INFO() << "zone5HvacStatus subsciption succeeded\n";
			}
			else
			{
				LOG_ERROR() << "Failed to Subscribe for zone5HvacStatus\n";
			}
		}
		else
		{
			LOG_ERROR() << "Failed to SetReceiveHandler for zone5HvacStatus\n";
		}
	}
	else
	{
		LOG_ERROR() << "zone5HvacStatus.GetSubscriptionState() != kNotSubscribed\n";
	}

	// zone5HvacTemps
	if (this->mProxy->zone5HvacTemps.GetSubscriptionState() == ara::com::SubscriptionState::kNotSubscribed)
	{
		auto result = this->mProxy->zone5HvacTemps.SetReceiveHandler(std::bind(&Zone5HvacProxyImpl::cbZone5HvacTemps, this));
		if (result.HasValue())
		{
			result = this->mProxy->zone5HvacTemps.Subscribe(10);
			if (result.HasValue())
			{
				LOG_INFO() << "zone5HvacTemps subsciption succeeded\n";
			}
			else
			{
				LOG_ERROR() << "Failed to Subscribe for zone5HvacTemps\n";
			}
		}
		else
		{
			LOG_ERROR() << "Failed to SetReceiveHandler for zone5HvacTemps\n";
		}
	}
	else
	{
		LOG_ERROR() << "zone5HvacTemps.GetSubscriptionState() != kNotSubscribed\n";
	}

	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::UnsubscribeEvent()
{
	if (this->mProxy != nullptr)
	{
		// To Do:
	}
}

void Zone5HvacProxyImpl::UnsubscribeField()
{
	if (this->mProxy != nullptr)
	{
		// zone5HvacDeviceNormal
		this->mProxy->zone5HvacDeviceNormal.Unsubscribe();

		// zone5HvacSetting
		this->mProxy->zone5HvacSetting.Unsubscribe();

		// zone5HvacStatus
		this->mProxy->zone5HvacStatus.Unsubscribe();

		// zone5HvacTemps
		this->mProxy->zone5HvacTemps.Unsubscribe();
	}
}

void Zone5HvacProxyImpl::cbZone5HvacDeviceNormal()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone5HvacDeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone5HvacDeviceNormal.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone5HvacDeviceNormal.GetNewSamples([&](auto msg) {
		std::uint8_t fieldValue = static_cast<std::uint8_t>(*msg);   // fieldValue = *msg
		DeviceNormal_e isNormal = Convert_DeviceNormal_Zone2SOA(fieldValue);
		LOG_INFO() << "fieldValue=" << (int)fieldValue << " -> isNormal=" << getStringEnum_DeviceNormal_e(isNormal) << "\n";
		LOG_INFO() << "Listener Count = " << this->mListenerList.size() << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifyDeviceNormal(isNormal);
		}
	});

	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::cbZone5HvacSetting()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone5HvacSetting.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone5HvacSetting.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone5HvacSetting.GetNewSamples([&](auto msg) {
		zone5::hvac::control::EcuZ5HvacStatus z5Setting = static_cast<zone5::hvac::control::EcuZ5HvacStatus>(*msg);
		HvacStatus_t soaSetting;
		Convert_HvacStatus_Zone2SOA(z5Setting, soaSetting);

		//LOG_INFO() << "fieldValue=" << (int)fieldValue << " -> isNormal=" << getStringEnum_DeviceNormal_e(isNormal) << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifyHvacSetting(soaSetting);
		}
	});

	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::cbZone5HvacStatus()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone5HvacStatus.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone5HvacStatus.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone5HvacStatus.GetNewSamples([&](auto msg) {
		zone5::hvac::control::EcuZ5HvacStatus z5Status = static_cast<zone5::hvac::control::EcuZ5HvacStatus>(*msg);
		HvacStatus_t soaStatus;
		Convert_HvacStatus_Zone2SOA(z5Status, soaStatus);

		//LOG_INFO() << "fieldValue=" << (int)fieldValue << " -> isNormal=" << getStringEnum_DeviceNormal_e(isNormal) << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifyHvacStatus(soaStatus);
		}
	});

	LOG_INFO() << "(-)\n";
}

void Zone5HvacProxyImpl::cbZone5HvacTemps()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone5HvacTemps.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone5HvacTemps.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone5HvacTemps.GetNewSamples([&](auto msg) {
		zone5::hvac::control::EcuZ5HvacTemps z5Temps = static_cast<zone5::hvac::control::EcuZ5HvacTemps>(*msg);
		HvacTemps_t soaTemps;
		Convert_HvacTemps_Zone2SOA(z5Temps, soaTemps);
		//LOG_INFO() << "fieldValue=" << (int)fieldValue << " -> isNormal=" << getStringEnum_DeviceNormal_e(isNormal) << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifyHvacTemps(soaTemps);
		}
	});

	LOG_INFO() << "(-)\n";
}


}
}
}