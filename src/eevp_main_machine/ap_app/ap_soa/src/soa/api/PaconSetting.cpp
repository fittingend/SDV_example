#include <Common.h>
#include <SoaDataType.h>
#include "ara/core/initialization.h"
#include "ara/exec/execution_client.h"
#include "eevp/control/soarcurtain_common.h"
#include "eevp/control/soamlm_common.h"
#include "SoaRcurtainSkeletonImpl.hpp"
#include "SoaMlmSkeletonImpl.hpp"
#include <PaconSetting.hpp>
#include <Log.hpp>

using namespace eevp::control;

PaconSetting *PaconSetting::mInst = nullptr;

PaconSetting *PaconSetting::GetInstance()
{
    if (PaconSetting::mInst == nullptr)
    {
        PaconSetting::mInst = new PaconSetting();
    }

    return PaconSetting::mInst;
}

void PaconSetting::RemoveInstance()
{
    if (PaconSetting::mInst != nullptr)
    {
        delete PaconSetting::mInst;
        PaconSetting::mInst = nullptr;
    }
}

PaconSetting::PaconSetting()
{
    this->mCoreInitialized = false;
    this->mExecutionStateReported = false;
    this->mSoaRctn = nullptr;
    this->mSoaMlm = nullptr;
}

PaconSetting::~PaconSetting()
{
    if (this->mSoaRctn != nullptr)
    {
        delete this->mSoaRctn;
        this->mSoaRctn = nullptr;
    }

    if (this->mSoaMlm != nullptr)
    {
        delete this->mSoaMlm;
        this->mSoaMlm = nullptr;
    }

    if (this->mCoreInitialized)
    {
        ara::core::Deinitialize();
    }
}

bool PaconSetting::StartPacon()
{
    LOG_INFO() << "[PaconSetting::StartPacon] (+)\n";
#if defined(DISABLE_SOMEIP)
    LOG_INFO() << "[PaconSetting::StartPacon] (-)\n";
    return true;
#else
    LOG_DEBUG() << "[PaconSetting::StartPacon] ara::core::Initialize() (+)\n";
    ara::core::Initialize();
    this->mCoreInitialized = true;
    LOG_DEBUG() << "[PaconSetting::StartPacon] ara::core::Initialize() (-)\n";

    LOG_DEBUG() << "[PaconSetting::StartPacon] ara::exec::ExecutionClient::ReportExecutionState (+)\n";
    ara::exec::ExecutionClient execClient{};
    ara::core::Result<void> result = execClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);
    if (!result)
    {
        LOG_ERROR() << "[PaconSetting::StartPacon] (-) execClient.ReportExecutionState failed\n";
        return false;
    }
    this->mExecutionStateReported = true;
    LOG_DEBUG() << "[PaconSetting::StartPacon] ara::exec::ExecutionClient::ReportExecutionState (-)\n";

    this->setMoodLamp();
    this->setRearCurtain();


    LOG_INFO() << "[PaconSetting::StartPacon] (-)\n";
    return true;
#endif
}

void PaconSetting::setRearCurtain()
{
    LOG_DEBUG() << "[PaconSetting::setRearCurtain] (+)\n";
    ara::core::InstanceSpecifier specifier("SOA/AA/PPortRctn");
    this->mSoaRctn = new SoaRcurtainSkeletonImpl(specifier);
    this->mSoaRctn->OfferService();
    LOG_DEBUG() << "[PaconSetting::setRearCurtain] (-)\n";
}

void PaconSetting::setMoodLamp()
{
    LOG_DEBUG() << "[PaconSetting::setMoodLamp] (+)\n";
    ara::core::InstanceSpecifier specifier("SOA/AA/PPortMlm");
    this->mSoaMlm = new SoaMlmSkeletonImpl(specifier);
    this->mSoaMlm->OfferService();
    LOG_DEBUG() << "[PaconSetting::setMoodLamp] (-)\n";
}

eevp::control::SoaRcurtainSkeletonImpl *PaconSetting::GetSoaRcurtain()
{
    return this->mSoaRctn;
}

eevp::control::SoaMlmSkeletonImpl *PaconSetting::GetSoaMlm()
{
    return this->mSoaMlm;
}
