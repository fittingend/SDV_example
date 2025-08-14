#ifndef __REAR_CURTAIN_MANAGER_HPP__
#define __REAR_CURTAIN_MANAGER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <ManagerBase.hpp>

class RearCurtainManager: public ManagerBase
{
private:
	static RearCurtainManager *mInst;
	RearCurtainManager();
	virtual ~RearCurtainManager();

	void init();

	void onMoveMotor(ClientLevel_e level, RCtnMotorDir_e dir);
	void onMovePosition(ClientLevel_e level, int percentage);
	void onEnableInputBlocking();
	void onDisableInputBlocking(bool isTimeout);
	void onEventStatus(RCtnPos_e pos, DeviceNormal_e normal);

public:
	static RearCurtainManager *GetInstance();
	static void RemoveInstance();

	void MoveMotor(ClientLevel_e level, RCtnMotorDir_e dir);
	void MovePosition(ClientLevel_e level, int percentage);
	void DisableInputBlocking(bool isTimeout = false);
	void EventStatus(RCtnPos_e pos, DeviceNormal_e normal);
};
#endif // __REAR_CURTAIN_MANAGER_HPP__

