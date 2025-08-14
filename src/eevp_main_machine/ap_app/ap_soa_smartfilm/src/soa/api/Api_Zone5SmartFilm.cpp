#include <Api_Zone5SmartFilm.hpp>
#include <IZone5SmartFilmListener.hpp>
#include <Zone5SmartFilmProxyImpl.hpp>
#include <SmartFilmManager.hpp>
#include <Log.hpp>
#include <Debug_DataType2String.hpp>
#include <VehicleContext.hpp>

using namespace zone5::smtfilm::control;

class SmartFilmListener : public IZone5SmartFilmListener {
public:
	void notifyDeviceNormal(const DeviceNormal_e &isNormal) override
	{
		LOG_DEBUG() << "isNormal=" << getStringEnum_DeviceNormal_e(isNormal) << "\n";
		SmartFilmManager::GetInstance()->UpdateDeviceNormal(isNormal);
	}

	void notifyOpacities(const SmtfilmOpacities_t &opacities)
	{
		LOG_DEBUG() << "opacities:" << getString_SmtfilmOpacities_t(opacities) << "\n";
		SmartFilmManager::GetInstance()->UpdateSmtfilmOpacities(opacities);
	}
};

void Zone5_SMARTFILM_Init(void)
{
	auto *inst = Zone5SmartFilmProxyImpl::GetInstance();
	inst->setEventListener(std::make_shared<SmartFilmListener>());
	inst->init();
}

void Zone5_SMARTFILM_Term()
{
	Zone5SmartFilmProxyImpl::DeleteInstance();
}

void Zone5_SMARTFILM_SetOpacity(SmtfilmPos_e pos, int opacity)
{
	LOG_DEBUG() << "(+) pos=" << getStringEnum_SmtfilmPos_e(pos) << ", opacity=" << opacity << "\n";
	//Zone5HvacProxyImpl::GetInstance()->SetHvacPower(on);
	LOG_DEBUG() << "(-)\n";
}

void Zone5_SMARTFILM_SetAllOpacity(int opacity)
{
	LOG_DEBUG() << "(+) opacity=" << opacity << "\n";
	//Zone5HvacProxyImpl::GetInstance()->SetHvacPower(on);
	LOG_DEBUG() << "(-)\n";
}

void Zone5_SMARTFILM_UpdateContext(void)
{

}
