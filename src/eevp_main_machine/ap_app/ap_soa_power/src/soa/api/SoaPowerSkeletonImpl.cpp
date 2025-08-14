#include "SoaPowerSkeletonImpl.hpp"
#include <Common.h>
#include <SoaDataType.h>
#include <VehicleContext.hpp>
#include <Log.hpp>

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

SoaPowerSkeletonImpl::SoaPowerSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode) :
	SoaPowerSkeleton(instanceSpec, mode)
{
	auto soaPowerDeviceNormal_getHandler = [this]() {
		return this->PowerNormalGetter();
	};

	auto soaPowerSwVersion_getHandler = [this]() {
		return this->SwVersionGetter();
	};

	VehicleContext *context = VehicleContext::GetInstance();
	switch (context->mTotalResult)
	{
	case eDeviceNormal_Ok:
		this->mPowerDeviceNormal = eevp::control::SoaDeviceIsNormal::kNORMAL;
		break;
	case eDeviceNormal_Abnormal:
		this->mPowerDeviceNormal = eevp::control::SoaDeviceIsNormal::kABNORMAL;
		break;
	default:
		this->mPowerDeviceNormal = eevp::control::SoaDeviceIsNormal::kABNORMAL;
		break;
	}

	this->mSwVersion = (std::uint8_t)SW_VERSION;

	soaPowerDeviceNormal.RegisterGetHandler(soaPowerDeviceNormal_getHandler);
	soaPowerSwVersion.RegisterGetHandler(soaPowerSwVersion_getHandler);
}

ara::core::Future<fields::soaPowerDeviceNormal::FieldType> SoaPowerSkeletonImpl::PowerNormalGetter()
{
	ara::core::Promise<fields::soaPowerDeviceNormal::FieldType> promise;
	promise.set_value(this->mPowerDeviceNormal);
	return promise.get_future();
}

ara::core::Future<fields::soaPowerSwVersion::FieldType> SoaPowerSkeletonImpl::SwVersionGetter()
{
	ara::core::Promise<fields::soaPowerSwVersion::FieldType> promise;
	promise.set_value(this->mSwVersion);
	return promise.get_future();
}

void SoaPowerSkeletonImpl::UpdatePowerDeviceNormal(void)
{
	VehicleContext *context = VehicleContext::GetInstance();
	fields::soaPowerDeviceNormal::FieldType value;

	if ((context->mZone1Connected != true) || (context->mZone1PowerNormal != eDeviceNormal_Ok))
	{
		context->mTotalResult = eDeviceNormal_Abnormal;
	}
	else if ((context->mZone2Connected != true) || (context->mZone2PowerNormal != eDeviceNormal_Ok))
	{
		context->mTotalResult = eDeviceNormal_Abnormal;
	}
	else if ((context->mZone3Connected != true) || (context->mZone3PowerNormal != eDeviceNormal_Ok))
	{
		context->mTotalResult = eDeviceNormal_Abnormal;
	}
	else if ((context->mZone4Connected != true) || (context->mZone4PowerNormal != eDeviceNormal_Ok))
	{
		context->mTotalResult = eDeviceNormal_Abnormal;
	}
	else
	{
		context->mTotalResult = eDeviceNormal_Ok;
	}

	switch (context->mTotalResult)
	{
	case eDeviceNormal_Ok:
		value = eevp::control::SoaDeviceIsNormal::kNORMAL;
		break;
	case eDeviceNormal_Abnormal:
	default:
		value = eevp::control::SoaDeviceIsNormal::kABNORMAL;
		break;
	}

	if (this->mPowerDeviceNormal != value)
	{
		this->mPowerDeviceNormal = value;
		soaPowerDeviceNormal.Update(this->mPowerDeviceNormal);
	}
}

SoaPowerSkeletonImpl::~SoaPowerSkeletonImpl() {
	StopOfferService();
}


} // namespace control
} // namespace eevp
