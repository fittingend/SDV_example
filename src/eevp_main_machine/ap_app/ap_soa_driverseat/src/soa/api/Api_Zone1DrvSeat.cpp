//#include <zone1/drvseat/control/zone1drvseat_proxy.h>
#include <Zone1DrvSeatProxyImpl.hpp>
#include <Api_Zone1DrvSeat.hpp>
#include <DriverSeatManager.hpp>
#include <Log.hpp>
#include <VehicleContext.hpp>

using namespace zone1::drvseat::control;

static EcuDrvSeatMotorInst zone1_convertCmdsToEcuMotorCmd()
{
	EcuDrvSeatMotorInst command;
	VehicleContext *context = VehicleContext::GetInstance();
	switch(context->mCmdHeight)
	{
	case eDriverSeatDir_Up:
		command.height = 1;
		break;
	case eDriverSeatDir_Down:
		command.height = 2;
		break;
	case eDriverSeatDir_Stop:
	default:
		command.height = 0;
		break;
	}

	switch(context->mCmdRecline)
	{
	case eDriverSeatDir_Forward:
		command.recline = 1;
		break;
	case eDriverSeatDir_Backward:
		command.recline = 2;
		break;
	case eDriverSeatDir_Stop:
	default:
		command.recline = 0;
		break;
	}

	switch(context->mCmdSlide)
	{
	case eDriverSeatDir_Forward:
		command.slide = 1;
		break;
	case eDriverSeatDir_Backward:
		command.slide = 2;
		break;
	case eDriverSeatDir_Stop:
	default:
		command.slide = 0;
		break;
	}

	switch(context->mCmdTilt)
	{
	case eDriverSeatDir_Up:
		command.rlxTilt = 1;
		break;
	case eDriverSeatDir_Down:
		command.rlxTilt = 2;
		break;
	case eDriverSeatDir_Stop:
	default:
		command.rlxTilt = 0;
		break;
	}

	return command;
}

static void zone1_getDirAndSetToContext(EcuDrvSeatMotorDirection dir)
{
	DriverSeatDir_e dir_height = eDriverSeatDir_NotAvailable;
	DriverSeatDir_e dir_recline = eDriverSeatDir_NotAvailable;
	DriverSeatDir_e dir_slide = eDriverSeatDir_NotAvailable;
	DriverSeatDir_e dir_tilt = eDriverSeatDir_NotAvailable;
	DeviceNormal_e isNormal = eDeviceNormal_NotAvailable;

	switch(dir.slide)
	{
	case 0:
		dir_slide = eDriverSeatDir_Stop;
		break;
	case 1:
		dir_slide = eDriverSeatDir_Forward;
		break;
	case 2:
		dir_slide = eDriverSeatDir_Backward;
		break;
	default:
		dir_slide = eDriverSeatDir_Stop;
		break;
	}

	switch(dir.recline)
	{
	case 0:
		dir_recline = eDriverSeatDir_Stop;
		break;
	case 1:
		dir_recline = eDriverSeatDir_Forward;
		break;
	case 2:
		dir_recline = eDriverSeatDir_Backward;
		break;
	default:
		dir_recline = eDriverSeatDir_Stop;
		break;
	}

	switch(dir.height)
	{
	case 0:
		dir_height = eDriverSeatDir_Stop;
		break;
	case 1:
		dir_height = eDriverSeatDir_Up;
		break;
	case 2:
		dir_height = eDriverSeatDir_Down;
		break;
	default:
		dir_height = eDriverSeatDir_Stop;
		break;
	}

	switch(dir.rlxTilt)
	{
	case 0:
		dir_tilt = eDriverSeatDir_Stop;
		break;
	case 1:
		dir_tilt = eDriverSeatDir_Up;
		break;
	case 2:
		dir_tilt = eDriverSeatDir_Down;
		break;
	default:
		dir_tilt = eDriverSeatDir_Stop;
		break;
	}

	switch(dir.isNormal)
	{
	case 0:
		isNormal = eDeviceNormal_Ok;
		break;
	case 1:
		isNormal = eDeviceNormal_Abnormal;
		break;
	default:
		isNormal = eDeviceNormal_Abnormal;
		break;
	}

	DriverSeatManager::GetInstance()->UpdateDeviceNormal(isNormal);
	DriverSeatManager::GetInstance()->UpdateMotorDirection(dir_height, dir_recline, dir_slide, dir_tilt);
}

