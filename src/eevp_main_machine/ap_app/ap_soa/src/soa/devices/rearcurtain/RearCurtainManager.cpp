#include <Common.h>
#include <MessageItem.hpp>
#include <VehicleContext.hpp>
#include <Zone1Api_Local.hpp>
#include <Zone3Api_Local.hpp>
#include <ApiRcurtain_Local.hpp>
#include <RearCurtainManager.hpp>
#include <Log.hpp>

RearCurtainManager *RearCurtainManager::mInst = nullptr;

RearCurtainManager *RearCurtainManager::GetInstance()
{
    if (RearCurtainManager::mInst == nullptr)
    {
        RearCurtainManager::mInst = new RearCurtainManager();
        Api_Rcurtain_Field_SoaRctnStatus();
    }

    return RearCurtainManager::mInst;
}

RearCurtainManager::RearCurtainManager()
{

}

RearCurtainManager::~RearCurtainManager()
{

}

void RearCurtainManager::OnEventPower(PowerState_e power_curr, PowerState_e power_prev)
{

}

void RearCurtainManager::OnEventVehiclSpeed(int speed_curr, int speed_prev)
{

}


void RearCurtainManager::OnEventRcButton(RCtnSwitch_e btn_curr, RCtnSwitch_e btn_prev)
{
    funcMsgItem msgItem;

    switch (btn_curr)
    {
    case eRCtnSwitch_OpenOn:
        msgItem.mFunc = [](int arg1, int arg2, int arg3, void *argPtr) {
            LOG_DEBUG() << "[RearCurtainManager::OnEventRcButton] eRCtnSwitch_OpenOn\n";
            VehicleContext *context = VehicleContext::GetInstance();

            if (context->mRCState != eRCtnState_FullyOpened)
            {
                Zone3_RCtn_MoveCurtainMotor(eRCtnSwitch_OpenOn);
                // context->mRCState = eRCtnState_Opening;
                context->mRCLastState = eRCtnState_PartlyOpened;
                context->mRCLastUserSwitch = eRCtnSwitch_OpenOn;
                context->Save();
            }

            Api_Rcurtain_Field_SoaRctnStatus();
            return 0;
        };
        break;

    case eRCtnSwitch_CloseOn:
        msgItem.mFunc = [](int arg1, int arg2, int arg3, void *argPtr) {
            LOG_DEBUG() << "[RearCurtainManager::OnEventRcButton] eRCtnSwitch_CloseOn\n";
            VehicleContext *context = VehicleContext::GetInstance();

            if (context->mRCState != eRCtnState_FullyClosed && context->mGearState != eGearState_R)
            {
                Zone3_RCtn_MoveCurtainMotor(eRCtnSwitch_CloseOn);
                // context->mRCState = eRCtnState_Closing;
                context->mRCLastState = eRCtnState_PartlyOpened;
                context->mRCLastUserSwitch = eRCtnSwitch_CloseOn;
                context->Save();
            }

             Api_Rcurtain_Field_SoaRctnStatus();
             return 0;
        };
        break;

    case eRCtnSwitch_Off:
        msgItem.mFunc = [](int arg1, int arg2, int arg3, void *argPtr) {
            LOG_DEBUG() << "[RearCurtainManager::OnEventRcButton] eRCtnSwitch_Off\n";
            VehicleContext *context = VehicleContext::GetInstance();

            if (context->mRCLastUserSwitch == eRCtnSwitch_Unknown)
            {
                context->mRCLastUserSwitch = eRCtnSwitch_Off;
                context->Save();
            }
            else if (context->mRCLastUserSwitch != eRCtnSwitch_Off)
            {
                Zone3_RCtn_MoveCurtainMotor(eRCtnSwitch_Off);
                if (context->mRCState != eRCtnState_FullyClosed && context->mRCState != eRCtnState_FullyOpened)
                {
                    // context->mRCState = eRCtnState_PartlyOpened;
                    context->mRCLastState = eRCtnState_PartlyOpened;
                    // context->mRCLastUserSwitch = eRCtnSwitch_Off;
                    context->Save();
                }
            }

            Api_Rcurtain_Field_SoaRctnStatus();
            return 0;
        };
        break;

    default:
        return;
    }

    this->sendMessage(msgItem);
}

void RearCurtainManager::OnEventGearState(GearState_e gear_curr, GearState_e gear_prev)
{
    funcMsgItem msgItem;

    if (gear_curr == eGearState_R)
    {
        msgItem.mFunc = [](int arg1, int arg2, int arg3, void *argPtr) {
            LOG_DEBUG() << "[RearCurtainManager::OnEventGearState] eGearState_R\n";
            VehicleContext *context = VehicleContext::GetInstance();

            if (context->mRCState != eRCtnState_FullyOpened)
            {
                Zone3_RCtn_MoveCurtainMotor(eRCtnSwitch_OpenOn);
                //context->mRCState = eRCtnState_Opening;
                context->mRCLastState = eRCtnState_PartlyOpened;
                context->Save();
            }

            Api_Rcurtain_Field_SoaRctnStatus();
            return 0;
        };
    }
    else
    {
        msgItem.mFunc = [](int arg1, int arg2, int arg3, void *argPtr) {
            LOG_DEBUG() << "[RearCurtainManager::OnEventGearState] !eGearState_R\n";
            VehicleContext *context = VehicleContext::GetInstance();

            if (context->mRCLastUserSwitch == eRCtnSwitch_OpenOn && context->mRCState != eRCtnState_FullyOpened)
            {
                Zone3_RCtn_MoveCurtainMotor(eRCtnSwitch_OpenOn);
                //context->mRCState = eRCtnState_Opening;
                context->mRCLastState = eRCtnState_PartlyOpened;

                context->Save();
            }
            else if (context->mRCLastUserSwitch == eRCtnSwitch_CloseOn && context->mRCState != eRCtnState_FullyClosed)
            {
                Zone3_RCtn_MoveCurtainMotor(eRCtnSwitch_CloseOn);
                //context->mRCState = eRCtnState_Closing;
                context->mRCLastState = eRCtnState_PartlyOpened;
                context->Save();
            }

            Api_Rcurtain_Field_SoaRctnStatus();
            return 0;
        };
    }

    this->sendMessage(msgItem);
}

