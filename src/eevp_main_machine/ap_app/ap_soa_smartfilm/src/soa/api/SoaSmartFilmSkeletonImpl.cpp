#include "SoaSmartFilmSkeletonImpl.hpp"
#include <Api_SoaSmartFilm.hpp>
#include <VehicleContext.hpp>
#include <Common.h>
#include <SoaDataType.h>
#include <SoaSmartFilmConverter.hpp>
#include <Log.hpp>
#include <Debug_DataType2String.hpp>

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

SoaSmartFilmSkeletonImpl::SoaSmartFilmSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode) :
	SoaSmartFilmSkeleton(instanceSpec, mode) ,
	mLogger(ara::log::CreateLogger("SOA", "SmtFilm", ara::log::LogLevel::kInfo))
{
	auto soaFilmDeviceNormal_getHandler = [this]() {
		return this->DeviceNormalGetter();
	};

	auto soaFilmAuto_getHandler = [this]() {
		return this->SmtfilmAutoGetter();
	};

	auto soaFilmOpacities_getHandler = [this]() {
		return this->SmtfilmOpacitiesGetter();
	};

	auto soaSwVersion_getHandler = [this]() {
		return this->SwVersionGetter();
	};

	this->UpdateDeviceNormal(false);
	this->UpdateSmtfilmAuto(false);
	this->UpdateSmtfilmOpacities(false);
	this->UpdateSwVersion(false);

	soaFilmDeviceNormal.RegisterGetHandler(soaFilmDeviceNormal_getHandler);
	soaFilmAuto.RegisterGetHandler(soaFilmAuto_getHandler);
	soaFilmOpacities.RegisterGetHandler(soaFilmOpacities_getHandler);
	soaFilmSwVersion.RegisterGetHandler(soaSwVersion_getHandler);
}

ara::core::Future<fields::soaFilmDeviceNormal::FieldType> SoaSmartFilmSkeletonImpl::DeviceNormalGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaSmartFilm.soaFilmDeviceNormal.Get] (+)\n";
	ara::core::Promise<fields::soaFilmDeviceNormal::FieldType> promise;
	promise.set_value(this->mDeviceNormal);
	return promise.get_future();
}

ara::core::Future<fields::soaFilmAuto::FieldType> SoaSmartFilmSkeletonImpl::SmtfilmAutoGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaSmartFilm.soaFilmAuto.Get] (+)\n";
	ara::core::Promise<fields::soaFilmAuto::FieldType> promise;
	promise.set_value(this->mSmtFilmAuto);
	return promise.get_future();
}

ara::core::Future<fields::soaFilmOpacities::FieldType> SoaSmartFilmSkeletonImpl::SmtfilmOpacitiesGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaSmartFilm.soaFilmOpacities.Get] (+)\n";
	ara::core::Promise<fields::soaFilmOpacities::FieldType> promise;
	promise.set_value(this->mSmtFilmOpacities);
	return promise.get_future();
}

ara::core::Future<fields::soaFilmSwVersion::FieldType> SoaSmartFilmSkeletonImpl::SwVersionGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaSmartFilm.soaFilmSwVersion.Get] (+)\n";
	ara::core::Promise<fields::soaFilmSwVersion::FieldType> promise;
	promise.set_value(this->mSwVersion);
	return promise.get_future();
}

void SoaSmartFilmSkeletonImpl::SetOpacity(const eevp::control::SoaFilmPos& pos, const std::uint8_t& opacity)
{
	SmtfilmPos_e _pos = Convert_SmtfilmPos_API2SOA(pos);
	int _opacity = (int)opacity;

	LOG_DEBUG() << "(+) pos=" << getStringEnum_SmtfilmPos_e(_pos) << ", opacity=" << _opacity << "\n";
	SOA_SMARTFILM_SetOpacity(eClientLevel_GuiApps, _pos, _opacity);
	LOG_DEBUG() << "(-)\n";
}

void SoaSmartFilmSkeletonImpl::SetAllOpacities(const std::uint8_t& opacity)
{
	int _opacity = (int)opacity;

	LOG_DEBUG() << "(+) opacity=" << _opacity << "\n";
	SOA_SMARTFILM_SetAllOpacity(eClientLevel_GuiApps, _opacity);
	LOG_DEBUG() << "(-)\n";
}

void SoaSmartFilmSkeletonImpl::SetAuto(const bool& on)
{
	LOG_DEBUG() << "(+) auto=" << getStringEnum_bool(on) << "\n";
	SOA_SMARTFILM_SetAuto(eClientLevel_GuiApps, on);
	LOG_DEBUG() << "(-)\n";
}

void SoaSmartFilmSkeletonImpl::UpdateDeviceNormal(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();
	bool isUpdated = false;

	fields::soaFilmDeviceNormal::FieldType value = Convert_DeviceNormal_SOA2API(context->mDeviceNormal);
	if (this->mDeviceNormal != value)
	{
		isUpdated = true;
		this->mDeviceNormal = value;
	}

	if ((notify) && (isUpdated))
	{
		LOG_DEBUG() << "[eevp.control.SoaSmartFilm.soaFilmDeviceNormal.Notify] (+)\n";
		soaFilmDeviceNormal.Update(this->mDeviceNormal);
	}
	LOG_DEBUG() << "(-)\n";
}

void SoaSmartFilmSkeletonImpl::UpdateSmtfilmAuto(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();
	bool isUpdated = false;

	fields::soaFilmAuto::FieldType value = context->mAuto;
	if (this->mSmtFilmAuto != value)
	{
		isUpdated = true;
		this->mSmtFilmAuto = value;
	}

	if ((notify) && (isUpdated))
	{
		LOG_DEBUG() << "[eevp.control.SoaSmartFilm.soaFilmAuto.Notify] (+)\n";
		soaFilmAuto.Update(this->mSmtFilmAuto);
	}
	LOG_DEBUG() << "(-)\n";
}

void SoaSmartFilmSkeletonImpl::UpdateSmtfilmOpacities(bool notify)
{
	LOG_DEBUG() << "(+)\n";
	VehicleContext *context = VehicleContext::GetInstance();
	fields::soaFilmOpacities::FieldType opacities;
	Convert_SmtfilmOpacity_SOA2API(context->mOpacities, opacities);
	bool isUpdated = CompareAndUpdate_SmtfilmOpacity_API(opacities, this->mSmtFilmOpacities);
	if ((notify) && (isUpdated))
	{
		LOG_DEBUG() << "[eevp.control.SoaSmartFilm.mSmtFilmOpacities.Notify] (+)\n";
		soaFilmOpacities.Update(this->mSmtFilmOpacities);
	}

	LOG_DEBUG() << "(-)\n";
}

void SoaSmartFilmSkeletonImpl::UpdateSwVersion(bool notify)
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
		LOG_DEBUG() << "[eevp.control.SoaSmartFilm.soaFilmSwVersion.Notify] (+)\n";
		soaFilmSwVersion.Update(this->mSwVersion);
	}
	LOG_DEBUG() << "(-)\n";
}


SoaSmartFilmSkeletonImpl::~SoaSmartFilmSkeletonImpl()
{
	StopOfferService();
}


} // namespace control
} // namespace eevp
