#ifndef __MOODLAMP_MANAGER_HPP__
#define __MOODLAMP_MANAGER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <ManagerBase.hpp>

class MoodLampManager: public ManagerBase
{
private:
	static MoodLampManager *mInst;
	MoodLampManager();
	virtual ~MoodLampManager();

	void init();

public:
	static MoodLampManager *GetInstance();
	static void RemoveInstance();

	void SetColorIndex(ClientLevel_e level, int index);
	void SetMode(ClientLevel_e level, MlmMoodMode_e mode);
	void SetBrightness(ClientLevel_e level, int brightness);

	void UpdateStatus(int colorIndex, int brightness, DeviceNormal_e isNormal);
};
#endif // __MOODLAMP_MANAGER_HPP__

