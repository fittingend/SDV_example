#include <Common.h>
#include <SoaDataType.h>
#include <IZone1MfswListener.hpp>
#include <Zone1MfswConverter.hpp>
#include <Zone1MfswProxyImpl.hpp>
#include <SimpleTimer.hpp>

#include <Log.hpp>
#include <Debug_DataType2String.hpp>

namespace zone1
{
namespace mfsw
{
namespace input
{

Zone1MfswProxyImpl *Zone1MfswProxyImpl::s_mInst = nullptr;

Zone1MfswProxyImpl *Zone1MfswProxyImpl::GetInstance()
{
	if (Zone1MfswProxyImpl::s_mInst == nullptr)
	{
		Zone1MfswProxyImpl::s_mInst = new Zone1MfswProxyImpl();
	}

	return Zone1MfswProxyImpl::s_mInst;
}

void Zone1MfswProxyImpl::DeleteInstance()
{
	if (Zone1MfswProxyImpl::s_mInst != nullptr)
	{
		delete Zone1MfswProxyImpl::s_mInst;
		Zone1MfswProxyImpl::s_mInst = nullptr;
	}
}

Zone1MfswProxyImpl::Zone1MfswProxyImpl()
{
	this->mServiceFound = false;
	this->mProxy = nullptr;
	this->mFindHandle = nullptr;
	this->mListenerList.clear();
	this->mIsLogged = false;
	this->mLastLoggedTick = 0;
}

Zone1MfswProxyImpl::~Zone1MfswProxyImpl()
{
	if (this->mProxy)
	{
		this->mProxy->StopFindService(*(this->mFindHandle));
		this->mProxy.reset();
	}

	this->mListenerList.clear();
}

void Zone1MfswProxyImpl::setEventListener(const std::shared_ptr<IZone1MfswListener> _listener)
{
	std::unique_lock<std::mutex> lock(this->mMutexForListener);
	for (auto item: this->mListenerList)
	{
		if (item.get() == _listener.get())
		{
			return;
		}
	}

	this->mListenerList.push_back(_listener);
}

bool Zone1MfswProxyImpl::init()
{
	LOG_INFO() << "(+)\n";

	ara::core::InstanceSpecifier specifier("SOA_Wiper/AA/RPort_Zone1Mfsw");

	auto callback = [&](auto container, auto findHandle) {
		FindServiceCallback(container, findHandle);
	};

	auto result = proxy::Zone1MfswInputProxy::StartFindService(callback, specifier);

	if (!result.HasValue())
	{
		LOG_INFO() << "Zone1MfswInputProxy StartFindService() Failed\n";
		return false;
	}

	LOG_INFO() << "(-)\n";
	return true;
}

void Zone1MfswProxyImpl::FindServiceCallback(
		ara::com::ServiceHandleContainer<zone1::mfsw::input::proxy::Zone1MfswInputProxy::HandleType> container,
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
			LOG_DEBUG() <<  "Zone1MfswProxyImpl::Find::Searched Instance::ServiceId =" <<
										handle.GetServiceHandle().serviceId <<
										", InstanceId =" <<
										handle.GetServiceHandle().instanceId << "\n";
		}

		this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
		this->mProxy = std::make_shared<proxy::Zone1MfswInputProxy>(container.at(0));

		LOG_INFO() << "Zone1MfswInputProxy Find-Service Success\n";
		SubscribeEvent();
		SubscribeField();
		this->mServiceFound = true;
	}

	LOG_INFO() << "(-)\n";
}

bool Zone1MfswProxyImpl::isServiceFound()
{
	return this->mServiceFound;
}

void Zone1MfswProxyImpl::SubscribeEvent()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_WARNING() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->onMfswInput.GetSubscriptionState() == ara::com::SubscriptionState::kNotSubscribed)
	{
		auto result = this->mProxy->onMfswInput.SetReceiveHandler(std::bind(&Zone1MfswProxyImpl::cbZone1Mfsw, this));
		if (result.HasValue())
		{
			result = this->mProxy->onMfswInput.Subscribe(10);
			if (result.HasValue())
			{
				LOG_INFO() << "onMfswInput subscription succeeded.\n";

			}
			else
			{
				LOG_ERROR() << "Failed to Subscribe for onMfswInput\n";
			}
		}
		else
		{
			LOG_WARNING() << "Failed to onMfswInput.SetReceiveHandler for cbZone1Mfsw.\n";
		}
	}
	else
	{
		LOG_WARNING() << "VehicleInfo.GetSubscriptionState() != kNotSubscribed\n";
	}

	LOG_INFO() << "(-)\n";
}

void Zone1MfswProxyImpl::SubscribeField()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone1MfswProxyImpl::UnsubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	if (this->mProxy != nullptr)
	{
		this->mProxy->onMfswInput.Unsubscribe();
	}
	LOG_INFO() << "(-)\n";
}

