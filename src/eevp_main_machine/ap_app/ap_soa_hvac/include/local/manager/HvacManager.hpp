#ifndef __HVAC_MANAGER_HPP__
#define __HVAC_MANAGER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <ManagerBase.hpp>

class HvacManager: public ManagerBase
{
private:
	static HvacManager *mInst;
	HvacManager();
	virtual ~HvacManager();

	void init();

public:
	static HvacManager *GetInstance();
	static void RemoveInstance();

	void SetHvacPower(ClientLevel_e level, bool on);
	void SetAcOnOff(ClientLevel_e level, bool on);
	void SetAutoTemp(ClientLevel_e level, bool on);
	void SetSync(ClientLevel_e level, bool on);
	void SetDriverTemp(ClientLevel_e level, int temperature);
	void SetPassengerTemp(ClientLevel_e level, int temperature);
	void SetBlowingForce(ClientLevel_e level, int force);
	void SetBlowingDirection(ClientLevel_e level, HvacBlowDir_e dir);
	void SetAirSource(ClientLevel_e level, HvacAirSource_e src);
	void SetForwardDefrostOn(ClientLevel_e level, bool on);
	void SetBackwardDefrostOn(ClientLevel_e level, bool on);
	void UpdateDeviceNormal(DeviceNormal_e isNormal);
	void UpdateHvacSetting(const HvacStatus_t &setting);
	void UpdateHvacStatus(const HvacStatus_t &status);
	void UpdateHvacTemps(const HvacTemps_t &temps);

};
#endif // __HVAC_MANAGER_HPP__