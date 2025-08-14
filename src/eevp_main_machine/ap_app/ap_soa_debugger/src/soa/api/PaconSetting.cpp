#include <Common.h>
#include "ara/core/initialization.h"
#include "ara/exec/execution_client.h"
#include <PaconSetting.hpp>
#include <DriverSeatProxyImpl.h>
#include <MoodLampProxyImpl.h>
#include <RearCurtainProxyImpl.h>
#include <RoaProxyImpl.h>
#include <WiperProxyImpl.h>
#include <PowerProxyImpl.h>
#include <DmsProxyImpl.h>
#include <HvacProxyImpl.h>
#include <SmartFilmProxyImpl.h>
#include <Log.hpp>

//using namespace eevp::control;

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
}

PaconSetting::~PaconSetting()
{
    if (this->mCoreInitialized)
    {
        ara::core::Deinitialize();
    }
}

bool PaconSetting::StartPacon()
{
    LOG_INFO() << "(+)\n";
#if defined(DISABLE_SOMEIP)
    LOG_INFO() << "(-)\n";
    return true;
#else
    LOG_DEBUG() << "ara::core::Initialize() (+)\n";
    ara::core::Initialize();
    this->mCoreInitialized = true;
    LOG_DEBUG() << "ara::core::Initialize() (-)\n";

    LOG_DEBUG() << "ara::exec::ExecutionClient::ReportExecutionState (+)\n";
    ara::exec::ExecutionClient execClient{};
    ara::core::Result<void> result = execClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);
    if (!result)
    {
        LOG_ERROR() << "(-) execClient.ReportExecutionState failed\n";
        return false;
    }
    this->mExecutionStateReported = true;
    LOG_DEBUG() << "ara::exec::ExecutionClient::ReportExecutionState (-)\n";

    // Initialize
    auto *driverseat_inst = eevp::control::driverseat::DriverSeatProxyImpl::GetInstance();
    driverseat_inst->init();

    auto *moodlamp_inst = eevp::control::moodlamp::MoodLampProxyImpl::GetInstance();
    moodlamp_inst->init();

    auto *rearcurtain_inst = eevp::control::rearcurtain::RearCurtainProxyImpl::GetInstance();
    rearcurtain_inst->init();

    auto *roa_inst = eevp::control::roa::RoaProxyImpl::GetInstance();
    roa_inst->init();

    auto *wiper_inst = eevp::control::wiper::WiperProxyImpl::GetInstance();
    wiper_inst->init();

    auto *power_inst = eevp::control::power::PowerProxyImpl::GetInstance();
    power_inst->init();

    auto *dms_inst = eevp::control::dms::DmsProxyImpl::GetInstance();
    dms_inst->init();

    auto *hvac_inst = eevp::control::hvac::HvacProxyImpl::GetInstance();
    hvac_inst->init();

    auto *smtfilm_inst = eevp::control::smartfilm::SmartFilmProxyImpl::GetInstance();
    smtfilm_inst->init();

    LOG_INFO() << "(-)\n";
    return true;
#endif
}
