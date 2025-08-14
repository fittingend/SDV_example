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
#if !defined(VEHICLE_CONTEXT_USE_ATOMIC)
    bool mSpeedValid;
    int mVehicleSpeed;
    GearState_e mGearState;

    // Rear Curtain
    PowerState_e mRCPower;
    RCtnSwitch_e mRCSwitch;
    RCtnState_e mRCState;
    RCtnSwitch_e mRCLastUserSwitch;

    // Mood Lamp
    PowerState_e mMlmPower;
    int mMlmColorIndex;
    int mMlmBrightness;
    MlmMoodMode_e mMlmMoodMode;
#else
    std::atomic<bool> mSpeedValid;
    std::atomic<int> mVehicleSpeed;
    std::atomic<GearState_e> mGearState;

    // Rear Curtain
    std::atomic<PowerState_e> mRCPower;
    std::atomic<RCtnSwitch_e> mRCSwitch;
    std::atomic<RCtnState_e> mRCState;

    std::atomic<RCtnSwitch_e> mRCLastUserSwitch;
    std::atomic<RCtnState_e> mRCLastState;

    // Mood Lamp
    std::atomic<PowerState_e> mMlmPower;
    std::atomic<int> mMlmColorIndex;
    std::atomic<int> mMlmBrightness;
    std::atomic<MlmMoodMode_e> mMlmMoodMode;
#endif

    //
    // Methods
    //
    static VehicleContext *GetInstance();

    int Load(void);
    int Save(void);
    std::string DebugInfo(void);

};

#endif