#include <Common.h>
#include <csignal>
#include <MessageItem.hpp>
#include <VehicleContext.hpp>
#include <SoaRcurtainConverter.hpp>
#include <RearCurtainManager.hpp>
#include <Api_SoaRcurtain.hpp>
#include <Api_Zone3Rcurtain.hpp>
#include <SimpleTimerItem.hpp>
#include <SimpleTimer.hpp>
#include <Log.hpp>
#include <Debug_DataType2String.hpp>

RearCurtainManager *RearCurtainManager::mInst = nullptr;

RearCurtainManager *RearCurtainManager::GetInstance()
{
	if (RearCurtainManager::mInst == nullptr)
	{
		RearCurtainManager::mInst = new RearCurtainManager();
	}

	return RearCurtainManager::mInst;
}

void RearCurtainManager::RemoveInstance()
{
	if (RearCurtainManager::mInst != nullptr)
	{
		RearCurtainManager::mInst->Stop();
		delete RearCurtainManager::mInst;
		RearCurtainManager::mInst = nullptr;
	}
}

RearCurtainManager::RearCurtainManager()
{
	this->executeFunction([&](int arg1, int arg2, int arg3, int arg4, void *argv) {
		this->init();
		return 0;
	});
}

RearCurtainManager::~RearCurtainManager()
{

}

void RearCurtainManager::init()
{
	LOG_DEBUG() << "(+)\n";

	LOG_DEBUG() << "(-)\n";
}

void RearCurtainManager::onMoveMotor(ClientLevel_e level, RCtnMotorDir_e dir)
{
	VehicleContext *context = VehicleContext::GetInstance();

	LOG_DEBUG() << "(+) clientLevel=" << getStringEnum_ClientLevel_e(level) << ", motor_dir=" << getStringEnum_RCtnMotorDir_e(dir) << ", curr_state=" << getStringEnum_RCtnState_e(context->mRctnState) << "\n";

	if (context->mGearState == eGearState_R)
	{
		LOG_DEBUG() << "context->mGearState == eGearState_R... do not work in this time.\n";
	}
	else if (context->mMotorMoving)
	{
		LOG_DEBUG() << "context->mMotorMoving == true... wait until motor movement done.\n";
	}
	else
	{
		if ((dir == eRCtnMotorDir_OpenOn) || (dir == eRCtnMotorDir_CloseOn))
		{
			int posNum;
			RCtnPos_e posEnum;
			RCtnState_e runningState;

			if (dir == eRCtnMotorDir_OpenOn)
			{
				posNum = 0;
				posEnum = eRCtnPos_FullyOpened;
				runningState = eRCtnState_Opening;
			}
			else
			{
				posNum = 100;
				posEnum = eRCtnPos_FullyClosed;
				runningState = eRCtnState_Closing;
			}

			if (context->mRctnInputPosEnum != posEnum)
			{
				Zone3_RearCurtain_MovePosition(posEnum);
				this->onEnableInputBlocking();

				context->mRctnMotorDir = dir;
				context->mRctnState = runningState;
				context->mRctnInputPosEnum = posEnum;
				context->mRctnInputPosNum = posNum;

				SOA_REARCURTAIN_UpdateStatus();
			}
		}
	}

	LOG_DEBUG() << "(-)\n";
}

void RearCurtainManager::onMovePosition(ClientLevel_e level, int percentage)
{
	VehicleContext *context = VehicleContext::GetInstance();

	LOG_DEBUG() << "(+) clientLevel=" << getStringEnum_ClientLevel_e(level) << ", percentage=" << percentage << ", curr_state=" << getStringEnum_RCtnState_e(context->mRctnState) << "\n";

	if (context->mGearState == eGearState_R)
	{
		LOG_DEBUG() << "context->mGearState == eGearState_R... do not work in this time.\n";
	}
	else if (context->mMotorMoving)
	{
		LOG_DEBUG() << "context->mMotorMoving == true... wait until motor movement done.\n";
	}
	else
	{
		RCtnPos_e posEnum = Convert_MotorPos_API2SOA((uint8_t)percentage);
		if (posEnum != context->mRctnInputPosEnum)
		{
			RCtnMotorDir_e dir = ((int)posEnum > (int)context->mRctnInputPosEnum) ? eRCtnMotorDir_OpenOn : eRCtnMotorDir_CloseOn;
			RCtnState_e runningState = ((int)posEnum > (int)context->mRctnInputPosEnum) ? eRCtnState_Opening : eRCtnState_Closing;

			Zone3_RearCurtain_MovePosition(posEnum);
			this->onEnableInputBlocking();

			context->mRctnMotorDir = dir;
			context->mRctnState = runningState;
			context->mRctnInputPosEnum = posEnum;
			context->mRctnInputPosNum = percentage;

			SOA_REARCURTAIN_UpdateStatus();
		}
		else
		{
			LOG_DEBUG() << "context->posEnum == posEnum\n";
			if (context->mRctnInputPosNum != percentage)
			{
				context->mRctnInputPosNum = percentage;
				SOA_REARCURTAIN_UpdateStatus();
			}
		}
	}

	LOG_DEBUG() << "(-)\n";

}

