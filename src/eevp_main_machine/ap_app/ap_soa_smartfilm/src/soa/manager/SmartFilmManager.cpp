#include <Common.h>
#include <csignal>
#include <MessageItem.hpp>
#include <VehicleContext.hpp>
#include <SmartFilmManager.hpp>
#include <Api_SoaSmartFilm.hpp>
#include <Api_Zone5SmartFilm.hpp>
#include <Log.hpp>

SmartFilmManager *SmartFilmManager::mInst = nullptr;

SmartFilmManager *SmartFilmManager::GetInstance()
{
	if (SmartFilmManager::mInst == nullptr)
	{
		SmartFilmManager::mInst = new SmartFilmManager();
	}

	return SmartFilmManager::mInst;
}

void SmartFilmManager::RemoveInstance()
{
	if (SmartFilmManager::mInst != nullptr)
	{
		SmartFilmManager::mInst->Stop();
		delete SmartFilmManager::mInst;
		SmartFilmManager::mInst = nullptr;
	}
}

SmartFilmManager::SmartFilmManager()
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		this->init();
		return 0;
	});
}

SmartFilmManager::~SmartFilmManager()
{

}

void SmartFilmManager::init()
{
	LOG_DEBUG() << "(+)\n";

	LOG_DEBUG() << "(-)\n";
}

void SmartFilmManager::SetOpacity(ClientLevel_e level, SmtfilmPos_e pos, int opacity)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		SmtfilmPos_e _pos = (SmtfilmPos_e)arg2;
		int _opacity = (int)arg3;

		if ((_pos >= eSmtfilmPos_WindShield) && (_pos <= eSmtfilmPos_RearGrass))
		{
			Zone5_SMARTFILM_SetOpacity(_pos, _opacity);

			if (context->mOpacities.mOpacities[_pos] != _opacity)
			{
				context->mOpacities.mOpacities[_pos] = _opacity;
				SOA_SMARTFILM_UpdateSmtfilmOpacities();
			}
		}

		return 0;
	}, (int)level, (int)pos, (int)opacity);
	// LOG_DEBUG() << "(-)\n";
}

void SmartFilmManager::SetAllOpacity(ClientLevel_e level, int opacity)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		int _opacity = (int)arg2;

		Zone5_SMARTFILM_SetAllOpacity(_opacity);

		bool _changed = false;
		for (int _pos = eSmtfilmPos_WindShield; _pos <= eSmtfilmPos_RearGrass; _pos++)
		{
			if (context->mOpacities.mOpacities[_pos] != _opacity)
			{
				context->mOpacities.mOpacities[_pos] = _opacity;
				_changed = true;
			}
		}

		if (_changed)
		{
			SOA_SMARTFILM_UpdateSmtfilmOpacities();
		}

		return 0;
	}, (int)level, (int)opacity);
	// LOG_DEBUG() << "(-)\n";
}

void SmartFilmManager::SetAuto(ClientLevel_e level, bool on)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		bool _on = (bool)arg2;

		if (context->mAuto != _on)
		{
			context->mAuto = _on;
			SOA_SMARTFILM_UpdateSmtfilmAuto();
		}

		return 0;
	}, (int)level, (int)on);
	// LOG_DEBUG() << "(-)\n";
}

void SmartFilmManager::UpdateDeviceNormal(DeviceNormal_e isNormal)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		DeviceNormal_e _isNormal = (DeviceNormal_e)arg1;
		context->mDeviceNormal = _isNormal;
		SOA_SMARTFILM_UpdateDeviceNormal();
		return 0;
	}, (int)isNormal);
	// LOG_DEBUG() << "(-)\n";
}

void SmartFilmManager::UpdateAutoMode(bool on)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		bool _on = (bool)arg1;
		context->mAuto = _on;
		SOA_SMARTFILM_UpdateSmtfilmAuto();
		return 0;
	}, (int)on);
	// LOG_DEBUG() << "(-)\n";
}

void SmartFilmManager::UpdateSmtfilmOpacities(const SmtfilmOpacities_t &opacities)
{
	// LOG_DEBUG() << "(+)\n";
	SmtfilmOpacities_t *dupOpacities = new SmtfilmOpacities_t();
	*dupOpacities = opacities;

	FuncMsgItem_t msgItem;
	msgItem.mFunc = [this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		SmtfilmOpacities_t *_opacities = (SmtfilmOpacities_t *)argv;
		context->mOpacities = *_opacities;
		delete _opacities;
		SOA_SMARTFILM_UpdateSmtfilmOpacities();
		return 0;
	};
	msgItem.mArgPtr = (void *)dupOpacities;
	this->sendMessage(msgItem);
	// LOG_DEBUG() << "(-)\n";
}
