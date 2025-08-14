#include <Common.h>
#include <csignal>
#include <MessageItem.hpp>
#include <VehicleContext.hpp>
#include <MoodLampManager.hpp>
#include <Api_SoaMoodLamp.hpp>
#include <Api_Zone1MoodLamp.hpp>
#include <Log.hpp>
#include <Debug_DataType2String.hpp>

MoodLampManager *MoodLampManager::mInst = nullptr;

MoodLampManager *MoodLampManager::GetInstance()
{
	if (MoodLampManager::mInst == nullptr)
	{
		MoodLampManager::mInst = new MoodLampManager();
	}

	return MoodLampManager::mInst;
}

void MoodLampManager::RemoveInstance()
{
	if (MoodLampManager::mInst != nullptr)
	{
		MoodLampManager::mInst->Stop();
		delete MoodLampManager::mInst;
		MoodLampManager::mInst = nullptr;
	}
}

MoodLampManager::MoodLampManager()
{
	this->executeFunction([&](int arg1, int arg2, int arg3, int arg4, void *argv) {
		this->init();
		return 0;
	});
}

MoodLampManager::~MoodLampManager()
{

}

void MoodLampManager::init()
{
	LOG_DEBUG() << "(+)\n";

	LOG_DEBUG() << "(-)\n";
}

void MoodLampManager::SetColorIndex(ClientLevel_e level, int index)
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		int _index = (int)arg2;
		LOG_DEBUG() << "(+) level=" << getStringEnum_ClientLevel_e(_level) << ", index=" << _index << "\n";
		Zone1_MoodLamp_SetColorIndex(_index);
		if (context->mMlmColorIndex != _index)
		{
			context->mMlmColorIndex = _index;
			SOA_MoodLamp_UpdateStatus();
		}
		LOG_DEBUG() << "(-)\n";
		return 0;
	}, (int)level, (int)index);
}

void MoodLampManager::SetMode(ClientLevel_e level, MlmMoodMode_e mode)
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		MlmMoodMode_e _mode = (MlmMoodMode_e)arg2;
		LOG_DEBUG() << "(+) level=" << getStringEnum_ClientLevel_e(_level) << ", index=" << getStringEnum_MlmMoodMode_e(_mode) << "\n";
		Zone1_MoodLamp_SetColorMode(_mode);
		if (context->mMlmMoodMode != _mode)
		{
			context->mMlmMoodMode = _mode;
			SOA_MoodLamp_UpdateStatus();
		}
		LOG_DEBUG() << "(-)\n";
		return 0;
	}, (int)level, (int)mode);
}

void MoodLampManager::SetBrightness(ClientLevel_e level, int brightness)
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		int _brightness = (int)arg2;
		LOG_DEBUG() << "(+) level=" << getStringEnum_ClientLevel_e(_level) << ", brightness=" << _brightness << "\n";
		Zone1_MoodLamp_SetBrightness(_brightness);
		if (context->mMlmBrightness != _brightness)
		{
			context->mMlmBrightness = _brightness;
			SOA_MoodLamp_UpdateStatus();
		}
		LOG_DEBUG() << "(-)\n";
		return 0;
	}, (int)level, (int)brightness);

}

void MoodLampManager::UpdateStatus(int colorIndex, int brightness, DeviceNormal_e isNormal)
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		int _colorIndex = (int)arg1;
		int _brightness = (int)arg2;
		DeviceNormal_e _isNormal = (DeviceNormal_e)arg3;
		LOG_DEBUG() << "(+) colorIndex=" << _colorIndex << ", brightness=" << _brightness << ", isNormal=" << getStringEnum_DeviceNormal_e(_isNormal) << "\n";
		bool isChanged = false;
		if (context->mMlmColorIndex != _colorIndex)
		{
			context->mMlmColorIndex = _colorIndex;
			isChanged = true;
		}

		if (context->mMlmBrightness != _brightness)
		{
			context->mMlmBrightness = _brightness;
			isChanged = true;
		}

		if (context->mDeviceNormal != _isNormal)
		{
			context->mDeviceNormal = _isNormal;
			isChanged = true;
		}

		if (isChanged)
		{
			SOA_MoodLamp_UpdateStatus();
		}

		LOG_DEBUG() << "(-)\n";
		return 0;

	}, (int)colorIndex, (int)brightness, (int)isNormal);
}

