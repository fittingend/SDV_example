#include "SoaMlmSkeletonImpl.hpp"
#include <Common.h>
#include <SoaDataType.h>
#include <Api_SoaMoodLamp.hpp>
#include <SoaMlmConverter.hpp>

#include <VehicleContext.hpp>
#include <Log.hpp>
#include <Debug_DataType2String.hpp>

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

SoaMlmSkeletonImpl::SoaMlmSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode) :
	SoaMlmSkeleton(instanceSpec, mode) ,
	mLogger(ara::log::CreateLogger("SOA", "MLM", ara::log::LogLevel::kInfo))
{
	auto soaMlmStatus_get_handler = [this]() {
		return this->soaMlmStatusGetter();
	};

	auto soaMlmSwVersion_get_handler = [this]() {
		return this->soaMlmSwVersionGetter();
	};

	this->UpdateStatus(false);
	this->mSwVersion = (std::uint8_t)SW_VERSION;

	soaMlmStatus.RegisterGetHandler(soaMlmStatus_get_handler);
	soaMlmSwVersion.RegisterGetHandler(soaMlmSwVersion_get_handler);
}

ara::core::Future<fields::soaMlmStatus::FieldType> SoaMlmSkeletonImpl::soaMlmStatusGetter()
{
	LOG_DEBUG() << "[SoaMlmSkeletonImpl::soaMlmStatusGetter] (+)\n";
	ara::core::Promise<fields::soaMlmStatus::FieldType> promise;
	promise.set_value(this->mStatus);
	LOG_DEBUG() << "[SoaMlmSkeletonImpl::soaMlmStatusGetter] (-)\n";
	return promise.get_future();
}

ara::core::Future<fields::soaMlmSwVersion::FieldType> SoaMlmSkeletonImpl::soaMlmSwVersionGetter()
{
	LOG_DEBUG() << "[SoaMlmSkeletonImpl::soaMlmSwVersionGetter] (+)\n";
	ara::core::Promise<fields::soaMlmSwVersion::FieldType> promise;
	promise.set_value(this->mSwVersion);
	LOG_DEBUG() << "[SoaMlmSkeletonImpl::soaMlmSwVersionGetter] (-)\n";
	return promise.get_future();
}

SoaMlmSkeletonImpl::~SoaMlmSkeletonImpl() {
	StopOfferService();
}

void SoaMlmSkeletonImpl::RequestMlmSetBrightness(const std::uint16_t& brightness)
{
	LOG_DEBUG() << "(+) brightness=" << (int)brightness << "\n";
	SOA_MoodLamp_SetBrightness((int)brightness);
	LOG_DEBUG() << "(-)\n";
}

void SoaMlmSkeletonImpl::RequestMlmSetMode(const eevp::control::SoaMlmMoodMode& mood)
{
	MlmMoodMode_e mode = Convert_MoodMode_API2SOA(mood);

	LOG_DEBUG() << "(+) mood=" << getStringEnum_MlmMoodMode_e(mode) << "\n";
	SOA_MoodLamp_SetMode(mode);
	LOG_DEBUG() << "(-)\n";

}

void SoaMlmSkeletonImpl::RequestMlmSetRgbColor(const std::uint8_t& colorTableIndex)
{
	LOG_DEBUG() << "(+) colorTableIndex=" << (int)colorTableIndex << "\n";
	SOA_MoodLamp_SetColorIndex((int)colorTableIndex);
	LOG_DEBUG() << "(-)\n";
}

void SoaMlmSkeletonImpl::UpdateStatus(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();

	this->mStatus.colorTableIndex = (uint8_t)context->mMlmColorIndex;
	this->mStatus.brightness = (uint16_t)context->mMlmBrightness;
	this->mStatus.isNormal = Convert_DeviceNormal_SOA2API(context->mDeviceNormal);
	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaMlm.soaMlmStatus.Notify] (+)\n";
		soaMlmStatus.Update(this->mStatus);
	}

	LOG_DEBUG() << "(-)\n";
}

} // namespace control
} // namespace eevp
