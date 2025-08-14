#ifndef __VEHICLE_CONTEXT_HPP__
#define __VEHICLE_CONTEXT_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <string>

class VehicleContext
{
private:
	static VehicleContext *mInst;

	VehicleContext();
	~VehicleContext();

public:
	DeviceNormal_e mDeviceNormal;
	bool mAuto;
	SmtfilmOpacities_t mOpacities;

	uint8_t mSwVersion;

	//
	// Methods
	//
	static VehicleContext *GetInstance();

	int Load(void);
	int Save(void);
	std::string DebugInfo(void);
};

#endif