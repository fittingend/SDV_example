#include <Common.h>
#include <SoaDataType.h>
#include <SoaWiperConverter.hpp>
#include "SoaWiperSkeletonImpl.hpp"
#include <VehicleContext.hpp>
#include <Api_SoaWiper.hpp>

#include <Log.hpp>
#include <Debug_DataType2String.hpp>

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {
SoaWiperSkeletonImpl::SoaWiperSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode) :
	SoaWiperSkeleton(instanceSpec, mode) ,
	mLogger(ara::log::CreateLogger("SOA", "WIPER", ara::log::LogLevel::kInfo))
{
	auto deviceNormal_getHandler = [this]() {
		return this->getter_DeviceNormal();
	};

	auto wiperStatus_getHandler = [this]() {
		return this->getter_WiperStatus();
	};

	auto swVersion_getHandler = [this]() {
		return this->getter_SwVersion();
	};

	this->UpdateDeviceNormal(false);
	this->UpdateWiperStatus(false);
	this->mSwVersion = (std::uint8_t)SW_VERSION;

	soaWiperDeviceNormal.RegisterGetHandler(deviceNormal_getHandler);
	soaWiperStatus.RegisterGetHandler(wiperStatus_getHandler);
	soaWiperSwVersion.RegisterGetHandler(swVersion_getHandler);
}

ara::core::Future<fields::soaWiperDeviceNormal::FieldType> SoaWiperSkeletonImpl::getter_DeviceNormal()
{
	ara::core::Promise<fields::soaWiperDeviceNormal::FieldType> promise;
	promise.set_value(this->mDeviceNormal);
	return promise.get_future();
}

ara::core::Future<fields::soaWiperStatus::FieldType> SoaWiperSkeletonImpl::getter_WiperStatus()
{
	ara::core::Promise<fields::soaWiperStatus::FieldType> promise;
	promise.set_value(this->mWiperStatus);
	return promise.get_future();
}

ara::core::Future<fields::soaWiperSwVersion::FieldType> SoaWiperSkeletonImpl::getter_SwVersion()
{
	ara::core::Promise<fields::soaWiperSwVersion::FieldType> promise;
	promise.set_value(this->mSwVersion);
	return promise.get_future();
}

SoaWiperSkeletonImpl::~SoaWiperSkeletonImpl() {
	StopOfferService();
}

void SoaWiperSkeletonImpl::RequestWiperOperation(const eevp::control::SoaWiperMode& mode)
{
	LOG_DEBUG() << "(+)\n";
	SOA_Wiper_ControlWiper(Convert_WiperMode_API2SOA(mode));
	LOG_DEBUG() << "(-)\n";
}

void SoaWiperSkeletonImpl::SetWiperAutoSpeed(const bool& isAutoSpeed)
{
	LOG_DEBUG() << "(+)\n";
	SOA_Wiper_SetAutoSpeed(isAutoSpeed);
	LOG_DEBUG() << "(-)\n";
}

void SoaWiperSkeletonImpl::UpdateDeviceNormal(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	this->mDeviceNormal = Convert_DeviceNormal_SOA2API(VehicleContext::GetInstance()->mDeviceNormal);
	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaWiper.soaWiperDeviceNormal] notify\n";
		soaWiperDeviceNormal.Update(this->mDeviceNormal);
	}
	LOG_DEBUG() << "(-)\n";
}

void SoaWiperSkeletonImpl::UpdateWiperStatus(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	this->mWiperStatus.isAutoMode = VehicleContext::GetInstance()->mAutoMode;
	this->mWiperStatus.mode = Convert_WiperMode_SOA2API(VehicleContext::GetInstance()->mWiperOpMode);
	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaWiper.soaWiperStatus] notify\n";
		soaWiperStatus.Update(this->mWiperStatus);
	}
	LOG_DEBUG() << "(-)\n";
}

} // namespace control
} // namespace eevp
