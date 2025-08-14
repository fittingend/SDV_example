#include "Zone2ReservedAProxyImpl.hpp"
#include "thread"
#include "Log.hpp"

namespace zone2 {
namespace reserved {
namespace a {

Zone2ReservedAProxyImpl *Zone2ReservedAProxyImpl::s_mInst = nullptr;

Zone2ReservedAProxyImpl *Zone2ReservedAProxyImpl::GetInstance()
{
	if (Zone2ReservedAProxyImpl::s_mInst == nullptr)
	{
		Zone2ReservedAProxyImpl::s_mInst = new Zone2ReservedAProxyImpl();
	}

	return Zone2ReservedAProxyImpl::s_mInst;
}

void Zone2ReservedAProxyImpl::DeleteInstance()
{
	if (Zone2ReservedAProxyImpl::s_mInst != nullptr)
	{
		delete Zone2ReservedAProxyImpl::s_mInst;
		Zone2ReservedAProxyImpl::s_mInst = nullptr;
	}
}

Zone2ReservedAProxyImpl::Zone2ReservedAProxyImpl()
{
	this->mServiceFound = false;
	this->mProxy = nullptr;
	this->mFindHandle = nullptr;
	this->mListenerList.clear();
}

Zone2ReservedAProxyImpl::~Zone2ReservedAProxyImpl()
{
	if (this->mProxy)
	{
		this->mProxy->StopFindService(*(this->mFindHandle));
		this->mProxy.reset();
	}

	this->mListenerList.clear();
}

void Zone2ReservedAProxyImpl::setEventListener(const std::shared_ptr<zone2::reserved::a::IZone2ReservedAListener> _listener)
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

bool Zone2ReservedAProxyImpl::init()
{
	LOG_INFO() << "(+)\n";

	ara::core::InstanceSpecifier specifier("SOA_Power/AA/RPort_Zone2RsvA");

	auto callback = [&](auto container, auto findHandle) {
		FindServiceCallback(container, findHandle);
	};

	auto result = proxy::Zone2RsvAProxy::StartFindService(callback, specifier);

	if (!result.HasValue())
	{
		LOG_INFO() << "Zone2RsvAProxy StartFindService() Failed\n";
		return false;
	}

	for (int i = 0; i < 10; i++)
	{
		if (this->mServiceFound)
		{
			LOG_INFO() << "Zone2RsvAProxy Start Find Service Success\n";
			SubscribeEvent();
			SubscribeField();
			return true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	LOG_INFO() << "(-)\n";
	return false;
}

void Zone2ReservedAProxyImpl::FindServiceCallback(
		ara::com::ServiceHandleContainer<zone2::reserved::a::proxy::Zone2RsvAProxy::HandleType> container,
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
			LOG_DEBUG() <<  "Zone2ReservedAProxy::Find::Searched Instance::ServiceId =" <<
										handle.GetServiceHandle().serviceId <<
										", InstanceId =" <<
										handle.GetServiceHandle().instanceId << "\n";
		}

		this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
		this->mProxy = std::make_shared<proxy::Zone2RsvAProxy>(container.at(0));

		LOG_INFO() << "Zone2ReservedAProxy Find-Service Success\n";
		SubscribeEvent();
		SubscribeField();
		this->mServiceFound = true;
	}

	LOG_INFO() << "(-)\n";
}

bool Zone2ReservedAProxyImpl::getterDeviceNormal(std::uint8_t &isNormal)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone2DeviceNormal.Get();
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
	   LOG_ERROR() << "Timeout to call zone2DrvSeatMotorDir's Getter\n";//
	}

	LOG_INFO() << "(-)\n";
	return false;
}

bool Zone2ReservedAProxyImpl::isServiceFound()
{
	return this->mServiceFound;
}

void Zone2ReservedAProxyImpl::SubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone2ReservedAProxyImpl::SubscribeField()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone2DeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed)
	{
		LOG_ERROR() << "zone2DeviceNormal.GetSubscriptionState() != kNotSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	auto result = this->mProxy->zone2DeviceNormal.SetReceiveHandler(std::bind(&Zone2ReservedAProxyImpl::cbDeviceNormal, this));
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to zone2DeviceNormal for cbZone3RctnStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	result = this->mProxy->zone2DeviceNormal.Subscribe(10);
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to Subscribe for zone3RcurtainStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	LOG_INFO() << "(-)\n";
}

void Zone2ReservedAProxyImpl::UnsubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone2ReservedAProxyImpl::UnsubscribeField()
{
	LOG_INFO() << "(+)\n";
	if (this->mProxy != nullptr)
	{
		this->mProxy->zone2DeviceNormal.Unsubscribe();
	}
	LOG_INFO() << "(-)\n";
}

void Zone2ReservedAProxyImpl::cbDeviceNormal()
{
	LOG_INFO() << "(+)\n";

	std::uint8_t fieldValue;

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone2DeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone2DrvSeatMotorDir.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone2DeviceNormal.GetNewSamples([&](auto msg) {
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