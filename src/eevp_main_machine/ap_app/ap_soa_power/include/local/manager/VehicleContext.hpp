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
	std::atomic<bool> mZone1Connected;
	std::atomic<DeviceNormal_e> mZone1PowerNormal;
	std::atomic<bool> mZone2Connected;
	std::atomic<DeviceNormal_e> mZone2PowerNormal;
	std::atomic<bool> mZone3Connected;
	std::atomic<DeviceNormal_e> mZone3PowerNormal;
	std::atomic<bool> mZone4Connected;
	std::atomic<DeviceNormal_e> mZone4PowerNormal;

	std::atomic<DeviceNormal_e> mTotalResult;

	//
	// Methods
	//
	static VehicleContext *GetInstance();

	int Load(void);
	int Save(void);
	std::string DebugInfo(void);

};

#endif