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
	std::atomic<bool> mPowerOn;
	std::atomic<DmsDriverInside_e> mDriverInside;
	std::atomic<DmsGazingDir_e> mGazingDir;
	std::atomic<DmsFatigue_e> mFatigue;
	std::atomic<DmsOnPhone_e> mIsOnPhone;
	std::atomic<DmsSmoking_e> mIsSmoking;
	std::atomic<uint8_t> mSwVersion;

	//
	// Methods
	//
	static VehicleContext *GetInstance();

	int Load(void);
	int Save(void);
	std::string DebugInfo(void);
};

#endif