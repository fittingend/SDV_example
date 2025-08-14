#include "Zone3RcurtainProxyImpl.hpp"
#include "thread"
#include "Log.hpp"

namespace zone3 {
namespace rcurtain {
namespace control {

Zone3RearCurtainProxyImpl *Zone3RearCurtainProxyImpl::s_mInst = nullptr;

Zone3RearCurtainProxyImpl *Zone3RearCurtainProxyImpl::GetInstance()
{
	if (Zone3RearCurtainProxyImpl::s_mInst == nullptr)
	{
		Zone3RearCurtainProxyImpl::s_mInst = new Zone3RearCurtainProxyImpl();
	}

	return Zone3RearCurtainProxyImpl::s_mInst;
}

void Zone3RearCurtainProxyImpl::DeleteInstance()
{
	if (Zone3RearCurtainProxyImpl::s_mInst != nullptr)
	{
		delete Zone3RearCurtainProxyImpl::s_mInst;
		Zone3RearCurtainProxyImpl::s_mInst = nullptr;
	}
}

Zone3RearCurtainProxyImpl::Zone3RearCurtainProxyImpl()
{
	this->mServiceFound = false;
	this->mProxy = nullptr;
	this->mFindHandle = nullptr;
	this->mListenerList.clear();
}

Zone3RearCurtainProxyImpl::~Zone3RearCurtainProxyImpl()
{
	if (this->mProxy)
	{
		this->mProxy->StopFindService(*(this->mFindHandle));
		this->mProxy.reset();
	}
	this->mListenerList.clear();
}

void Zone3RearCurtainProxyImpl::setEventListener(const std::shared_ptr<zone3::rcurtain::control::IZone3RearCurtainListener> _listener)
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

bool Zone3RearCurtainProxyImpl::init()
{
	LOG_INFO() << "(+)\n";

	ara::core::InstanceSpecifier specifier("SOA_RearCurtain/AA/RPort_Zone3Rcurtain");

	auto callback = [&](auto container, auto findHandle) {
		FindServiceCallback(container, findHandle);
	};

	auto result = proxy::Zone3RcurtainProxy::StartFindService(callback, specifier);

	if (!result.HasValue())
	{
		LOG_INFO() << "Zone3RearCurtainProxy StartFindService() Failed\n";
		return false;
	}

	for (int i = 0; i < 20; i++)
	{
		if (this->mServiceFound)
		{
			LOG_INFO() << "Zone3RearCurtainProxy Start Find Service Success\n";
			return true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	LOG_INFO() << "(-)\n";
	return false;
}

void Zone3RearCurtainProxyImpl::FindServiceCallback(
		ara::com::ServiceHandleContainer<zone3::rcurtain::control::proxy::Zone3RcurtainProxy::HandleType> container,
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
			LOG_DEBUG() <<  "Zone3RearCurtainProxy::Find::Searched Instance::ServiceId =" <<
										handle.GetServiceHandle().serviceId <<
										", InstanceId =" <<
										handle.GetServiceHandle().instanceId << "\n";
		}

		this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
		this->mProxy = std::make_shared<proxy::Zone3RcurtainProxy>(container.at(0));

		LOG_INFO() << "Zone3RearCurtainProxy Find-Service Success\n";
		SubscribeEvent();
		SubscribeField();
		this->mServiceFound = true;
	}

	LOG_INFO() << "(-)\n";
}

void Zone3RearCurtainProxyImpl::controlMotor(const uint8_t & motorDir)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->controlRcurtain(motorDir);
	LOG_INFO() << "(-)\n";
}

bool Zone3RearCurtainProxyImpl::getterRcurtainStatus(zone3::rcurtain::control::EcuRcurtainStatus& rctnStatus)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone3RcurtainStatus.Get();
	auto status = future.wait_for(std::chrono::milliseconds(250));
	if (status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			rctnStatus = static_cast<zone3::rcurtain::control::EcuRcurtainStatus>(result.Value());
			return true;
		}
		else
		{
		   LOG_ERROR() << "Return error with\n";// << result.Error().Message();
		}
	}
	else
	{
	   LOG_ERROR() << "Timeout to call soaRctnStatus's Getter\n";//
	}

	LOG_INFO() << "(-)\n";
	return false;
}

bool Zone3RearCurtainProxyImpl::isServiceFound()
{
	return this->mServiceFound;
}

void Zone3RearCurtainProxyImpl::SubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone3RearCurtainProxyImpl::SubscribeField()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone3RcurtainStatus.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed)
	{
		LOG_ERROR() << "zone3RcurtainStatus.GetSubscriptionState() != kNotSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	auto result = this->mProxy->zone3RcurtainStatus.SetReceiveHandler(std::bind(&Zone3RearCurtainProxyImpl::cbZone3RctnStatus, this));
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to SetReceiveHandler for cbZone3RctnStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	result = this->mProxy->zone3RcurtainStatus.Subscribe(10);
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to Subscribe for zone3RcurtainStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	LOG_INFO() << "(-)\n";
}

void Zone3RearCurtainProxyImpl::UnsubscribeEvent()
{
	if (this->mProxy != nullptr)
	{
		// To Do:
	}
}

void Zone3RearCurtainProxyImpl::UnsubscribeField()
{
	if (this->mProxy != nullptr)
	{
		this->mProxy->zone3RcurtainStatus.Unsubscribe();
	}
}


void Zone3RearCurtainProxyImpl::cbZone3RctnStatus()
{
	LOG_INFO() << "(+)\n";

	zone3::rcurtain::control::EcuRcurtainStatus fieldValue;

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone3RcurtainStatus.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "rcurtainStat.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone3RcurtainStatus.GetNewSamples([&](auto msg) {
		fieldValue = static_cast<zone3::rcurtain::control::EcuRcurtainStatus>(*msg);   // fieldValue = *msg
		LOG_INFO() << "cbZone3RctnStatus : curMotorDir " << (int)fieldValue.curMotorDir << "\n";
		LOG_INFO() << "cbZone3RctnStatus : curMotorLimit " << (int)fieldValue.curMotorLimit << "\n";
		LOG_INFO() << "cbZone3RctnStatus : motorCurrent " << (int)fieldValue.motorCurrent << "\n";
		LOG_INFO() << "cbZone3RctnStatus : isNormal " << (int)fieldValue.isNormal << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifyRcurtainStatus(fieldValue);
		}
	});

	LOG_INFO() << "(-)\n";
}


}
}
}