#ifndef __DRIVERSEAT_MANAGER_HPP__
#define __DRIVERSEAT_MANAGER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <ManagerBase.hpp>

class DriverSeatManager: public ManagerBase
{
private:
	static DriverSeatManager *mInst;
	DriverSeatManager();
	virtual ~DriverSeatManager();

	void init();

public:
	static DriverSeatManager *GetInstance();
	static void RemoveInstance();

	// Method
	void SetHeight(ClientLevel_e clientLevel, DriverSeatDir_e dir);
	void SetRecline(ClientLevel_e clientLevel, DriverSeatDir_e dir);
	void SetSlide(ClientLevel_e clientLevel, DriverSeatDir_e dir);
	void SetTilt(ClientLevel_e clientLevel, DriverSeatDir_e dir);

	// Field Update
	void UpdateDeviceNormal(DeviceNormal_e isNormal);
	void UpdateMotorDirection(DriverSeatDir_e height, DriverSeatDir_e recline, DriverSeatDir_e slide, DriverSeatDir_e tilt);
	void UpdateMotorPosition(int height, int recline, int slide, int tilt);
};
#endif // __DRIVERSEAT_MANAGER_HPP__