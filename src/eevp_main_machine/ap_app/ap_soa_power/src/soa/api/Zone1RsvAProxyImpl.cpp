#include "Zone1ReservedAProxyImpl.hpp"
#include "thread"
#include <Common.h>
#include "Log.hpp"

namespace zone1 {
namespace reserved {
namespace a {

Zone1ReservedAProxyImpl *Zone1ReservedAProxyImpl::s_mInst = nullptr;

Zone1ReservedAProxyImpl *Zone1ReservedAProxyImpl::GetInstance()
{
	if (Zone1ReservedAProxyImpl::s_mInst == nullptr)
	{
		Zone1ReservedAProxyImpl::s_mInst = new Zone1ReservedAProxyImpl();
	}

	return Zone1ReservedAProxyImpl::s_mInst;
}

void Zone1ReservedAProxyImpl::DeleteInstance()
{
	if (Zone1ReservedAProxyImpl::s_mInst != nullptr)
	{
		delete Zone1ReservedAProxyImpl::s_mInst;
		Zone1ReservedAProxyImpl::s_mInst = nullptr;
	}
}

Zone1ReservedAProxyImpl::Zone1ReservedAProxyImpl()
{
	this->mServiceFound = false;
	this->mProxy = nullptr;
	this->mFindHandle = nullptr;
	this->mListenerList.clear();
}

Zone1ReservedAProxyImpl::~Zone1ReservedAProxyImpl()
{
	if (this->mProxy)
	{
		this->mProxy->StopFindService(*(this->mFindHandle));
		this->mProxy.reset();
	}

	this->mListenerList.clear();
}

void Zone1ReservedAProxyImpl::setEventListener(const std::shared_ptr<zone1::reserved::a::IZone1ReservedAListener> _listener)
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

bool Zone1ReservedAProxyImpl::init()
{
	LOG_INFO() << "(+)\n";

	ara::core::InstanceSpecifier specifier("SOA_Power/AA/RPort_Zone1RsvA");

	auto callback = [&](auto container, auto findHandle) {
		FindServiceCallback(container, findHandle);
	};

	auto result = proxy::Zone1RsvAProxy::StartFindService(callback, specifier);

	if (!result.HasValue())
	{
		LOG_INFO() << "Zone1RsvAProxy StartFindService() Failed\n";
		return false;
	}

	for (int i = 0; i < 10; i++)
	{
		if (this->mServiceFound)
		{
			LOG_INFO() << "Zone1RsvAProxy Start Find Service Success\n";
			SubscribeEvent();
			SubscribeField();
			return true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	LOG_INFO() << "(-)\n";
	return false;
}

void Zone1ReservedAProxyImpl::FindServiceCallback(
		ara::com::ServiceHandleContainer<zone1::reserved::a::proxy::Zone1RsvAProxy::HandleType> container,
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
	}
	else
	{
		for (auto& handle : container)
		{
			LOG_ERROR() <<  "Zone1ReservedAProxyImpl::Find::Searched Instance::ServiceId =" <<
										handle.GetServiceHandle().serviceId <<
										", InstanceId =" <<
										handle.GetServiceHandle().instanceId << "\n";
		}

		this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
		this->mProxy = std::make_shared<proxy::Zone1RsvAProxy>(container.at(0));

		LOG_INFO() << "Zone1RsvAProxy Find-Service Success\n";
		SubscribeEvent();
		SubscribeField();
		this->mServiceFound = true;
	}

	LOG_INFO() << "(-)\n";
}

bool Zone1ReservedAProxyImpl::getterDeviceNormal(std::uint8_t &isNormal)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

  #if defined(HOTFIX_TEMP_001)
	isNormal = 0;
	return true;
  #else

	auto future = this->mProxy->zone1DeviceNormal.Get();
	auto status = future.wait_for(std::chrono::milliseconds(250));
	if (status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			isNormal = static_cast<std::uint8_t>(result.Value());
			return true;
		}
		else
		{
		   LOG_ERROR() << "Return error with\n";// << result.Error().Message();
		}
	}
	else
	{
	   LOG_ERROR() << "Timeout to call zone1DrvSeatMotorDir's Getter\n";//
	}

	LOG_INFO() << "(-)\n";
	return false;
#endif
}

bool Zone1ReservedAProxyImpl::isServiceFound()
{
	return this->mServiceFound;
}

void Zone1ReservedAProxyImpl::SubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone1ReservedAProxyImpl::SubscribeField()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone1DeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed)
	{
		LOG_ERROR() << "zone1DeviceNormal.GetSubscriptionState() != kNotSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	auto result = this->mProxy->zone1DeviceNormal.SetReceiveHandler(std::bind(&Zone1ReservedAProxyImpl::cbDeviceNormal, this));
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to zone1DeviceNormal for cbZone3RctnStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	result = this->mProxy->zone1DeviceNormal.Subscribe(10);
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to Subscribe for zone3RcurtainStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	LOG_INFO() << "(-)\n";
}

void Zone1ReservedAProxyImpl::UnsubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone1ReservedAProxyImpl::UnsubscribeField()
{
	LOG_INFO() << "(+)\n";
	if (this->mProxy != nullptr)
	{
		this->mProxy->zone1DeviceNormal.Unsubscribe();
	}
	LOG_INFO() << "(-)\n";
}

void Zone1ReservedAProxyImpl::cbDeviceNormal()
{
	LOG_INFO() << "(+)\n";

	std::uint8_t fieldValue;

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone1DeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone1DrvSeatMotorDir.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone1DeviceNormal.GetNewSamples([&](auto msg) {
		fieldValue = static_cast<std::uint8_t>(*msg);   // fieldValue = *msg
		LOG_INFO() << "cbDeviceNormal : isNormal " << (int)fieldValue << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifyDeviceNormal(fieldValue);
		}
	});

	LOG_INFO() << "(-)\n";
}

}
}
}