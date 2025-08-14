#include <Common.h>
#include <SoaDataType.h>
#include "eevp/control/impl_type_soafilmopacityarray.h"
#include "Zone5SmartFilmProxyImpl.hpp"
#include "thread"
#include <Zone5SmartFilmConverter.hpp>
#include <Log.hpp>
#include <Debug_DataType2String.hpp>

namespace zone5
{
namespace smtfilm
{
namespace control
{

Zone5SmartFilmProxyImpl *Zone5SmartFilmProxyImpl::s_mInst = nullptr;

Zone5SmartFilmProxyImpl *Zone5SmartFilmProxyImpl::GetInstance()
{
	if (Zone5SmartFilmProxyImpl::s_mInst == nullptr)
	{
		Zone5SmartFilmProxyImpl::s_mInst = new Zone5SmartFilmProxyImpl();
	}

	return Zone5SmartFilmProxyImpl::s_mInst;
}

void Zone5SmartFilmProxyImpl::DeleteInstance()
{
	if (Zone5SmartFilmProxyImpl::s_mInst != nullptr)
	{
		delete Zone5SmartFilmProxyImpl::s_mInst;
		Zone5SmartFilmProxyImpl::s_mInst = nullptr;
	}
}

Zone5SmartFilmProxyImpl::Zone5SmartFilmProxyImpl()
{
	this->mServiceFound = false;
	this->mProxy = nullptr;
	this->mFindHandle = nullptr;
	this->mListenerList.clear();
}

Zone5SmartFilmProxyImpl::~Zone5SmartFilmProxyImpl()
{
	if (this->mProxy)
	{
		this->mProxy->StopFindService(*(this->mFindHandle));
		this->mProxy.reset();
	}

	this->mListenerList.clear();
}

void Zone5SmartFilmProxyImpl::setEventListener(const std::shared_ptr<zone5::smtfilm::control::IZone5SmartFilmListener> _listener)
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

bool Zone5SmartFilmProxyImpl::init()
{
	LOG_INFO() << "(+)\n";

	ara::core::InstanceSpecifier specifier("SOA_SmartFilm/AA/RPort_Zone5_SmtFilm");

	auto callback = [&](auto container, auto findHandle) {
		FindServiceCallback(container, findHandle);
	};

	auto result = proxy::Zone5SmtFilmProxy::StartFindService(callback, specifier);

	if (!result.HasValue())
	{
		LOG_INFO() << "Zone5SmtFilmProxy StartFindService() Failed\n";
		return false;
	}

#if 0
	for (int i = 0; i < 20; i++)
	{
		if (this->mServiceFound)
		{
			LOG_INFO() << "Zone5SmtFilmProxy Start Find Service Success\n";
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

void Zone5SmartFilmProxyImpl::FindServiceCallback(
		ara::com::ServiceHandleContainer<zone5::smtfilm::control::proxy::Zone5SmtFilmProxy::HandleType> container,
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
			LOG_DEBUG() <<  "Zone5SmartFilmProxyImpl::Find::Searched Instance::ServiceId =" <<
										handle.GetServiceHandle().serviceId <<
										", InstanceId =" <<
										handle.GetServiceHandle().instanceId << "\n";
		}

		this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
		this->mProxy = std::make_shared<proxy::Zone5SmtFilmProxy>(container.at(0));

		LOG_INFO() << "Zone5SmtFilmProxy Find-Service Success\n";
		SubscribeEvent();
		SubscribeField();
		this->mServiceFound = true;
	}

	LOG_INFO() << "(-)\n";
}

void Zone5SmartFilmProxyImpl::SetOpacity(SmtfilmPos_e pos, int opacity)
{
	LOG_DEBUG() << "(+) pos=" << getStringEnum_SmtfilmPos_e(pos) << ", opacity=" << opacity << "\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->SetOpacity(Convert_SmtfilmPos_SOA2Zone(pos), (std::uint8_t)opacity);
	LOG_INFO() << "(-)\n";
}

void Zone5SmartFilmProxyImpl::SetAllOpacity(int opacity)
{
	LOG_DEBUG() << "(+) opacity=" << opacity << "\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->SetAllOpacity((std::uint8_t)opacity);
	LOG_INFO() << "(-)\n";
}


bool Zone5SmartFilmProxyImpl::getterDeviceNormal(DeviceNormal_e &isNormal)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone5SmtFilmDeviceNormal.Get();
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
	   LOG_ERROR() << "Timeout to call zone5SmtFilmDeviceNormal Getter\n";//
	}