void RearCurtainManager::OnEventRcState(RCtnState_e state_curr, RCtnState_e state_prev)
{
    funcMsgItem msgItem;

    switch (state_curr)
    {
    case eRCtnState_FullyOpened:
        msgItem.mFunc = [](int arg1, int arg2, int arg3, void *argPtr) {
            LOG_DEBUG() << "[RearCurtainManager::OnEventRcState] eRCtnState_FullyOpened\n";
            VehicleContext *context = VehicleContext::GetInstance();

            if (context->mRCLastState != eRCtnState_FullyOpened)
            {
                Zone3_RCtn_MoveCurtainMotor(eRCtnSwitch_Off);
                // context->mRCState = eRCtnState_FullyOpened;
                context->mRCLastState = eRCtnState_FullyOpened;
                context->Save();
            }

            Api_Rcurtain_Field_SoaRctnStatus();
            return 0;
        };
        break;

    case eRCtnState_FullyClosed:
        msgItem.mFunc = [](int arg1, int arg2, int arg3, void *argPtr) {
            LOG_DEBUG() << "[RearCurtainManager::OnEventRcState] eRCtnState_FullyClosed\n";
            VehicleContext *context = VehicleContext::GetInstance();

            if (context->mRCLastState != eRCtnState_FullyClosed)
            {
                Zone3_RCtn_MoveCurtainMotor(eRCtnSwitch_Off);
                // context->mRCState = eRCtnState_FullyClosed;
                context->mRCLastState = eRCtnState_FullyClosed;
                context->Save();
            }

            Api_Rcurtain_Field_SoaRctnStatus();
            return 0;
        };
        break;

    default:
        msgItem.mFunc = [](int arg1, int arg2, int arg3, void *argPtr) {
            LOG_DEBUG() << "[RearCurtainManager::OnEventRcState] Others....\n";
            Api_Rcurtain_Field_SoaRctnStatus();
            return 0;
        };
        break;
    }

    this->sendMessage(msgItem);
}

int RearCurtainManager::OnCmdRequestRearCurtainOperation(RCtnSwitch_e btn, bool api_modal)
{
    funcMsgItem msgItem;

    switch (btn)
    {
    case eRCtnSwitch_OpenOn:
        msgItem.mFunc = [](int arg1, int arg2, int arg3, void *argPtr) {
            LOG_DEBUG() << "[RearCurtainManager::OnCmdRequestRearCurtainOperation] eRCtnSwitch_OpenOn\n";
            VehicleContext *context = VehicleContext::GetInstance();

            if (context->mRCState != eRCtnState_FullyOpened)
            {
                Zone3_RCtn_MoveCurtainMotor(eRCtnSwitch_OpenOn);
                context->mRCState = eRCtnState_Opening;
                context->mRCLastState = eRCtnState_PartlyOpened;
                context->mRCLastUserSwitch = eRCtnSwitch_OpenOn;
                context->Save();
            }

            Api_Rcurtain_Field_SoaRctnStatus();
            return 0;
        };
        break;

    case eRCtnSwitch_CloseOn:
        msgItem.mFunc = [](int arg1, int arg2, int arg3, void *argPtr) {
            LOG_DEBUG() << "[RearCurtainManager::OnCmdRequestRearCurtainOperation] eRCtnSwitch_CloseOn\n";
            VehicleContext *context = VehicleContext::GetInstance();

            if (context->mRCState != eRCtnState_FullyClosed && context->mGearState != eGearState_R)
            {
                Zone3_RCtn_MoveCurtainMotor(eRCtnSwitch_CloseOn);
                context->mRCState = eRCtnState_Closing;
                context->mRCLastState = eRCtnState_PartlyOpened;
                context->mRCLastUserSwitch = eRCtnSwitch_CloseOn;
                context->Save();
            }

            Api_Rcurtain_Field_SoaRctnStatus();
            return 0;
        };
        break;

    case eRCtnSwitch_Off:
        msgItem.mFunc = [](int arg1, int arg2, int arg3, void *argPtr) {
            LOG_DEBUG() << "[RearCurtainManager::OnCmdRequestRearCurtainOperation] eRCtnSwitch_Off\n";
            VehicleContext *context = VehicleContext::GetInstance();

            if (context->mRCLastUserSwitch == eRCtnSwitch_Unknown)
            {
                context->mRCLastUserSwitch = eRCtnSwitch_Off;
                context->Save();
            }
            else if (context->mRCLastUserSwitch != eRCtnSwitch_Off)
            {
                Zone3_RCtn_MoveCurtainMotor(eRCtnSwitch_Off);
                if (context->mRCState != eRCtnState_FullyClosed && context->mRCState != eRCtnState_FullyOpened)
                {
                    context->mRCState = eRCtnState_PartlyOpened;
                    context->mRCLastState = eRCtnState_PartlyOpened;
                    context->mRCLastUserSwitch = eRCtnSwitch_Off;
                    context->Save();
                }
            }

            Api_Rcurtain_Field_SoaRctnStatus();
            return 0;
        };
        break;

    default:
        return 1;
    }

    return this->sendMessage(msgItem, api_modal);
}