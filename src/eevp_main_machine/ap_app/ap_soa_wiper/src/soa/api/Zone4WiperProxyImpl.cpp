#include "Zone4WiperProxyImpl.hpp"
#include "thread"
#include "Log.hpp"

namespace zone4 {
namespace wiper {
namespace control {

Zone4WiperProxyImpl *Zone4WiperProxyImpl::s_mInst = nullptr;

Zone4WiperProxyImpl *Zone4WiperProxyImpl::GetInstance()
{
	if (Zone4WiperProxyImpl::s_mInst == nullptr)
	{
		Zone4WiperProxyImpl::s_mInst = new Zone4WiperProxyImpl();
	}

	return Zone4WiperProxyImpl::s_mInst;
}

void Zone4WiperProxyImpl::DeleteInstance()
{
	if (Zone4WiperProxyImpl::s_mInst != nullptr)
	{
		delete Zone4WiperProxyImpl::s_mInst;
		Zone4WiperProxyImpl::s_mInst = nullptr;
	}
}

Zone4WiperProxyImpl::Zone4WiperProxyImpl()
{
	this->mServiceFound = false;
	this->mProxy = nullptr;
	this->mFindHandle = nullptr;
	this->mListenerList.clear();
}

Zone4WiperProxyImpl::~Zone4WiperProxyImpl()
{
	if (this->mProxy)
	{
		this->mProxy->StopFindService(*(this->mFindHandle));
		this->mProxy.reset();
	}

	this->mListenerList.clear();
}

void Zone4WiperProxyImpl::setEventListener(const std::shared_ptr<zone4::wiper::control::IZone4WiperListener> _listener)
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

bool Zone4WiperProxyImpl::init()
{
	LOG_INFO() << "(+)\n";

	ara::core::InstanceSpecifier specifier("SOA_Wiper/AA/RPort_Zone4Wiper");

	auto callback = [&](auto container, auto findHandle) {
		FindServiceCallback(container, findHandle);
	};

	auto result = proxy::Zone4WiperProxy::StartFindService(callback, specifier);

	if (!result.HasValue())
	{
		LOG_INFO() << "Zone4WiperProxy StartFindService() Failed\n";
		return false;
	}

	for (int i = 0; i < 20; i++)
	{
		if (this->mServiceFound)
		{
			LOG_INFO() << "Zone4WiperProxy Start Find Service Success\n";
			SubscribeEvent();
			SubscribeField();
			return true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	LOG_INFO() << "(-)\n";
	return false;
}

void Zone4WiperProxyImpl::FindServiceCallback(
		ara::com::ServiceHandleContainer<zone4::wiper::control::proxy::Zone4WiperProxy::HandleType> container,
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
			LOG_DEBUG() <<  "Zone4WiperProxyImpl::Find::Searched Instance::ServiceId =" <<
										handle.GetServiceHandle().serviceId <<
										", InstanceId =" <<
										handle.GetServiceHandle().instanceId << "\n";
		}

		this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
		this->mProxy = std::make_shared<proxy::Zone4WiperProxy>(container.at(0));

		LOG_INFO() << "Zone4WiperProxy Find-Service Success\n";
		SubscribeEvent();
		SubscribeField();
		this->mServiceFound = true;
	}

	LOG_INFO() << "(-)\n";
}

void Zone4WiperProxyImpl::controlWiper(const std::uint8_t& mode)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->controlWiper(mode);
	LOG_INFO() << "(-)\n";
}

bool Zone4WiperProxyImpl::getterWiperOpStatus(zone4::wiper::control::EcuWiperOperationStatus& opStatus)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone4WiperOpStatus.Get();
	auto status = future.wait_for(std::chrono::milliseconds(250));
	if (status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			opStatus = static_cast<zone4::wiper::control::EcuWiperOperationStatus>(result.Value());
			return true;
		}
		else
		{
		   LOG_ERROR() << "Return error with\n";// << result.Error().Message();
		}
	}
	else
	{
	   LOG_ERROR() << "Timeout to call zone4WiperMotorDir's Getter\n";//
	}

	LOG_INFO() << "(-)\n";
	return false;
}

bool Zone4WiperProxyImpl::isServiceFound()
{
	return this->mServiceFound;
}

void Zone4WiperProxyImpl::SubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone4WiperProxyImpl::SubscribeField()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone4WiperOpStatus.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed)
	{
		LOG_ERROR() << "zone4WiperOpStatus.GetSubscriptionState() != kNotSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	auto result = this->mProxy->zone4WiperOpStatus.SetReceiveHandler(std::bind(&Zone4WiperProxyImpl::cbZone4WiperOpStatus, this));
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to SetReceiveHandler for cbZone3RctnStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	result = this->mProxy->zone4WiperOpStatus.Subscribe(10);
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to Subscribe for zone3RcurtainStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	LOG_INFO() << "(-)\n";
}

void Zone4WiperProxyImpl::UnsubscribeEvent()
{
	if (this->mProxy != nullptr)
	{
		// To Do:
	}
}

void Zone4WiperProxyImpl::UnsubscribeField()
{
	if (this->mProxy != nullptr)
	{
		this->mProxy->zone4WiperOpStatus.Unsubscribe();
	}
}

void Zone4WiperProxyImpl::cbZone4WiperOpStatus()
{
	LOG_INFO() << "(+)\n";

	zone4::wiper::control::EcuWiperOperationStatus fieldValue;

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone4WiperOpStatus.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone4WiperOpStatus.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone4WiperOpStatus.GetNewSamples([&](auto msg) {
		fieldValue = static_cast<zone4::wiper::control::EcuWiperOperationStatus>(*msg);   // fieldValue = *msg
		LOG_INFO() << "cbZone4MotorDir : curOperationMode " << (int)fieldValue.curOperationMode << "\n";
		LOG_INFO() << "cbZone4MotorDir : isNormal " << (int)fieldValue.isNormal << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifyWiperOpStatus(fieldValue);
		}
	});

	LOG_INFO() << "(-)\n";
}

}
}
}