	LOG_INFO() << "(-)\n";
	return false;
}

bool Zone5SmartFilmProxyImpl::getterSmtfilmOpacities(SmtfilmOpacities_t &opacities)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone5SmtFilmOpacities.Get();
	auto read_status = future.wait_for(std::chrono::milliseconds(250));
	if (read_status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			eevp::control::SoaFilmOpacityArray ret_val = static_cast<eevp::control::SoaFilmOpacityArray>(result.Value());
			Convert_SmtfilmOpacity_Zone2SOA(ret_val, opacities);
			return true;
		}
		else
		{
		   LOG_ERROR() << "Return error with " << result.Error().Message() << "\n";
		}
	}
	else
	{
	   LOG_ERROR() << "Timeout to call zone5SmtFilmOpacities Getter\n";//
	}

	LOG_INFO() << "(-)\n";
	return false;
}

bool Zone5SmartFilmProxyImpl::isServiceFound()
{
	return this->mServiceFound;
}

void Zone5SmartFilmProxyImpl::SubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone5SmartFilmProxyImpl::SubscribeField()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	// Device Normal
	{
		if (this->mProxy->zone5SmtFilmDeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed)
		{
			LOG_ERROR() << "zone5SmtFilmDeviceNormal.GetSubscriptionState() != kNotSubscribed\n";
			LOG_INFO() << "(-)\n";
			return;
		}

		auto result = this->mProxy->zone5SmtFilmDeviceNormal.SetReceiveHandler(std::bind(&Zone5SmartFilmProxyImpl::cbZone5SmartFilmDeviceNormal, this));
		if (!result.HasValue())
		{
			LOG_ERROR() << "Failed to SetReceiveHandler for zone5SmtFilmDeviceNormal\n";
			LOG_INFO() << "(-)\n";
			return;
		}

		result = this->mProxy->zone5SmtFilmDeviceNormal.Subscribe(10);
		if (!result.HasValue())
		{
			LOG_ERROR() << "Failed to Subscribe for zone5SmtFilmDeviceNormal\n";
			LOG_INFO() << "(-)\n";
			return;
		}
	}

	// Opacities
	{
		if (this->mProxy->zone5SmtFilmOpacities.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed)
		{
			LOG_ERROR() << "zone5SmtFilmOpacities.GetSubscriptionState() != kNotSubscribed\n";
			LOG_INFO() << "(-)\n";
			return;
		}

		auto result = this->mProxy->zone5SmtFilmOpacities.SetReceiveHandler(std::bind(&Zone5SmartFilmProxyImpl::cbZone5SmartFilmOpacities, this));
		if (!result.HasValue())
		{
			LOG_ERROR() << "Failed to SetReceiveHandler for zone5SmtFilmOpacities\n";
			LOG_INFO() << "(-)\n";
			return;
		}

		result = this->mProxy->zone5SmtFilmOpacities.Subscribe(10);
		if (!result.HasValue())
		{
			LOG_ERROR() << "Failed to Subscribe for zone5SmtFilmOpacities\n";
			LOG_INFO() << "(-)\n";
			return;
		}
	}

	LOG_INFO() << "(-)\n";
}

void Zone5SmartFilmProxyImpl::UnsubscribeEvent()
{
	if (this->mProxy != nullptr)
	{
		// To Do:
	}
}

void Zone5SmartFilmProxyImpl::UnsubscribeField()
{
	if (this->mProxy != nullptr)
	{
		this->mProxy->zone5SmtFilmDeviceNormal.Unsubscribe();
	}
}

void Zone5SmartFilmProxyImpl::cbZone5SmartFilmDeviceNormal()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone5SmtFilmDeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone5SmtFilmDeviceNormal.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone5SmtFilmDeviceNormal.GetNewSamples([&](auto msg) {
		std::uint8_t fieldValue = static_cast<std::uint8_t>(*msg);   // fieldValue = *msg
		DeviceNormal_e isNormal = Convert_DeviceNormal_Zone2SOA(fieldValue);
		LOG_INFO() << "fieldValue=" << (int)fieldValue << " -> isNormal=" << getStringEnum_DeviceNormal_e(isNormal) << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifyDeviceNormal(isNormal);
		}
	});

	LOG_INFO() << "(-)\n";
}

void Zone5SmartFilmProxyImpl::cbZone5SmartFilmOpacities()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone5SmtFilmOpacities.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone5SmtFilmOpacities.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone5SmtFilmOpacities.GetNewSamples([&](auto msg) {
		eevp::control::SoaFilmOpacityArray z5Opacities = static_cast<eevp::control::SoaFilmOpacityArray>(*msg);
		SmtfilmOpacities_t soaOpacities;
		Convert_SmtfilmOpacity_Zone2SOA(z5Opacities, soaOpacities);

		//LOG_INFO() << "fieldValue=" << (int)fieldValue << " -> isNormal=" << getStringEnum_DeviceNormal_e(isNormal) << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifyOpacities(soaOpacities);
		}
	});

	LOG_INFO() << "(-)\n";
}

}
}
}