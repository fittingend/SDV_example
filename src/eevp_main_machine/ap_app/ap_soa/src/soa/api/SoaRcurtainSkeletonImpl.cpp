#include "SoaRcurtainSkeletonImpl.hpp"
#include <Common.h>
#include <SoaDataType.h>
#include <ApiRcurtain_Local.hpp>
#include <Log.hpp>

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {
SoaRcurtainSkeletonImpl::SoaRcurtainSkeletonImpl(
        ara::core::InstanceSpecifier instanceSpec,
        ara::com::MethodCallProcessingMode mode) :
    SoaRcurtainSkeleton(instanceSpec, mode),
    mLogger(ara::log::CreateLogger("SOA", "RCTN", ara::log::LogLevel::kInfo))
{
    auto soaRctnStatus_get_handler = [this]() {
        return this->soaRctnStatusGetter();
    };

    soaRctnStatus.RegisterGetHandler(soaRctnStatus_get_handler);
}

ara::core::Future<fields::soaRctnStatus::FieldType> SoaRcurtainSkeletonImpl::soaRctnStatusGetter()
{
    LOG_DEBUG() << "[SoaRcurtainSkeletonImpl::soaRctnStatusGetter] (+)\n";
    ara::core::Promise<fields::soaRctnStatus::FieldType> promise;
    promise.set_value(this->mField);
    LOG_DEBUG() << "[SoaRcurtainSkeletonImpl::soaRctnStatusGetter] (-)\n";
    return promise.get_future();
}

SoaRcurtainSkeletonImpl::~SoaRcurtainSkeletonImpl() {
    StopOfferService();
}

ara::core::Future<SoaRcurtainSkeleton::RequestRearCurtainOperationOutput> SoaRcurtainSkeletonImpl::RequestRearCurtainOperation(const eevp::control::SoaRctnMotorDir& motorDir)
{
    RequestRearCurtainOperationOutput response;
    ara::core::Promise<RequestRearCurtainOperationOutput> promise;

    LOG_DEBUG() << "[SoaRcurtainSkeletonImpl::RequestRearCurtainOperation] (+)\n";

    RCtnSwitch_e eBtnValue;
    switch (motorDir)
    {
    case eevp::control::SoaRctnMotorDir::kDOWN:
        eBtnValue = eRCtnSwitch_OpenOn;
        break;

    case eevp::control::SoaRctnMotorDir::kUP:
        eBtnValue = eRCtnSwitch_CloseOn;
        break;

    case eevp::control::SoaRctnMotorDir::kSTOP:
    default:
        eBtnValue = eRCtnSwitch_Off;
        break;
    }

    int ret = Api_Rcurtain_Method_RequestRearCurtainOperation(eBtnValue);
    response.err = (ret == 0) ? eevp::control::SoaErrorState::kOK : eevp::control::SoaErrorState::kERROR;
    promise.set_value(response);

    LOG_DEBUG() << "[SoaRcurtainSkeletonImpl::RequestRearCurtainOperation] (-)\n";
    return promise.get_future();
}

void SoaRcurtainSkeletonImpl::SetSoaRctnStatus(eevp::control::SoaRctnStatus status)
{
    LOG_DEBUG() << "[SoaRcurtainSkeletonImpl::SetSoaRctnStatus] (+)\n";
    if ((this->mField.errorState != status.errorState) ||
        (this->mField.curMotorDirection != status.curMotorDirection) ||
        (this->mField.curtainState != status.curtainState) ||
        (this->mField.isNormal != status.isNormal))
    {
        this->mField = status;
        soaRctnStatus.Update(this->mField);
    }
    LOG_DEBUG() << "[SoaRcurtainSkeletonImpl::SetSoaRctnStatus] (-)\n";
}

} // namespace control
} // namespace eevp
