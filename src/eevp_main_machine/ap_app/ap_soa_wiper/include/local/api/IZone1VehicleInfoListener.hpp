#ifndef __IZONE1_VEHICLEINFO_LISTENER_HPP__
#define __IZONE1_VEHICLEINFO_LISTENER_HPP__

#include <SoaDataType.h>

class IZone1VehicleInfoListener {
public:
	virtual ~IZone1VehicleInfoListener() {};
	virtual void notifyVehicleInfo(int vehicleSpeed, GearState_e gear, AccState_e acc, IgnState_e ign1, IgnState_e ign2) = 0;
};

#endif // __IZONE1_VEHICLEINFO_LISTENER_HPP__