#ifndef __PACON_SETTING_HPP__
#define __PACON_SETTING_HPP__

#include <Common.h>

class PaconSetting
{
private:
    static PaconSetting *mInst;

    bool mCoreInitialized;
    bool mExecutionStateReported;

    PaconSetting();
    ~PaconSetting();

public:
    static PaconSetting *GetInstance();
    static void RemoveInstance();

    bool StartPacon();
};

#endif