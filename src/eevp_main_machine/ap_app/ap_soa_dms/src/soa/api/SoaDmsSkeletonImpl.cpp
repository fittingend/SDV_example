#include "SoaDmsSkeletonImpl.hpp"
#include "SoaDmsConverter.hpp"
#include <Api_SoaDms.hpp>
#include <VehicleContext.hpp>
#include <Common.h>
#include <SoaDataType.h>
#include <Log.hpp>


using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

SoaDmsSkeletonImpl::SoaDmsSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode) :
	SoaDmsSkeleton(instanceSpec, mode) ,
	mLogger(ara::log::CreateLogger("SOA", "DMS", ara::log::LogLevel::kInfo))
{
	auto soaDmsDeviceNormal_getHandler = [this]() {
		return this->DeviceNormalGetter();
	};

	auto soaDmsStatus_getHandler = [this]() {
		return this->DmsStatusGetter();
	};

	auto soaDmsDriverStatus_getHandler = [this]() {
		return this->DriverStatusGetter();
	};

	auto soaDmsSwVersion_getHandler = [this]() {
		return this->SwVersionGetter();
	};

	this->UpdateDeviceNormal(false);
	this->UpdateDmsStatus(false);
	this->UpdateDriverStatus(false);
	this->UpdateSwVersion(false);

	soaDmsDeviceNormal.RegisterGetHandler(soaDmsDeviceNormal_getHandler);
	soaDmsStatus.RegisterGetHandler(soaDmsStatus_getHandler);
	soaDmsDriverStatus.RegisterGetHandler(soaDmsDriverStatus_getHandler);
	soaDmsSwVersion.RegisterGetHandler(soaDmsSwVersion_getHandler);
}

ara::core::Future<fields::soaDmsDeviceNormal::FieldType> SoaDmsSkeletonImpl::DeviceNormalGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaDms.soaDmsDeviceNormal.Get] (+)\n";
	ara::core::Promise<fields::soaDmsDeviceNormal::FieldType> promise;
	promise.set_value(this->mDeviceNormal);
	return promise.get_future();
}

ara::core::Future<fields::soaDmsStatus::FieldType> SoaDmsSkeletonImpl::DmsStatusGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaDms.soaDmsStatus.Get] (+)\n";
	ara::core::Promise<fields::soaDmsStatus::FieldType> promise;
	promise.set_value(this->mDmsStatus);
	return promise.get_future();
}

ara::core::Future<fields::soaDmsDriverStatus::FieldType> SoaDmsSkeletonImpl::DriverStatusGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaDms.soaDmsDriverStatus.Get] (+)\n";
	ara::core::Promise<fields::soaDmsDriverStatus::FieldType> promise;
	promise.set_value(this->mDriverStatus);
	return promise.get_future();
}

ara::core::Future<fields::soaDmsSwVersion::FieldType> SoaDmsSkeletonImpl::SwVersionGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaDms.soaDmsSwVersion.Get] (+)\n";
	ara::core::Promise<fields::soaDmsSwVersion::FieldType> promise;
	promise.set_value(this->mSwVersion);
	return promise.get_future();
}

void SoaDmsSkeletonImpl::SetDmsPower(const bool& on)
{
	LOG_DEBUG() << "(+)\n";
	SOA_DMS_SetPower(eClientLevel_GuiApps, on);
	LOG_DEBUG() << "(-)\n";
}

void SoaDmsSkeletonImpl::UpdateDeviceNormal(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();

	this->mDeviceNormal = Convert_DeviceNormal_SOA2API(context->mDeviceNormal);

	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaDms.soaDmsDeviceNormal.Notify] (+)\n";
		soaDmsDeviceNormal.Update(this->mDeviceNormal);
	}
	LOG_DEBUG() << "(-)\n";
}

void SoaDmsSkeletonImpl::UpdateDmsStatus(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();

	this->mDmsStatus.powerOn = context->mPowerOn;
	this->mDmsStatus.driverInside = Convert_DriverInside_SOA2API(context->mDriverInside);

	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaDms.soaDmsStatus.Notify] (+)\n";
		soaDmsStatus.Update(this->mDmsStatus);
	}

	LOG_DEBUG() << "(-)\n";
}

void SoaDmsSkeletonImpl::UpdateDriverStatus(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();

	this->mDriverStatus.gazingDir = Convert_GazingDir_SOA2API(context->mGazingDir);;
	this->mDriverStatus.fatigue = Convert_Fatigue_SOA2API(context->mFatigue);
	this->mDriverStatus.smoking = Convert_Smoke_SOA2API(context->mIsSmoking);
	this->mDriverStatus.onPhone = Convert_OnPhone_SOA2API(context->mIsOnPhone);

	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaDms.soaDmsDriverStatus.Notify] (+)\n";
		soaDmsDriverStatus.Update(this->mDriverStatus);
	}

	LOG_DEBUG() << "(-)\n";
}

void SoaDmsSkeletonImpl::UpdateSwVersion(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();

	this->mSwVersion = context->mSwVersion;

	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaDms.soaDmsSwVersion.Notify] (+)\n";
		soaDmsSwVersion.Update(this->mSwVersion);
	}

	LOG_DEBUG() << "(-)\n";
}

void SoaDmsSkeletonImpl::NotifyOnDriverNotFocusing()
{
	LOG_DEBUG() << "(+)\n";
	onDriverNotFocusing.Send(true);
	LOG_DEBUG() << "(-)\n";
}

void SoaDmsSkeletonImpl::NotifyOnDriverNotWaken()
{
	LOG_DEBUG() << "(+)\n";
	onDriverNotWaken.Send(true);
	LOG_DEBUG() << "(-)\n";
}

void SoaDmsSkeletonImpl::NotifyOnDriverNotWatchingFront()
{
	LOG_DEBUG() << "(+)\n";
	onDriverNotWatchingFront.Send(true);
	LOG_DEBUG() << "(-)\n";
}

SoaDmsSkeletonImpl::~SoaDmsSkeletonImpl()
{
	StopOfferService();
}


} // namespace control
} // namespace eevp
