#include <Common.h>
#include <SoaDataType.h>
#include <string.h>
#include <VehicleContext.hpp>
#include <RearCurtainManager.hpp>
#include <ApiRcurtain_Local.hpp>
#include <SoaRcurtainSkeletonImpl.hpp>
#if !defined(DISABLE_SOMEIP)
#include <PaconSetting.hpp>
#endif
#include <Log.hpp>

int Api_Rcurtain_Method_RequestRearCurtainOperation(RCtnSwitch_e eDirection)
{
    RearCurtainManager *mgr = RearCurtainManager::GetInstance();
    return mgr->OnCmdRequestRearCurtainOperation(eDirection);
}

void Api_Rcurtain_Field_SoaRctnStatus()
{
    eevp::control::SoaRctnStatus rctnStatus;
    VehicleContext *context = VehicleContext::GetInstance();

    eevp::control::SoaErrorState errorState;
    eevp::control::SoaRctnMotorDir curMotorDirection;
    eevp::control::SoaRctnState curtainState;
    eevp::control::SoaDeviceIsNormal isNormal;

    errorState = eevp::control::SoaErrorState::kOK;

    switch (context->mRCSwitch)
    {
    case eRCtnSwitch_OpenOn:
        curMotorDirection = eevp::control::SoaRctnMotorDir::kDOWN;
        break;
    case eRCtnSwitch_CloseOn:
        curMotorDirection = eevp::control::SoaRctnMotorDir::kUP;
        break;
    case eRCtnSwitch_Off:
    default:
        curMotorDirection = eevp::control::SoaRctnMotorDir::kSTOP;
        break;
    }

    switch (context->mRCState)
    {
    case eRCtnState_FullyOpened:
        curtainState = eevp::control::SoaRctnState::kFULLY_DOWN;
        break;
    case eRCtnState_FullyClosed:
        curtainState = eevp::control::SoaRctnState::kFULLY_UP;
        break;
    case eRCtnState_PartlyOpened:
        curtainState = eevp::control::SoaRctnState::kPARTLY_OPENED;
        break;
    case eRCtnState_Opening:
        curtainState = eevp::control::SoaRctnState::kMOVING_DOWN;
        break;
    case eRCtnState_Closing:
        curtainState = eevp::control::SoaRctnState::kMOVING_UP;
        break;
    default:
        curtainState = eevp::control::SoaRctnState::kFULLY_UP;
        break;
    }

    isNormal = eevp::control::SoaDeviceIsNormal::kNORMAL;

    rctnStatus.errorState = errorState;
    rctnStatus.curMotorDirection = curMotorDirection;
    rctnStatus.curtainState = curtainState;
    rctnStatus.isNormal = isNormal;

#if !defined(DISABLE_SOMEIP)
    PaconSetting *pacon = PaconSetting::GetInstance();
    if (pacon != nullptr)
    {
        eevp::control::SoaRcurtainSkeletonImpl *rctn = pacon->GetSoaRcurtain();
        if (rctn != nullptr)
        {
            rctn->SetSoaRctnStatus(rctnStatus);
        }
    }
#else
    LOG_DEBUG() << "[Api_Rcurtain_Field_SoaRctnStatus] rctnStatus.errorState = " << (int)rctnStatus.errorState << "\n";
    LOG_DEBUG() << "[Api_Rcurtain_Field_SoaRctnStatus] rctnStatus.curMotorDirection = " << (int)rctnStatus.curMotorDirection << "\n";
    LOG_DEBUG() << "[Api_Rcurtain_Field_SoaRctnStatus] rctnStatus.curtainState = " << (int)rctnStatus.curtainState << "\n";
    LOG_DEBUG() << "[Api_Rcurtain_Field_SoaRctnStatus] rctnStatus.isNormal = " << (int)rctnStatus.isNormal << "\n";
#endif
}