void Zone1MfswProxyImpl::UnsubscribeField()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone1MfswProxyImpl::cbZone1Mfsw()
{
	// LOG_INFO() << "(+)\n";

	if (this->mProxy != nullptr)
	{
		if (this->mProxy->onMfswInput.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
		{
			this->mProxy->onMfswInput.GetNewSamples([&](auto msg) {
				zone::input::type::ZoneByteVectorType info = static_cast<zone::input::type::ZoneByteVectorType>(*msg);
				if (info.size() >= 4)
				{
					MfswWiperSw_e wiperSw = Convert_MfswWiperSwitch_Zone2SOA(info[0]);
					MfswWasherSw_e washerSw = Convert_MfswWasherSwitch_Zone2SOA(info[1]);
					MfswWiperMistSw_e mistSw = Convert_MfswMistSwitch_Zone2SOA(info[2]);
					MfswWiperIntStepSw_e intSw = Convert_MfswWiperIntStep_Zone2SOA(info[3]);

					if (this->checkToNeedLog(wiperSw, washerSw, mistSw, intSw))
					{
						LOG_INFO() << "(+)\n";
						LOG_INFO() << "EcuZ1Mfsw.wiperState : " << (int)info[0] << " -> " << getStringEnum_MfswWiperSw_e(wiperSw) << "\n";
						LOG_INFO() << "EcuZ1Mfsw.wiperMistState : " << (int)info[1] << " -> " << getStringEnum_MfswWiperMistSw_e(mistSw) << "\n";
						LOG_INFO() << "EcuZ1Mfsw.wiperIntState : " << (int)info[2] << " -> " << getStringEnum_MfswWiperIntStepSw_e(intSw) << "\n";
						LOG_INFO() << "EcuZ1Mfsw.wiperWasherState : " << (int)info[3] << " -> " << getStringEnum_MfswWasherSw_e(washerSw) << "\n";
						LOG_INFO() << "(-)\n";
					}

					if (this->checkToNeedUpdate(wiperSw, washerSw, mistSw, intSw))
					{
						std::unique_lock<std::mutex> lock(this->mMutexForListener);
						// LOG_INFO() << "EcuZ1Mfsw to be Notified...\n";

						for (auto item: this->mListenerList)
						{
							item->notifyMfsw(wiperSw, washerSw, intSw, mistSw);
						}
					}
				}
			});
		}
		else
		{
			LOG_ERROR() << "VehicleInfo.GetSubscriptionState() != kSubscribed\n";
		}
	}
	else
	{
		LOG_ERROR() << "this->mProxy == nullptr\n";
	}

	// LOG_INFO() << "(-)\n";
}

bool Zone1MfswProxyImpl::checkToNeedLog(MfswWiperSw_e wiper_sw, MfswWasherSw_e washer_sw, MfswWiperMistSw_e mist_sw, MfswWiperIntStepSw_e int_sw)
{
	bool need_log = false;
	uint64_t curr_tick = util::timer::SimpleTimer::GetSystemTick();

	if (this->mIsLogged)
	{
		need_log = ((this->mLastWiperSw != wiper_sw) || (this->mLastWasherSw != washer_sw) || (this->mLastMistSw != mist_sw) || (this->mLastIntStep != int_sw));

		if (!need_log)
		{
			need_log = (curr_tick >= this->mLastLoggedTick + 2000);		// 2000ms
		}
	}
	else
	{
		need_log = true;
	}

	if (need_log)
	{
		this->mIsLogged = true;
		this->mLastLoggedTick = curr_tick;
	}

	return need_log;
}

bool Zone1MfswProxyImpl::checkToNeedUpdate(MfswWiperSw_e wiper_sw, MfswWasherSw_e washer_sw, MfswWiperMistSw_e mist_sw, MfswWiperIntStepSw_e int_sw)
{
	bool need_update = false;
	uint64_t curr_tick = util::timer::SimpleTimer::GetSystemTick();

	if (this->mIsNotified)
	{
		need_update = ((this->mLastWiperSw != wiper_sw) || (this->mLastWasherSw != washer_sw) || (this->mLastMistSw != mist_sw) || (this->mLastIntStep != int_sw));
		if (!need_update)
		{
			need_update = (curr_tick >= this->mLastNotifiedTick + 200); 		// 200ms
		}
	}
	else
	{
		need_update = true;
	}

	if (need_update)
	{
		this->mIsNotified = true;
		this->mLastNotifiedTick = curr_tick;
		this->mLastWiperSw = wiper_sw;
		this->mLastWasherSw = washer_sw;
		this->mLastMistSw = mist_sw;
		this->mLastIntStep = int_sw;
	}

	return need_update;
}


}
}
}
