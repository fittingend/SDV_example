#include "Zone1DrvSeatProxyImpl.hpp"
#include "thread"
#include "Log.hpp"
#include <Common.h>

namespace zone1 {
namespace drvseat {
namespace control {

Zone1DriverSeatProxyImpl *Zone1DriverSeatProxyImpl::s_mInst = nullptr;

Zone1DriverSeatProxyImpl *Zone1DriverSeatProxyImpl::GetInstance()
{
	if (Zone1DriverSeatProxyImpl::s_mInst == nullptr)
	{
		Zone1DriverSeatProxyImpl::s_mInst = new Zone1DriverSeatProxyImpl();
	}

	return Zone1DriverSeatProxyImpl::s_mInst;
}

void Zone1DriverSeatProxyImpl::DeleteInstance()
{
	if (Zone1DriverSeatProxyImpl::s_mInst != nullptr)
	{
		delete Zone1DriverSeatProxyImpl::s_mInst;
		Zone1DriverSeatProxyImpl::s_mInst = nullptr;
	}
}

Zone1DriverSeatProxyImpl::Zone1DriverSeatProxyImpl()
{
	this->mServiceFound = false;
	this->mProxy = nullptr;
	this->mFindHandle = nullptr;
	this->mListenerList.clear();
}

Zone1DriverSeatProxyImpl::~Zone1DriverSeatProxyImpl()
{
	if (this->mProxy)
	{
		this->mProxy->StopFindService(*(this->mFindHandle));
		this->mProxy.reset();
	}

	this->mListenerList.clear();
}

void Zone1DriverSeatProxyImpl::setEventListener(const std::shared_ptr<zone1::drvseat::control::IZone1DrvseatListener> _listener)
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

bool Zone1DriverSeatProxyImpl::init()
{
	LOG_INFO() << "(+)\n";

	ara::core::InstanceSpecifier specifier("SOA_DriverSeat/AA/RPort_Zone1DrvSeat");

	auto callback = [&](auto container, auto findHandle) {
		FindServiceCallback(container, findHandle);
	};

	auto result = proxy::Zone1DrvSeatProxy::StartFindService(callback, specifier);

	if (!result.HasValue())
	{
		LOG_INFO() << "Zone1DrvSeatProxy StartFindService() Failed\n";
		return false;
	}

	for (int i = 0; i < 20; i++)
	{
		if (this->mServiceFound)
		{
			LOG_INFO() << "Zone1DrvSeatProxy Start Find Service Success\n";
			return true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	LOG_INFO() << "(-)\n";
	return false;
}

void Zone1DriverSeatProxyImpl::FindServiceCallback(
		ara::com::ServiceHandleContainer<zone1::drvseat::control::proxy::Zone1DrvSeatProxy::HandleType> container,
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
			LOG_DEBUG() <<  "Zone1DriverSeatProxyImpl::Find::Searched Instance::ServiceId =" <<
										handle.GetServiceHandle().serviceId <<
										", InstanceId =" <<
										handle.GetServiceHandle().instanceId << "\n";
		}

		this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
		this->mProxy = std::make_shared<proxy::Zone1DrvSeatProxy>(container.at(0));

		LOG_INFO() << "Zone1DrvSeatProxy Find-Service Success\n";
		SubscribeEvent();
		SubscribeField();
		this->mServiceFound = true;
	}

	LOG_INFO() << "(-)\n";
}

void Zone1DriverSeatProxyImpl::controlMotor(const zone1::drvseat::control::EcuDrvSeatMotorInst& inst)
{
	LOG_INFO() << "(+) slide=" << (int)inst.slide << ", recline" << (int)inst.recline << ", height=" << (int)inst.height << ", rlxTilt=" << (int)inst.rlxTilt << "\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return;
	}

	this->mProxy->controlMotor(inst);
	LOG_INFO() << "(-)\n";
}

bool Zone1DriverSeatProxyImpl::getterMotorDir(zone1::drvseat::control::EcuDrvSeatMotorDirection& dir)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

 #if defined(HOTFIX_TEMP_001)
	dir.isNormal = 0;
	return true;
 #else

	auto future = this->mProxy->zone1DrvSeatMotorDir.Get();
	auto status = future.wait_for(std::chrono::milliseconds(250));
	if (status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			dir = static_cast<zone1::drvseat::control::EcuDrvSeatMotorDirection>(result.Value());
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

bool Zone1DriverSeatProxyImpl::getterMotorPos(zone1::drvseat::control::EcuDrvseatMotorPosition& pos)
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_INFO() << "(-) mProxy is null\n";
		return false;
	}

