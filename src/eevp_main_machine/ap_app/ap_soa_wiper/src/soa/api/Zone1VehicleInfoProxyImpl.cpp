#include <Common.h>
#include <SoaDataType.h>
#include <IZone1VehicleInfoListener.hpp>
#include <Zone1VehicleInfoConverter.hpp>
#include <Zone1VehicleInfoProxyImpl.hpp>
#include <SimpleTimer.hpp>

#include <Log.hpp>
#include <Debug_DataType2String.hpp>

namespace zone {
namespace input {
namespace service {

Zone1VehicleInfoProxyImpl *Zone1VehicleInfoProxyImpl::s_mInst = nullptr;

Zone1VehicleInfoProxyImpl *Zone1VehicleInfoProxyImpl::GetInstance()
{
	if (Zone1VehicleInfoProxyImpl::s_mInst == nullptr)
	{
		Zone1VehicleInfoProxyImpl::s_mInst = new Zone1VehicleInfoProxyImpl();
	}

	return Zone1VehicleInfoProxyImpl::s_mInst;
}

void Zone1VehicleInfoProxyImpl::DeleteInstance()
{
	if (Zone1VehicleInfoProxyImpl::s_mInst != nullptr)
	{
		delete Zone1VehicleInfoProxyImpl::s_mInst;
		Zone1VehicleInfoProxyImpl::s_mInst = nullptr;
	}
}

Zone1VehicleInfoProxyImpl::Zone1VehicleInfoProxyImpl()
{
	this->mServiceFound = false;
	this->mProxy = nullptr;
	this->mFindHandle = nullptr;
	this->mListenerList.clear();
	this->mIsLogged = false;
	this->mLastLoggedTick = 0;
	this->mIsNotified = false;
	this->mLastNotifiedTick = 0;
	this->mLastVehicleSpeed = 0;
	this->mLastGear = eGearState_Unknown;
	this->mLastAcc = eAccState_Unknown;
	this->mLastIgn1 = eIgnState_Unknown;
	this->mLastIgn2 = eIgnState_Unknown;
}

Zone1VehicleInfoProxyImpl::~Zone1VehicleInfoProxyImpl()
{
	if (this->mProxy)
	{
		this->mProxy->StopFindService(*(this->mFindHandle));
		this->mProxy.reset();
	}

	this->mListenerList.clear();
}

void Zone1VehicleInfoProxyImpl::setEventListener(const std::shared_ptr<IZone1VehicleInfoListener> _listener)
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

bool Zone1VehicleInfoProxyImpl::init()
{
	LOG_INFO() << "(+)\n";

	ara::core::InstanceSpecifier specifier("SOA_Wiper/AA/RPort_Zone1VehicleInfo");

	auto callback = [&](auto container, auto findHandle) {
		FindServiceCallback(container, findHandle);
	};

	auto result = proxy::Zone_VehicleInfo_InputProxy::StartFindService(callback, specifier);

	if (!result.HasValue())
	{
		LOG_INFO() << "Zone_VehicleInfo_InputProxy StartFindService() Failed\n";
		return false;
	}

	LOG_INFO() << "(-)\n";
	return true;
}

void Zone1VehicleInfoProxyImpl::FindServiceCallback(
		ara::com::ServiceHandleContainer<zone::input::service::proxy::Zone_VehicleInfo_InputProxy::HandleType> container,
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
			LOG_DEBUG() <<  "Zone1VehicleInfoProxyImpl::Find::Searched Instance::ServiceId =" <<
										handle.GetServiceHandle().serviceId <<
										", InstanceId =" <<
										handle.GetServiceHandle().instanceId << "\n";
		}

		this->mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
		this->mProxy = std::make_shared<proxy::Zone_VehicleInfo_InputProxy>(container.at(0));

		LOG_INFO() << "Zone_VehicleInfo_InputProxy Find-Service Success\n";
		SubscribeEvent();
		SubscribeField();
		this->mServiceFound = true;
	}

