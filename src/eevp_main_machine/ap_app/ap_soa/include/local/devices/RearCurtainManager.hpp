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
    ~RearCurtainManager();
public:
    static RearCurtainManager *GetInstance();

    void OnEventPower(PowerState_e power_curr, PowerState_e power_prev);
    void OnEventVehiclSpeed(int speed_curr, int speed_prev);
    void OnEventRcButton(RCtnSwitch_e btn_curr, RCtnSwitch_e btn_prev);
    void OnEventGearState(GearState_e gear_curr, GearState_e gear_prev);
    void OnEventRcState(RCtnState_e state_curr, RCtnState_e state_prev);

    int OnCmdRequestRearCurtainOperation(RCtnSwitch_e btn, bool api_modal = false);
};
#endif