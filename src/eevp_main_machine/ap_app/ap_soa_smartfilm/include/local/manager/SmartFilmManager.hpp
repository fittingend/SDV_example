#ifndef __SMARTFILM_MANAGER_HPP__
#define __SMARTFILM_MANAGER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <ManagerBase.hpp>

class SmartFilmManager: public ManagerBase
{
private:
	static SmartFilmManager *mInst;
	SmartFilmManager();
	virtual ~SmartFilmManager();

	void init();

public:
	static SmartFilmManager *GetInstance();
	static void RemoveInstance();

	void SetOpacity(ClientLevel_e level, SmtfilmPos_e pos, int opacity);
	void SetAllOpacity(ClientLevel_e level, int opacity);
	void SetAuto(ClientLevel_e level, bool on);

	void UpdateDeviceNormal(DeviceNormal_e isNormal);
	void UpdateAutoMode(bool on);
	void UpdateSmtfilmOpacities(const SmtfilmOpacities_t &opacities);
};
#endif // __SMARTFILM_MANAGER_HPP__