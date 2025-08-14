#ifndef __ZONE1_VEHICLEINFO_PROXY_IMPL_H__
#define __ZONE1_VEHICLEINFO_PROXY_IMPL_H__

#include <list>
#include <mutex>
#include <SoaDataType.h>
#include <IZone1VehicleInfoListener.hpp>
#include <zone/input/service/zone_vehicleinfo_input_proxy.h>

namespace zone
{
namespace input
{
namespace service
{

class Zone1VehicleInfoProxyImpl {
public:
	static Zone1VehicleInfoProxyImpl *GetInstance();
	static void DeleteInstance();

	Zone1VehicleInfoProxyImpl();
	~Zone1VehicleInfoProxyImpl();

	void setEventListener(const std::shared_ptr<IZone1VehicleInfoListener> _listener);
	bool init();

	bool isServiceFound(void);

private:
	static Zone1VehicleInfoProxyImpl *s_mInst;

	void FindServiceCallback(
		ara::com::ServiceHandleContainer<zone::input::service::proxy::Zone_VehicleInfo_InputProxy::HandleType> container,
		ara::com::FindServiceHandle findHandle);

	/// @brief Subscribe Event
	void SubscribeEvent();
	/// @brief Subscribe Field
	void SubscribeField();
	/// @brief Unsubscribe Event
	void UnsubscribeEvent();
	/// @brief Unsubscribe Field
	void UnsubscribeField();

	// callback func
	void cbZone1VehicleInfo();

	// Check Data Changed;
	bool checkToNeedLog(int vehicle_speed, GearState_e gear, AccState_e acc, IgnState_e ign1, IgnState_e ign2);
	bool checkToNeedUpdate(int vehicle_speed, GearState_e gear, AccState_e acc, IgnState_e ign1, IgnState_e ign2);

	bool mServiceFound;
	std::mutex mMutexForListener;
	std::list<std::shared_ptr<IZone1VehicleInfoListener>> mListenerList;
	std::shared_ptr<zone::input::service::proxy::Zone_VehicleInfo_InputProxy> mProxy;
	std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;

	bool mIsLogged;
	uint64_t mLastLoggedTick;

	bool mIsNotified;
	uint64_t mLastNotifiedTick;
	int mLastVehicleSpeed;
	GearState_e mLastGear;
	AccState_e mLastAcc;
	IgnState_e mLastIgn1;
	IgnState_e mLastIgn2;
};

}
}
}

#endif // __ZONE1_VEHICLEINFO_PROXY_IMPL_H__