static void zone1_getPosAndSetToContext(EcuDrvseatMotorPosition pos)
{
	DriverSeatManager::GetInstance()->UpdateMotorPosition((int)pos.heightPos, (int)pos.reclinePos, (int)pos.slidePos, (int)pos.rlxTiltPos);
}

class DriveSeatListener : public zone1::drvseat::control::IZone1DrvseatListener
{
	void notifyMotorDir(const zone1::drvseat::control::EcuDrvSeatMotorDirection &fieldValue) override
	{
		LOG_DEBUG() << "EcuDrvSeatMotorDirection.slide = " << (int)fieldValue.slide << "\n";
		LOG_DEBUG() << "EcuDrvSeatMotorDirection.recline = " << (int)fieldValue.recline << "\n";
		LOG_DEBUG() << "EcuDrvSeatMotorDirection.height = " << (int)fieldValue.height << "\n";
		LOG_DEBUG() << "EcuDrvSeatMotorDirection.rlxTilt = " << (int)fieldValue.rlxTilt << "\n";
		LOG_DEBUG() << "EcuDrvSeatMotorDirection.isNormal = " << (int)fieldValue.isNormal << "\n";
		zone1_getDirAndSetToContext(fieldValue);
	}

	void getMotorDir() override
	{
	}

	void notifyMotorPos(const zone1::drvseat::control::EcuDrvseatMotorPosition &fieldValue)
	{
		LOG_DEBUG() << "EcuDrvseatMotorPosition.slidePos = " << (int)fieldValue.slidePos << "\n";
		LOG_DEBUG() << "EcuDrvseatMotorPosition.reclinePos = " << (int)fieldValue.reclinePos << "\n";
		LOG_DEBUG() << "EcuDrvseatMotorPosition.heightPos = " << (int)fieldValue.heightPos << "\n";
		LOG_DEBUG() << "EcuDrvseatMotorPosition.rlxTiltPos = " << (int)fieldValue.rlxTiltPos << "\n";
		zone1_getPosAndSetToContext(fieldValue);
	}

	void getMotorPos() override
	{
	}
};

void Zone1_Drvseat_Init(void)
{
	auto *inst = Zone1DriverSeatProxyImpl::GetInstance();
	inst->setEventListener(std::make_shared<DriveSeatListener>());
	inst->init();
}

void Zone1_Drvseat_Term()
{
	Zone1DriverSeatProxyImpl::DeleteInstance();
}

void Zone1_Drvseat_SetHeight(DriverSeatDir_e eDir)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mCmdHeight != eDir)
	{
		context->mCmdHeight = eDir;
		Zone1DriverSeatProxyImpl::GetInstance()->controlMotor(zone1_convertCmdsToEcuMotorCmd());
	}
	LOG_DEBUG() << "(-)\n";
}

void Zone1_Drvseat_SetRecline(DriverSeatDir_e eDir)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mCmdRecline != eDir)
	{
		context->mCmdRecline = eDir;
		Zone1DriverSeatProxyImpl::GetInstance()->controlMotor(zone1_convertCmdsToEcuMotorCmd());
	}
	LOG_DEBUG() << "(-)\n";
}

void Zone1_Drvseat_SetSlide(DriverSeatDir_e eDir)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mCmdSlide != eDir)
	{
		context->mCmdSlide = eDir;
		Zone1DriverSeatProxyImpl::GetInstance()->controlMotor(zone1_convertCmdsToEcuMotorCmd());
	}
	LOG_DEBUG() << "(-)\n";
}

void Zone1_Drvseat_SetTilt(DriverSeatDir_e eDir)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mCmdTilt != eDir)
	{
		context->mCmdTilt = eDir;
		Zone1DriverSeatProxyImpl::GetInstance()->controlMotor(zone1_convertCmdsToEcuMotorCmd());
	}
	LOG_DEBUG() << "(-)\n";
}

void Zone1_Drvseat_UpdateContext(void)
{
	auto *inst = Zone1DriverSeatProxyImpl::GetInstance();

	EcuDrvSeatMotorDirection dir;
	if (inst->getterMotorDir(dir))
	{
		zone1_getDirAndSetToContext(dir);
	}

	EcuDrvseatMotorPosition pos;
	if (inst->getterMotorPos(pos))
	{
		zone1_getPosAndSetToContext(pos);
	}
}
