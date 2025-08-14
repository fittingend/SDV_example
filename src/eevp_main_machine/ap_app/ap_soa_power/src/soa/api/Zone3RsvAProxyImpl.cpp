#include "Zone3ReservedAProxyImpl.hpp"
#include "thread"
#include "Log.hpp"

namespace zone3 {
namespace reserved {
namespace a {

Zone3ReservedAProxyImpl *Zone3ReservedAProxyImpl::s_mInst = nullptr;

Zone3ReservedAProxyImpl *Zone3ReservedAProxyImpl::GetInstance()
{
	if (Zone3ReservedAProxyImpl::s_mInst == nullptr)
	{
		Zone3ReservedAProxyImpl::s_mInst = new Zone3ReservedAProxyImpl();
	}

	return Zone3ReservedAProxyImpl::s_mInst;
}

void Zone3ReservedAProxyImpl::DeleteInstance()
{
	if (Zone3ReservedAProxyImpl::s_mInst != nullptr)
	{
		delete Zone3ReservedAProxyImpl::s_mInst;
		Zone3ReservedAProxyImpl::s_mInst = nullptr;
	}
}

Zone3ReservedAProxyImpl::Zone3ReservedAProxyImpl()
{
	this->mServiceFound = false;
	this->mProxy = nullptr;
	this->mFindHandle = nullptr;
	this->mListenerList.clear();
}

Zone3ReservedAProxyImpl::~Zone3ReservedAProxyImpl()
{
	if (this->mProxy)
	{
		this->mProxy->StopFindService(*(this->mFindHandle));
		this->mProxy.reset();
	}

	this->mListenerList.clear();
}

void Zone3ReservedAProxyImpl::setEventListener(const std::shared_ptr<zone3::reserved::a::IZone3ReservedAListener> _listener)
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

bool Zone3ReservedAProxyImpl::init()
{
	LOG_INFO() << "(+)\n";

	ara::core::InstanceSpecifier specifier("SOA_Power/AA/RPort_Zone3RsvA");

	auto callback = [&](auto container, auto findHandle) {
		FindServiceCallback(container, findHandle);
	};

	auto result = proxy::Zone3RsvAProxy::StartFindService(callback, specifier);

	if (!result.HasValue())
	{
		LOG_INFO() << "Zone3RsvAProxy StartFindService() Failed\n";
		return false;
	}

	for (int i = 0; i < 10; i++)
	{
		if (this->mServiceFound)
		{
			LOG_INFO() << "Zone3RsvAProxy Start Find Service Success\n";
			SubscribeEvent();
			SubscribeField();
			return true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	LOG_INFO() << "(-)\n";
	return false;
}

void Zone3ReservedAProxyImpl::FindServiceCallback(
		ara::com::ServiceHandleContainer<zone3::reserved::a::proxy::Zone3RsvAProxy::HandleType> container,
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
			LOG_DEBUG() <<  "Zone3ReservedAProxy::Find::Searched Instance::ServiceId =" <<
										handle.GetServiceHandle().serviceId <<
										", InstanceId =" <<
										handle.GetServiceHandle().instanceId << "\n";
		}

		this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
		this->mProxy = std::make_shared<proxy::Zone3RsvAProxy>(container.at(0));

		LOG_INFO() << "Zone3ReservedAProxy Find-Service Success\n";
		SubscribeEvent();
		SubscribeField();
		this->mServiceFound = true;
	}

	LOG_INFO() << "(-)\n";
}

bool Zone3ReservedAProxyImpl::getterDeviceNormal(std::uint8_t &isNormal)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone3DeviceNormal.Get();
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
	   LOG_ERROR() << "Timeout to call zone3DrvSeatMotorDir's Getter\n";//
	}

	LOG_INFO() << "(-)\n";
	return false;
}

bool Zone3ReservedAProxyImpl::isServiceFound()
{
	return this->mServiceFound;
}

void Zone3ReservedAProxyImpl::SubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone3ReservedAProxyImpl::SubscribeField()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone3DeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed)
	{
		LOG_ERROR() << "zone3DeviceNormal.GetSubscriptionState() != kNotSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	auto result = this->mProxy->zone3DeviceNormal.SetReceiveHandler(std::bind(&Zone3ReservedAProxyImpl::cbDeviceNormal, this));
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to zone3DeviceNormal for cbZone3RctnStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	result = this->mProxy->zone3DeviceNormal.Subscribe(10);
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to Subscribe for zone3RcurtainStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	LOG_INFO() << "(-)\n";
}

void Zone3ReservedAProxyImpl::UnsubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone3ReservedAProxyImpl::UnsubscribeField()
{
	LOG_INFO() << "(+)\n";
	if (this->mProxy != nullptr)
	{
		this->mProxy->zone3DeviceNormal.Unsubscribe();
	}
	LOG_INFO() << "(-)\n";
}

void Zone3ReservedAProxyImpl::cbDeviceNormal()
{
	LOG_INFO() << "(+)\n";

	std::uint8_t fieldValue;

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone3DeviceNormal.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone3DrvSeatMotorDir.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone3DeviceNormal.GetNewSamples([&](auto msg) {
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