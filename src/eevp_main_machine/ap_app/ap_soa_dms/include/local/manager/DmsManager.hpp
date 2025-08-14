#ifndef __DMS_MANAGER_HPP__
#define __DMS_MANAGER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <ManagerBase.hpp>

class DmsManager: public ManagerBase
{
private:
	static DmsManager *mInst;

	int mMulticastClientId;

	DmsManager();
	virtual ~DmsManager();

	void init();

	void cmdSetPower(const ClientLevel_e &level, const bool &on);
	void onDeviceNormalChanged(const DeviceNormal_e &isNormal);
	void onDmsStatusChanged(const bool &powerOn, const DmsDriverInside_e &inside);
	void onDriverStatusChanged(const DmsGazingDir_e &dir, const DmsFatigue_e &fatigue, const DmsSmoking_e &smoking, const DmsOnPhone_e &phone);
	void onVehicleInfoReceived(const int &vehicleSpeed, const GearState_e &gear, const AccState_e &acc, const IgnState_e &ign1, const IgnState_e &ign2);

public:
	static DmsManager *GetInstance();
	static void RemoveInstance();

	void SetPower(const ClientLevel_e &level, const bool &on);

	void DeviceNormalChanged(const DeviceNormal_e &isNormal);
	void DmsStatusChanged(const bool &powerOn, const DmsDriverInside_e &inside);
	void DriverStatusChanged(const DmsGazingDir_e &dir, const DmsFatigue_e &fatigue, const DmsSmoking_e &smoking, const DmsOnPhone_e &phone);
	void VehicleInfoReceived(const int &vehicleSpeed, const GearState_e &gear, const AccState_e &acc, const IgnState_e &ign1, const IgnState_e &ign2);

	void NotifyOnDriverNotFocusing();
	void NotifyOnDriverNotWaken();
	void NotifyOnDriverNotWatchingFront();
};
#endif // __DMS_MANAGER_HPP__