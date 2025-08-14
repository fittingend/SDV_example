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
	std::atomic<DeviceNormal_e> mDeviceNormal;

	std::atomic<DriverSeatDir_e> mCmdHeight;
	std::atomic<DriverSeatDir_e> mCmdRecline;
	std::atomic<DriverSeatDir_e> mCmdSlide;
	std::atomic<DriverSeatDir_e> mCmdTilt;

	std::atomic<DriverSeatDir_e> mDirHeight;
	std::atomic<DriverSeatDir_e> mDirRecline;
	std::atomic<DriverSeatDir_e> mDirSlide;
	std::atomic<DriverSeatDir_e> mDirTilt;

	std::atomic<int> mPosHeight;
	std::atomic<int> mPosRecline;
	std::atomic<int> mPosSlide;
	std::atomic<int> mPosTilt;

	//
	// Methods
	//
	static VehicleContext *GetInstance();

	int Load(void);
	int Save(void);
	std::string DebugInfo(void);

};

#endif