#ifndef __MOOD_LAMP_MANAGER_HPP__
#define __MOOD_LAMP_MANAGER_HPP__

#include <Common.h>
#include <SoaDataType.h>

#include <ManagerBase.hpp>

class MoodLampManager: public ManagerBase
{
private:
    static MoodLampManager *mInst;
    MoodLampManager();
    ~MoodLampManager();
public:
    static MoodLampManager *GetInstance();

    int OnCmdRequestMlmSetRgbColor(int colorTableIndex, bool api_modal = false);
    int OnCmdRequestMlmSetMode(MlmMoodMode_e mode, bool api_modal = false);
    int OnCmdRequestMlmSetBrightness(int brightness, bool api_modal = false);
};
#endif