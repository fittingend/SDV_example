#include "SoaHvacSkeletonImpl.hpp"
#include <Api_SoaHvac.hpp>
#include <VehicleContext.hpp>
#include <Common.h>
#include <SoaDataType.h>
#include <SoaHvacConverter.hpp>
#include <Log.hpp>


using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

SoaHvacSkeletonImpl::SoaHvacSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode) :
	SoaHvacSkeleton(instanceSpec, mode) ,
	mLogger(ara::log::CreateLogger("SOA", "HVAC", ara::log::LogLevel::kInfo))
{
	auto soaHvacDeviceNormal_getHandler = [this]() {
		return this->DeviceNormalGetter();
	};

	auto soaHvacSetting_getHandler = [this]() {
		return this->HvacSettingGetter();
	};

	auto soaHvacStatus_getHandler = [this]() {
		return this->HvacStatusGetter();
	};

	auto soaHvacTemps_getHandler = [this]() {
		return this->HvacTempsGetter();
	};

	auto soaHvacSwVersion_getHandler = [this]() {
		return this->SwVersionGetter();
	};

	this->UpdateDeviceNormal(false);
	this->UpdateHvacSetting(false);
	this->UpdateHvacStatus(false);
	this->UpdateHvacTemps(false);
	this->UpdateSwVersion(false);

	soaHvacDeviceNormal.RegisterGetHandler(soaHvacDeviceNormal_getHandler);
	soaHvacSetting.RegisterGetHandler(soaHvacSetting_getHandler);
	soaHvacStatus.RegisterGetHandler(soaHvacStatus_getHandler);
	soaHvacTemps.RegisterGetHandler(soaHvacTemps_getHandler);
	SoaHvacSwVersion.RegisterGetHandler(soaHvacSwVersion_getHandler);
}

ara::core::Future<fields::soaHvacDeviceNormal::FieldType> SoaHvacSkeletonImpl::DeviceNormalGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaHvac.soaHvacDeviceNormal.Get] (+)\n";
	ara::core::Promise<fields::soaHvacDeviceNormal::FieldType> promise;
	promise.set_value(this->mDeviceNormal);
	return promise.get_future();
}

ara::core::Future<fields::soaHvacSetting::FieldType> SoaHvacSkeletonImpl::HvacSettingGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaHvac.soaHvacSetting.Get] (+)\n";
	ara::core::Promise<fields::soaHvacSetting::FieldType> promise;
	promise.set_value(this->mSetting);
	return promise.get_future();
}

ara::core::Future<fields::soaHvacStatus::FieldType> SoaHvacSkeletonImpl::HvacStatusGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaHvac.soaHvacStatus.Get] (+)\n";
	ara::core::Promise<fields::soaHvacStatus::FieldType> promise;
	promise.set_value(this->mStatus);
	return promise.get_future();
}

ara::core::Future<fields::soaHvacTemps::FieldType> SoaHvacSkeletonImpl::HvacTempsGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaHvac.soaHvacTemps.Get] (+)\n";
	ara::core::Promise<fields::soaHvacTemps::FieldType> promise;
	promise.set_value(this->mTemps);
	return promise.get_future();
}

ara::core::Future<fields::SoaHvacSwVersion::FieldType> SoaHvacSkeletonImpl::SwVersionGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaHvac.SoaHvacSwVersion.Get] (+)\n";
	ara::core::Promise<fields::SoaHvacSwVersion::FieldType> promise;
	promise.set_value(this->mSwVersion);
	return promise.get_future();
}

