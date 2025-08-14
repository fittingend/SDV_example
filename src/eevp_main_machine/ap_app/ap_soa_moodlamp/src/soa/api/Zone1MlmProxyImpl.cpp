#include "Zone1MlmProxyImpl.hpp"
#include "thread"
#include "Log.hpp"
#include <Common.h>

namespace zone1 {
namespace mlm {
namespace control {

Zone1MlmProxyImpl *Zone1MlmProxyImpl::s_mInst = nullptr;

Zone1MlmProxyImpl *Zone1MlmProxyImpl::GetInstance()
{
	if (Zone1MlmProxyImpl::s_mInst == nullptr)
	{
		Zone1MlmProxyImpl::s_mInst = new Zone1MlmProxyImpl();
	}

	return Zone1MlmProxyImpl::s_mInst;
}

void Zone1MlmProxyImpl::DeleteInstance()
{
	if (Zone1MlmProxyImpl::s_mInst != nullptr)
	{
		delete Zone1MlmProxyImpl::s_mInst;
		Zone1MlmProxyImpl::s_mInst = nullptr;
	}
}

Zone1MlmProxyImpl::Zone1MlmProxyImpl()
{
	this->mServiceFound = false;
	this->mProxy = nullptr;
	this->mFindHandle = nullptr;
	this->mListenerList.clear();
	//this->mListener = nullptr;
}

Zone1MlmProxyImpl::~Zone1MlmProxyImpl()
{
	if (this->mProxy)
	{
		this->mProxy->StopFindService(*(this->mFindHandle));
		this->mProxy.reset();
	}

	this->mListenerList.clear();
}

void Zone1MlmProxyImpl::setEventListener(const std::shared_ptr<zone1::mlm::control::IZone1MlmListener> _listener)
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

bool Zone1MlmProxyImpl::init()
{
	LOG_INFO() << "(+)\n";

	ara::core::InstanceSpecifier specifier("SOA_MoodLamp/AA/RPort_Zone1Mlm");

	auto callback = [&](auto container, auto findHandle) {
		FindServiceCallback(container, findHandle);
	};

	auto result = proxy::Zone1MlmProxy::StartFindService(callback, specifier);

	if (!result.HasValue())
	{
		LOG_INFO() << "Zone1MlmProxy StartFindService() Failed\n";
		return false;
	}

	for (int i = 0; i < 20; i++)
	{
		if (this->mServiceFound)
		{
			LOG_INFO() << "Zone1MlmProxy Start Find Service Success\n";
			return true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	LOG_INFO() << "(-)\n";
	return false;
}

void Zone1MlmProxyImpl::FindServiceCallback(
		ara::com::ServiceHandleContainer<zone1::mlm::control::proxy::Zone1MlmProxy::HandleType> container,
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
			LOG_ERROR() <<  "Zone1MlmProxyImpl::Find::Searched Instance::ServiceId =" <<
										handle.GetServiceHandle().serviceId <<
										", InstanceId =" <<
										handle.GetServiceHandle().instanceId << "\n";
		}

		this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
		this->mProxy = std::make_shared<proxy::Zone1MlmProxy>(container.at(0));

		LOG_INFO() << "Zone1MlmProxy Find-Service Success\n";
		SubscribeEvent();
		SubscribeField();
		this->mServiceFound = true;
	}

	LOG_INFO() << "(-)\n";
}

void Zone1MlmProxyImpl::setMlmBrightness(const std::uint8_t& brightness)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->setMlmBrightness(brightness);
	LOG_INFO() << "(-)\n";
}

void Zone1MlmProxyImpl::setMlmColor(const std::uint8_t& colorIndex)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->setMlmColor(colorIndex);
	LOG_INFO() << "(-)\n";
}

bool Zone1MlmProxyImpl::getterMlmStatus(zone1::mlm::control::EcuMlmState & state)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

#if defined(HOTFIX_TEMP_001)
	state.isNormal = 0;
	return true;
#else

	auto future = this->mProxy->zone1MlmStatus.Get();
	auto status = future.wait_for(std::chrono::milliseconds(250));
	if (status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			state = static_cast<zone1::mlm::control::EcuMlmState>(result.Value());
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

bool Zone1MlmProxyImpl::isServiceFound()
{
	return this->mServiceFound;
}

void Zone1MlmProxyImpl::SubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone1MlmProxyImpl::SubscribeField()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone1MlmStatus.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed)
	{
		LOG_ERROR() << "zone1DrvSeatMotorDir.GetSubscriptionState() != kNotSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	auto result = this->mProxy->zone1MlmStatus.SetReceiveHandler(std::bind(&Zone1MlmProxyImpl::cbZone1MlmStatus, this));
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to SetReceiveHandler for cbZone3RctnStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	result = this->mProxy->zone1MlmStatus.Subscribe(10);
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to Subscribe for zone3RcurtainStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	LOG_INFO() << "(-)\n";
}

void Zone1MlmProxyImpl::UnsubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone1MlmProxyImpl::UnsubscribeField()
{
	LOG_INFO() << "(+)\n";
	if (this->mProxy != nullptr)
	{
		this->mProxy->zone1MlmStatus.Unsubscribe();
	}
	LOG_INFO() << "(-)\n";
}

void Zone1MlmProxyImpl::cbZone1MlmStatus()
{
	LOG_INFO() << "(+)\n";

	zone1::mlm::control::EcuMlmState fieldValue;

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone1MlmStatus.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone1MlmStatus.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone1MlmStatus.GetNewSamples([&](auto msg) {
		fieldValue = static_cast<zone1::mlm::control::EcuMlmState>(*msg);   // fieldValue = *msg
		LOG_INFO() << "cbZone1MlmStatus : colorIndex " << (int)fieldValue.colorIndex << "\n";
		LOG_INFO() << "cbZone1MlmStatus : brightness " << (int)fieldValue.brightness << "\n";
		LOG_INFO() << "cbZone1MlmStatus : isNormal " << (int)fieldValue.isNormal << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifyMlmStatus(fieldValue);
		}
	});

	LOG_INFO() << "(-)\n";
}


}
}
}