	LOG_INFO() << "(-)\n";
}

bool Zone1VehicleInfoProxyImpl::isServiceFound()
{
	return this->mServiceFound;
}

void Zone1VehicleInfoProxyImpl::SubscribeEvent()
{
	LOG_INFO() << "(+)\n";

	if (this->mProxy == nullptr)
	{
		LOG_WARNING() << "this->mProxy == nullptr\n";
		LOG_INFO() << "(-)\n";
		return;
	}

	if (this->mProxy->VehicleInfo.GetSubscriptionState() == ara::com::SubscriptionState::kNotSubscribed)
	{
		auto result = this->mProxy->VehicleInfo.SetReceiveHandler(std::bind(&Zone1VehicleInfoProxyImpl::cbZone1VehicleInfo, this));
		if (result.HasValue())
		{
			result = this->mProxy->VehicleInfo.Subscribe(10);
			if (result.HasValue())
			{
				LOG_INFO() << "VehicleInfo subscription succeeded.\n";

			}
			else
			{
				LOG_ERROR() << "Failed to Subscribe for VehicleInfo\n";
			}
		}
		else
		{
			LOG_WARNING() << "Failed to VehicleInfo.SetReceiveHandler for cbZone1VehicleInfo.\n";
		}
	}
	else
	{
		LOG_WARNING() << "VehicleInfo.GetSubscriptionState() != kNotSubscribed\n";
	}

	LOG_INFO() << "(-)\n";
}

void Zone1VehicleInfoProxyImpl::SubscribeField()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone1VehicleInfoProxyImpl::UnsubscribeEvent()
{
	LOG_INFO() << "(+)\n";
	if (this->mProxy != nullptr)
	{
		this->mProxy->VehicleInfo.Unsubscribe();
	}
	LOG_INFO() << "(-)\n";
}

void Zone1VehicleInfoProxyImpl::UnsubscribeField()
{
	LOG_INFO() << "(+)\n";
	// not used
	LOG_INFO() << "(-)\n";
}

void Zone1VehicleInfoProxyImpl::cbZone1VehicleInfo()
{
	// LOG_INFO() << "(+)\n";

	if (this->mProxy != nullptr)
	{
		if (this->mProxy->VehicleInfo.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed)
		{
			this->mProxy->VehicleInfo.GetNewSamples([&](auto msg) {
				zone::input::type::ZoneByteVectorType info = static_cast<zone::input::type::ZoneByteVectorType>(*msg);

				AccState_e acc = Convert_AccState_Zone2SOA(info[0]);
				IgnState_e ign1 = Convert_IgnState_Zone2SOA(info[1]);
				IgnState_e ign2 = Convert_IgnState_Zone2SOA(info[2]);
				GearState_e gear = Convert_GearState_Zone2SOA(info[3]);
				int vehicle_speed = ((int)info[4] & 0xFF) | (((int)info[5] & 0xFF) << 8);

				if (this->checkToNeedLog(vehicle_speed, gear, acc, ign1, ign2))
				{
					LOG_INFO() << "(+)\n";
					LOG_INFO() << "VehicleInfo.ACC_In : " << (int)info[0] << " -> " << getStringEnum_AccState_e(acc) << "\n";
					LOG_INFO() << "VehicleInfo.IGN1_In : " << (int)info[1] << " -> " << getStringEnum_IgnState_e(ign1) << "\n";
					LOG_INFO() << "VehicleInfo.IGN2_In : " << (int)info[2] << " -> " << getStringEnum_IgnState_e(ign2) << "\n";
					LOG_INFO() << "VehicleInfo.GearState : " << (int)info[3] << " -> " << getStringEnum_GearState_e(gear) << "\n";
					LOG_INFO() << "VehicleInfo.VehicleSpeed : " << vehicle_speed << "\n";
					LOG_INFO() << "(-)\n";
				}

				if (this->checkToNeedUpdate(vehicle_speed, gear, acc, ign1, ign2))
				{
					std::unique_lock<std::mutex> lock(this->mMutexForListener);
					// LOG_INFO() << "VehicleInfo to be Notified...\n";
					for (auto item: this->mListenerList)
					{
						item->notifyVehicleInfo(vehicle_speed, gear, acc, ign1, ign2);
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

bool Zone1VehicleInfoProxyImpl::checkToNeedLog(int vehicle_speed, GearState_e gear, AccState_e acc, IgnState_e ign1, IgnState_e ign2)
{
	bool need_log = false;
	uint64_t curr_tick = util::timer::SimpleTimer::GetSystemTick();

	if (this->mIsLogged)
	{
		need_log = ((this->mLastGear != gear) || (this->mLastAcc != acc) || (this->mLastIgn1 != ign1) || (this->mLastIgn2 != ign2));
		if (!need_log)
		{
			need_log = (((vehicle_speed != 0) && (this->mLastVehicleSpeed == 0)) || ((vehicle_speed == 0) && (this->mLastVehicleSpeed != 0)));
		}

		if (!need_log)
		{
			need_log = (curr_tick >= this->mLastLoggedTick + 2000); 		// 2000ms
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


bool Zone1VehicleInfoProxyImpl::checkToNeedUpdate(int vehicle_speed, GearState_e gear, AccState_e acc, IgnState_e ign1, IgnState_e ign2)
{
	bool need_update = false;
	uint64_t curr_tick = util::timer::SimpleTimer::GetSystemTick();

	if (this->mIsNotified)
	{
		need_update = ((this->mLastGear != gear) || (this->mLastAcc != acc) || (this->mLastIgn1 != ign1) || (this->mLastIgn2 != ign2));
		if (!need_update)
		{
			need_update = (((vehicle_speed != 0) && (this->mLastVehicleSpeed == 0)) || ((vehicle_speed == 0) && (this->mLastVehicleSpeed != 0)));
		}

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
		this->mLastVehicleSpeed = vehicle_speed;
		this->mLastGear = gear;
		this->mLastAcc = acc;
		this->mLastIgn1 = ign1;
		this->mLastIgn2 = ign2;
	}

	return need_update;
}

}
}
}