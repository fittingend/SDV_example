#ifndef __VEHICLE_CONTEXT_HPP__
#define __VEHICLE_CONTEXT_HPP__

#include <Common.h>
#include <SoaDataType.h>

#include <atomic>
#include <string>

class VehicleContext
{
private:
	static VehicleContext *mInst;

	VehicleContext();
	~VehicleContext();

public:
	std::atomic<DeviceNormal_e> mIsNormal;
	std::atomic<RoaRunningState_e> mRunningState;
	std::atomic<RoaMode_e> mRoaMode;
	std::atomic<int> mDetectCount;
	std::atomic<RoaSensorError_e> mSensorError;

	//
	// Methods
	//
	static VehicleContext *GetInstance();

	int Load(void);
	int Save(void);
	std::string DebugInfo(void);

};

#endif