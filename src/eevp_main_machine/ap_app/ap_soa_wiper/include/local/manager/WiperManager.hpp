#ifndef __WIPER_MANAGER_HPP__
#define __WIPER_MANAGER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <ManagerBase.hpp>

class WiperManager: public ManagerBase
{
private:
	static WiperManager *mInst;
	WiperManager();
	virtual ~WiperManager();

	void init();
	void cmdSetMotor(ClientLevel_e level, WiperOpMode_e opmode);
	void cmdSetAuto(ClientLevel_e level, bool isAuto);
	void onDeviceNormalChanged(DeviceNormal_e isNormal);
	void onAutoModeChanged(bool isAuto);
	void onMotorOpChanged(WiperOpMode_e opmode);
	void onVehicleInfoReceived(int speed, GearState_e gear, AccState_e acc, IgnState_e ign1, IgnState_e ign2);
	void onMfswReceived(MfswWiperSw_e wiper, MfswWasherSw_e washer, MfswWiperIntStepSw_e int_step, MfswWiperMistSw_e mist);

public:
	static WiperManager *GetInstance();
	static void RemoveInstance();

	void SetMotor(ClientLevel_e level, WiperOpMode_e opmode);
	void SetAuto(ClientLevel_e level, bool isAuto);
	void DeviceNormalChanged(DeviceNormal_e isNormal);
	void AutoModeChanged(bool isAuto);
	void MotorOpChanged(WiperOpMode_e opmode);

	void VehicleInfoReceived(int speed, GearState_e gear, AccState_e acc, IgnState_e ign1, IgnState_e ign2);
	void MfswReceived(MfswWiperSw_e wiper, MfswWasherSw_e washer, MfswWiperIntStepSw_e int_step, MfswWiperMistSw_e mist);
};
#endif // __WIPER_MANAGER_HPP__