void SoaHvacSkeletonImpl::SetHvacPower(const bool& on)
{
	LOG_DEBUG() << "(+)\n";
	SOA_HVAC_SetHvacPower(eClientLevel_GuiApps, on);
	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::SetAcOnOff(const bool& on)
{
	LOG_DEBUG() << "(+)\n";
	SOA_HVAC_SetAcOnOff(eClientLevel_GuiApps, on);
	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::SetAutoTemp(const bool& on)
{
	LOG_DEBUG() << "(+)\n";
	SOA_HVAC_SetAutoTemp(eClientLevel_GuiApps, on);
	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::SetSync(const bool& on)
{
	LOG_DEBUG() << "(+)\n";
	SOA_HVAC_SetSync(eClientLevel_GuiApps, on);
	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::SetDriverTemp(const std::int8_t& temperature)
{
	LOG_DEBUG() << "(+)\n";
	SOA_HVAC_SetDriverTemp(eClientLevel_GuiApps, (int)temperature);
	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::SetPassengerTemp(const std::int8_t& temperature)
{
	LOG_DEBUG() << "(+)\n";
	SOA_HVAC_SetPassengerTemp(eClientLevel_GuiApps, (int)temperature);
	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::SetBlowingForce(const std::uint8_t& force)
{
	LOG_DEBUG() << "(+)\n";
	SOA_HVAC_SetBlowingForce(eClientLevel_GuiApps, (int)force);
	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::SetBlowingDirection(const eevp::control::SoaHvacBlowDir& dir)
{
	LOG_DEBUG() << "(+)\n";
	SOA_HVAC_SetBlowingDirection(eClientLevel_GuiApps, Convert_BlowDir_API2SOA(dir));
	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::SetAirSource(const eevp::control::SoaHvacAirSource& src)
{
	LOG_DEBUG() << "(+)\n";
	SOA_HVAC_SetAirSource(eClientLevel_GuiApps, Convert_AirSource_API2SOA(src));
	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::SetForwardDefrostOn(const bool& on)
{
	LOG_DEBUG() << "(+)\n";
	SOA_HVAC_SetForwardDefrostOn(eClientLevel_GuiApps, on);
	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::SetBackwardDefrostOn(const bool& on)
{
	LOG_DEBUG() << "(+)\n";
	SOA_HVAC_SetBackwardDefrostOn(eClientLevel_GuiApps, on);
	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::UpdateDeviceNormal(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();
	bool isUpdated = false;

	fields::soaHvacDeviceNormal::FieldType value = Convert_DeviceNormal_SOA2API(context->mDeviceNormal);
	if (this->mDeviceNormal != value)
	{
		isUpdated = true;
		this->mDeviceNormal = value;
	}

	if ((notify) && (isUpdated))
	{
		LOG_DEBUG() << "[eevp.control.SoaHvac.soaHvacDeviceNormal.Notify] (+)\n";
		soaHvacDeviceNormal.Update(this->mDeviceNormal);
	}
	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::UpdateHvacSetting(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();
	bool isUpdated = false;
	fields::soaHvacSetting::FieldType setting;
	Convert_HvacStatus_SOA2API(context->mSetting, setting);
	isUpdated = CompareAndUpdate_HvacStatus_API(setting, this->mSetting);
	if ((notify) && (isUpdated))
	{
		LOG_DEBUG() << "[eevp.control.SoaHvac.soaHvacSetting.Notify] (+)\n";
		soaHvacSetting.Update(this->mSetting);
	}

	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::UpdateHvacStatus(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();
	bool isUpdated = false;
	fields::soaHvacStatus::FieldType status;
	Convert_HvacStatus_SOA2API(context->mStatus, status);
	isUpdated = CompareAndUpdate_HvacStatus_API(status, this->mStatus);
	if ((notify) && (isUpdated))
	{
		LOG_DEBUG() << "[eevp.control.SoaHvac.soaHvacStatus.Notify] (+)\n";
		soaHvacStatus.Update(this->mStatus);
	}

	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::UpdateHvacTemps(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();
	bool isUpdated = false;
	fields::soaHvacTemps::FieldType temps;
	Convert_HvacTemps_SOA2API(context->mTemps, temps);
	isUpdated = CompareAndUpdate_HvacTemps_API(temps, this->mTemps);
	if ((notify) && (isUpdated))
	{
		LOG_DEBUG() << "[eevp.control.SoaHvac.soaHvacTemps.Notify] (+)\n";
		soaHvacTemps.Update(this->mTemps);
	}

	LOG_DEBUG() << "(-)\n";
}

void SoaHvacSkeletonImpl::UpdateSwVersion(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();
	bool isUpdated = false;

	if (this->mSwVersion != context->mSwVersion)
	{
		isUpdated = true;
		this->mSwVersion = context->mSwVersion;
	}

	if ((notify) && (isUpdated))
	{
		LOG_DEBUG() << "[eevp.control.SoaHvac.SoaHvacSwVersion.Notify] (+)\n";
		SoaHvacSwVersion.Update(this->mSwVersion);
	}
	LOG_DEBUG() << "(-)\n";
}


SoaHvacSkeletonImpl::~SoaHvacSkeletonImpl()
{
	StopOfferService();
}


} // namespace control
} // namespace eevp
