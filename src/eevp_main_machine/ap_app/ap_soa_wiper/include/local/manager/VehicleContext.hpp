#ifndef __VEHICLE_CONTEXT_HPP__
#define __VEHICLE_CONTEXT_HPP__

#include <Common.h>
#include <SoaDataType.h>

#include <atomic>
#include <string>
#include <cstdint>

class VehicleContext
{
private:
	static VehicleContext *mInst;

	VehicleContext();
	~VehicleContext();

public:
	std::atomic<DeviceNormal_e> mDeviceNormal;
	std::atomic<bool> mAutoMode;
	std::atomic<WiperOpMode_e> mWiperOpMode;

	int mVehicleSpeed;
	GearState_e mGear;
	AccState_e mAcc;
	IgnState_e mIgn1;
	IgnState_e mIgn2;

	//
	// Methods
	//
	static VehicleContext *GetInstance();

	int Load(void);
	int Save(void);
	std::string DebugInfo(void);
};

#endif