	auto future = this->mProxy->zone1DrvSeatMotorPos.Get();
	auto status = future.wait_for(std::chrono::milliseconds(250));
	if (status == ara::core::future_status::ready)
	{
		auto result = future.GetResult();
		if (result.HasValue())
		{
			pos = static_cast<zone1::drvseat::control::EcuDrvseatMotorPosition>(result.Value());
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
}

bool Zone1DriverSeatProxyImpl::isServiceFound()
{
	return this->mServiceFound;
}

void Zone1DriverSeatProxyImpl::SubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone1DriverSeatProxyImpl::SubscribeField()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone1DrvSeatMotorDir.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed)
	{
		LOG_ERROR() << "zone1DrvSeatMotorDir.GetSubscriptionState() != kNotSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	auto result = this->mProxy->zone1DrvSeatMotorDir.SetReceiveHandler(std::bind(&Zone1DriverSeatProxyImpl::cbZone1MotorDir, this));
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to SetReceiveHandler for cbZone3RctnStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	result = this->mProxy->zone1DrvSeatMotorDir.Subscribe(10);
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to Subscribe for zone3RcurtainStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone1DrvSeatMotorPos.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed)
	{
		LOG_ERROR() << "zone1DrvSeatMotorPos.GetSubscriptionState() != kNotSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	result = this->mProxy->zone1DrvSeatMotorPos.SetReceiveHandler(std::bind(&Zone1DriverSeatProxyImpl::cbZone1MotorPos, this));
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to SetReceiveHandler for cbZone3RctnStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	result = this->mProxy->zone1DrvSeatMotorPos.Subscribe(10);
	if (!result.HasValue())
	{
		LOG_ERROR() << "Failed to Subscribe for zone3RcurtainStatus\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	LOG_INFO() << "(-)\n";
}

void Zone1DriverSeatProxyImpl::UnsubscribeEvent()
{
	if (this->mProxy != nullptr)
	{
		// To Do:
	}
}

void Zone1DriverSeatProxyImpl::UnsubscribeField()
{
	if (this->mProxy != nullptr)
	{
		this->mProxy->zone1DrvSeatMotorDir.Unsubscribe();
		this->mProxy->zone1DrvSeatMotorPos.Unsubscribe();
	}
}

void Zone1DriverSeatProxyImpl::cbZone1MotorDir()
{
	LOG_INFO() << "(+)\n";

	zone1::drvseat::control::EcuDrvSeatMotorDirection fieldValue;

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone1DrvSeatMotorDir.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone1DrvSeatMotorDir.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone1DrvSeatMotorDir.GetNewSamples([&](auto msg) {
		fieldValue = static_cast<zone1::drvseat::control::EcuDrvSeatMotorDirection>(*msg);   // fieldValue = *msg
		LOG_INFO() << "cbZone1MotorDir : slide " << (int)fieldValue.slide << "\n";
		LOG_INFO() << "cbZone1MotorDir : recline " << (int)fieldValue.recline << "\n";
		LOG_INFO() << "cbZone1MotorDir : height " << (int)fieldValue.height << "\n";
		LOG_INFO() << "cbZone1MotorDir : rlxTilt " << (int)fieldValue.rlxTilt << "\n";
		LOG_INFO() << "cbZone1MotorDir : isNormal " << (int)fieldValue.isNormal << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifyMotorDir(fieldValue);
		}
	});

	LOG_INFO() << "(-)\n";
}

void Zone1DriverSeatProxyImpl::cbZone1MotorPos()
{
	LOG_INFO() << "(+)\n";

	zone1::drvseat::control::EcuDrvseatMotorPosition fieldValue;

	if (this->mProxy == nullptr)
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->zone1DrvSeatMotorPos.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed)
	{
		LOG_ERROR() << "zone1DrvSeatMotorPos.GetSubscriptionState() != kSubscribed\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	this->mProxy->zone1DrvSeatMotorPos.GetNewSamples([&](auto msg) {
		fieldValue = static_cast<zone1::drvseat::control::EcuDrvseatMotorPosition>(*msg);   // fieldValue = *msg
		LOG_INFO() << "cbZone1MotorDir : slidePos " << (int)fieldValue.slidePos << "\n";
		LOG_INFO() << "cbZone1MotorDir : reclinePos " << (int)fieldValue.reclinePos << "\n";
		LOG_INFO() << "cbZone1MotorDir : heightPos " << (int)fieldValue.heightPos << "\n";
		LOG_INFO() << "cbZone1MotorDir : rlxTiltPos " << (int)fieldValue.rlxTiltPos << "\n";
		for (auto item: this->mListenerList)
		{
			item->notifyMotorPos(fieldValue);
		}
	});

	LOG_INFO() << "(-)\n";
}

}
}
}