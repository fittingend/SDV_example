#ifndef __ROA_MANAGER_HPP__
#define __ROA_MANAGER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <ManagerBase.hpp>

class RoaManager: public ManagerBase
{
private:
	static RoaManager *mInst;
	RoaManager();
	virtual ~RoaManager();

	void init();

	void cmdSetMode(ClientLevel_e level, RoaMode_e mode);
	void cmdReset(ClientLevel_e level);
	void cmdStart(ClientLevel_e level);
	void cmdStop(ClientLevel_e level);

	void onDeviceNormalChanged(DeviceNormal_e isNormal);
	void onRunningStateChanged(RoaRunningState_e runningState);
	void onRoaModeChanged(RoaMode_e mode);
	void onDetectCountChanged(int count);
	void onSensorErrorChanged(RoaSensorError_e err);

public:
	static RoaManager *GetInstance();
	static void RemoveInstance();

	void SetMode(ClientLevel_e level, RoaMode_e mode);
	void Reset(ClientLevel_e level);
	void Start(ClientLevel_e level);
	void Stop(ClientLevel_e level);

	void DeviceNormalChanged(DeviceNormal_e isNormal);
	void RunningStateChanged(RoaRunningState_e runningState);
	void RoaModeChanged(RoaMode_e mode);
	void DetectCountChanged(int count);
	void SensorErrorChanged(RoaSensorError_e err);
};
#endif // __ROA_MANAGER_HPP__