void RearCurtainManager::onEnableInputBlocking()
{
	LOG_DEBUG() << "(+)\n";

	VehicleContext *context = VehicleContext::GetInstance();

	if (context->mMotorMovingTimerId != SIMPLE_TIMER_ID_NULL)
	{
		util::timer::SimpleTimer::GetInstance()->DeleteItem(context->mMotorMovingTimerId);
		context->mMotorMovingTimerId = SIMPLE_TIMER_ID_NULL;
	}

	util::timer::SimpleTimerItem *timer_item = new util::timer::SimpleTimerItem();

	timer_item->SetDuration(4000);		// 4sec
	timer_item->SetFunction([this](util::timer::SimpleTimerItem *val) {
		this->DisableInputBlocking(true);
		return true;
	});

	util::timer::SimpleTimer::GetInstance()->InsertItem(timer_item);
	context->mMotorMovingTimerId = timer_item->GetKey();
	context->mMotorMoving = true;
	LOG_DEBUG() << "(-)\n";

}

void RearCurtainManager::onDisableInputBlocking(bool isTimeout)
{
	LOG_DEBUG() << "(+) isTimeout=" << getStringEnum_bool(isTimeout) << "\n";
	VehicleContext *context = VehicleContext::GetInstance();

	if (!isTimeout)
	{
		util::timer::SimpleTimer::GetInstance()->DeleteItem(context->mMotorMovingTimerId);
	}

	context->mMotorMovingTimerId = SIMPLE_TIMER_ID_NULL;
	context->mMotorMoving = false;
	context->mRctnMotorDir = eRCtnMotorDir_Off;
	context->mRctnState = Convert_MotorPos2RctnState(context->mRctnInputPosEnum);

	if (isTimeout)
	{
		SOA_REARCURTAIN_UpdateStatus();
	}

	LOG_DEBUG() << "(-)\n";
}

void RearCurtainManager::onEventStatus(RCtnPos_e pos, DeviceNormal_e normal)
{
	LOG_DEBUG() << "(+) pos=" << getStringEnum_RCtnPos_e(pos) << ", normal=" << getStringEnum_DeviceNormal_e(normal) << "\n";
	VehicleContext *context = VehicleContext::GetInstance();
	bool isChanged = false;

	if (context->mMotorMoving)
	{
		if (context->mRctnInputPosEnum == pos)
		{
			this->onDisableInputBlocking(false);
			isChanged = true;
		}
	}
	else if (context->mRctnInputPosEnum != pos)
	{
		context->mRctnMotorDir = eRCtnMotorDir_Off;
		context->mRctnState = Convert_MotorPos2RctnState(pos);
		context->mRctnInputPosNum = (int)Convert_MotorPos_SOA2API(pos);
		context->mRctnInputPosEnum = pos;
		isChanged = true;
	}

	if (context->mIsNormal != normal)
	{
		context->mIsNormal = normal;
		isChanged = true;
	}

	if (isChanged)
	{
		SOA_REARCURTAIN_UpdateStatus();
	}

	LOG_DEBUG() << "(-)\n";
}

void RearCurtainManager::MoveMotor(ClientLevel_e level, RCtnMotorDir_e dir)
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		ClientLevel_e _level = (ClientLevel_e)arg1;
		RCtnMotorDir_e _dir = (RCtnMotorDir_e)arg2;

		this->onMoveMotor(_level, _dir);
		return 0;
	}, (int)level, (int)dir);
}

void RearCurtainManager::MovePosition(ClientLevel_e level, int percentage)
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		ClientLevel_e _level = (ClientLevel_e)arg1;
		int _percentage = (int)arg2;

		this->onMovePosition(_level, _percentage);
		return 0;
	}, (int)level, (int)percentage);
}

void RearCurtainManager::DisableInputBlocking(bool isTimeout)
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		bool _isTimeout = (bool)arg1;
		this->onDisableInputBlocking(_isTimeout);
		return 0;
	}, (int)isTimeout);
}

void RearCurtainManager::EventStatus(RCtnPos_e pos, DeviceNormal_e normal)
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		RCtnPos_e _pos = (RCtnPos_e)arg1;
		DeviceNormal_e _normal = (DeviceNormal_e)arg2;
		this->onEventStatus(_pos, _normal);
		return 0;
	}, (int)pos, (int)normal);
}



