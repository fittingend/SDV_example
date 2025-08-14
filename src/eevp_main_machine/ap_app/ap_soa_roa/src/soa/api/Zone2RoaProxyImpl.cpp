#include "Zone2RoaProxyImpl.hpp"
#include "thread"
#include "Log.hpp"

namespace zone2 {
namespace roa {
namespace control {

Zone2RoaProxyImpl *Zone2RoaProxyImpl::s_mInst = nullptr;

Zone2RoaProxyImpl *Zone2RoaProxyImpl::GetInstance()
{
	if (Zone2RoaProxyImpl::s_mInst == nullptr)
	{
		Zone2RoaProxyImpl::s_mInst = new Zone2RoaProxyImpl();
	}

	return Zone2RoaProxyImpl::s_mInst;
}

void Zone2RoaProxyImpl::DeleteInstance()
{
	if (Zone2RoaProxyImpl::s_mInst != nullptr)
	{
		delete Zone2RoaProxyImpl::s_mInst;
		Zone2RoaProxyImpl::s_mInst = nullptr;
	}
}

Zone2RoaProxyImpl::Zone2RoaProxyImpl()
{
	this->mServiceFound = false;
	this->mProxy = nullptr;
	this->mFindHandle = nullptr;
	this->mListenerList.clear();
}

Zone2RoaProxyImpl::~Zone2RoaProxyImpl()
{
	if (this->mProxy)
	{
		this->mProxy->StopFindService(*(this->mFindHandle));
		this->mProxy.reset();
	}

	this->mListenerList.clear();
}

void Zone2RoaProxyImpl::setEventListener(const std::shared_ptr<zone2::roa::control::IZone2RoaListener> _listener)
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

bool Zone2RoaProxyImpl::init()
{
	LOG_INFO() << "(+)\n";

	ara::core::InstanceSpecifier specifier("SOA_ROA/AA/RPort_Zone2Roa");

	auto callback = [&](auto container, auto findHandle) {
		FindServiceCallback(container, findHandle);
	};

	auto result = proxy::Zone2RoaProxy::StartFindService(callback, specifier);

	if (!result.HasValue())
	{
		LOG_INFO() << "Zone2RoaProxy StartFindService() Failed\n";
		return false;
	}

	for (int i = 0; i < 20; i++)
	{
		if (this->mServiceFound)
		{
			LOG_INFO() << "Zone2RoaProxy Start Find Service Success\n";
			return true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	LOG_INFO() << "(-)\n";
	return false;
}

void Zone2RoaProxyImpl::FindServiceCallback(
		ara::com::ServiceHandleContainer<zone2::roa::control::proxy::Zone2RoaProxy::HandleType> container,
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
			LOG_DEBUG() <<  "Zone2RoaProxyImpl::Find::Searched Instance::ServiceId =" <<
										handle.GetServiceHandle().serviceId <<
										", InstanceId =" <<
										handle.GetServiceHandle().instanceId << "\n";
		}

		this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
		this->mProxy = std::make_shared<proxy::Zone2RoaProxy>(container.at(0));

		LOG_INFO() << "Zone2RoaProxy Find-Service Success\n";
		SubscribeEvent();
		SubscribeField();
		this->mServiceFound = true;
	}

	LOG_INFO() << "(-)\n";
}

void Zone2RoaProxyImpl::controlRoa(const std::uint8_t& snsrRunReq)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->controlRoa(snsrRunReq);
	LOG_INFO() << "(-)\n";
}

bool Zone2RoaProxyImpl::getterSnsrStatus(zone2::roa::control::EcmRoaSnsrStatus& snsrStatus)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone2RoaSnsrStatus.Get();
	auto status = future.wait_for(std::chrono::milliseconds(250));
	if (status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			snsrStatus = static_cast<zone2::roa::control::EcmRoaSnsrStatus>(result.Value());
			return true;
		}
		else
		{
		   LOG_ERROR() << "Return error with\n";// << result.Error().Message();
		}
	}
	else
	{
	   LOG_ERROR() << "Timeout to call zone2RoaSnsrStatus's Getter\n";//
	}

	LOG_INFO() << "(-)\n";
	return false;
}

bool Zone2RoaProxyImpl::isServiceFound()
{
	return this->mServiceFound;
}

void Zone2RoaProxyImpl::SubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone2RoaProxyImpl::SubscribeField()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone2RoaSnsrStatus.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed)
	{
		LOG_ERROR() << "zone2RoaSnsrStatus.GetSubscriptionState() != kNotSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	auto result = this->mProxy->zone2RoaSnsrStatus.SetReceiveHandler(std::bind(&Zone2RoaProxyImpl::cbZone2SnsrStatus, this));
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to SetReceiveHandler for cbZone2SnsrStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	result = this->mProxy->zone2RoaSnsrStatus.Subscribe(10);
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to Subscribe for zone2RoaSnsrStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	LOG_INFO() << "(-)\n";
}

void Zone2RoaProxyImpl::UnsubscribeEvent()
{
	if (this->mProxy != nullptr)
	{
		// To Do:
	}
}

void Zone2RoaProxyImpl::UnsubscribeField()
{
	if (this->mProxy != nullptr)
	{
		this->mProxy->zone2RoaSnsrStatus.Unsubscribe();
	}
}

void Zone2RoaProxyImpl::cbZone2SnsrStatus()
{
	LOG_INFO() << "(+)\n";

	zone2::roa::control::EcmRoaSnsrStatus fieldValue;

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone2RoaSnsrStatus.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone2RoaSnsrStatus.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone2RoaSnsrStatus.GetNewSamples([&](auto msg) {
		fieldValue = static_cast<zone2::roa::control::EcmRoaSnsrStatus>(*msg);   // fieldValue = *msg
		LOG_INFO() << "cbZone2SnsrStatus : detInfo " << (int)fieldValue.detInfo << "\n";
		LOG_INFO() << "cbZone2SnsrStatus : snsrStat " << (int)fieldValue.snsrStat << "\n";
		LOG_INFO() << "cbZone2SnsrStatus : snsrErrState " << (int)fieldValue.snsrErrState << "\n";
		LOG_INFO() << "cbZone2SnsrStatus : isNormal " << (int)fieldValue.isNormal << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifySnsrStatus(fieldValue);
		}
	});

	LOG_INFO() << "(-)\n";
}

}
}
}