#include "SoaRcurtainSkeletonImpl.hpp"
#include <Common.h>
#include <SoaDataType.h>
#include <SoaRcurtainConverter.hpp>
#include <Api_SoaRcurtain.hpp>
#include <VehicleContext.hpp>
#include <Log.hpp>
#include <Debug_DataType2String.hpp>

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

	auto soaRctnSwVersion_get_handler = [this]() {
		return this->soaRctnSwVersionGetter();
	};

	this->UpdateStatus(false);
	this->mSwVersion = (std::uint8_t)SW_VERSION;

	soaRctnStatus.RegisterGetHandler(soaRctnStatus_get_handler);
	soaRctnSwVersion.RegisterGetHandler(soaRctnSwVersion_get_handler);
}

ara::core::Future<fields::soaRctnStatus::FieldType> SoaRcurtainSkeletonImpl::soaRctnStatusGetter()
{
	LOG_DEBUG() << "(+)\n";
	ara::core::Promise<fields::soaRctnStatus::FieldType> promise;
	promise.set_value(this->mStatus);
	LOG_DEBUG() << "(-)\n";
	return promise.get_future();
}

ara::core::Future<fields::soaRctnSwVersion::FieldType> SoaRcurtainSkeletonImpl::soaRctnSwVersionGetter()
{
	LOG_DEBUG() << "(+)\n";
	ara::core::Promise<fields::soaRctnSwVersion::FieldType> promise;
	promise.set_value(this->mSwVersion);
	LOG_DEBUG() << "(-)\n";
	return promise.get_future();
}

SoaRcurtainSkeletonImpl::~SoaRcurtainSkeletonImpl() {
	StopOfferService();
}

ara::core::Future<SoaRcurtainSkeleton::RequestRearCurtainOperationOutput> SoaRcurtainSkeletonImpl::RequestRearCurtainOperation(const eevp::control::SoaRctnMotorDir& motorDir)
{
	RequestRearCurtainOperationOutput response;
	ara::core::Promise<RequestRearCurtainOperationOutput> promise;
	RCtnMotorDir_e eBtnValue = Convert_MotorDir_API2SOA(motorDir);

	LOG_DEBUG() << "(+) motorDir=" << getStringEnum_RCtnMotorDir_e(eBtnValue) << "\n";

	int ret = SOA_REARCURTAIN_MoveMotor(eBtnValue);
	response.err = (ret == 0) ? eevp::control::SoaErrorState::kOK : eevp::control::SoaErrorState::kERROR;
	promise.set_value(response);

	LOG_DEBUG() << "(-) ret=" << ret << "\n";
	return promise.get_future();
}

void SoaRcurtainSkeletonImpl::RequestRearCurtainPosition(const std::uint8_t& posPercentage)
{
	LOG_DEBUG() << "(+)\n";
	if (this->mSwVersion > 0)
	{
		SOA_REARCURTAIN_MovePosition((int)posPercentage);
	}
	LOG_DEBUG() << "(-)\n";
}

void SoaRcurtainSkeletonImpl::UpdateStatus(bool notify)
{
	VehicleContext *context = VehicleContext::GetInstance();
	this->mStatus.errorState = Convert_ErrorState_SOA2API(context->mErrorState);
	this->mStatus.curMotorDirection = Convert_MotorDir_SOA2API(context->mRctnMotorDir);
	this->mStatus.curtainState = Convert_RctnState_SOA2API(context->mRctnState);
	this->mStatus.isNormal = Convert_DeviceNormal_SOA2API(context->mIsNormal);

	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaRcurtain.soaRctnStatus.Notify] (+)\n";
		soaRctnStatus.Update(this->mStatus);
	}
}

} // namespace control
} // namespace eevp
