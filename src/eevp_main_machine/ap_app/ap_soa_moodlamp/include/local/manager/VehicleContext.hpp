#ifndef __VEHICLE_CONTEXT_HPP__
#define __VEHICLE_CONTEXT_HPP__

#include <Common.h>
#include <SoaDataType.h>

#if defined(VEHICLE_CONTEXT_USE_ATOMIC)
#include <atomic>
#endif
#include <string>

class VehicleContext
{
private:
	static VehicleContext *mInst;

	VehicleContext();
	~VehicleContext();

public:
	std::atomic<DeviceNormal_e> mDeviceNormal;
	std::atomic<bool> mSpeedValid;
	std::atomic<int> mVehicleSpeed;
	std::atomic<GearState_e> mGearState;

	// Mood Lamp
	std::atomic<PowerState_e> mMlmPower;
	std::atomic<int> mMlmColorIndex;
	std::atomic<int> mMlmBrightness;
	std::atomic<MlmMoodMode_e> mMlmMoodMode;

	//
	// Methods
	//
	static VehicleContext *GetInstance();

	int Load(void);
	int Save(void);
	std::string DebugInfo(void);

};

#endif