#include "SoaDriverSeatSkeletonImpl.hpp"
#include <SoaDriverSeatConverter.hpp>
#include <Api_SoaDriverSeat.hpp>
#include <VehicleContext.hpp>
#include <Common.h>
#include <SoaDataType.h>
#include <Log.hpp>


using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

SoaDriverSeatSkeletonImpl::SoaDriverSeatSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode) :
	SoaDriverSeatSkeleton(instanceSpec, mode) ,
	mLogger(ara::log::CreateLogger("SOA", "DriverSeat", ara::log::LogLevel::kInfo))
{
	auto soaDrvSeatDeviceNormal_getHandler = [this]() {
		return this->DeviceNormalGetter();
	};

	auto soaDrvSeatMotorDirection_getHandler = [this]() {
		return this->MotorDirectionGetter();
	};

	auto soaDrvSeatMotorPosition_getHandler = [this]() {
		return this->MotorPositionGetter();
	};

	auto soaDrvSeatSwVersion_getHandler = [this]() {
		return this->SwVersionGetter();
	};

	this->UpdateDeviceNormal(false);
	this->UpdateMotorDirection(false);
	this->UpdateMotorPosition(false);
	this->mSwVersion = (std::uint8_t)SW_VERSION;

	soaDrvSeatDeviceNormal.RegisterGetHandler(soaDrvSeatDeviceNormal_getHandler);
	soaDrvSeatMotorDirection.RegisterGetHandler(soaDrvSeatMotorDirection_getHandler);
	soaDrvSeatMotorPosition.RegisterGetHandler(soaDrvSeatMotorPosition_getHandler);
	soaDrvSeatSwVersion.RegisterGetHandler(soaDrvSeatSwVersion_getHandler);
}

ara::core::Future<fields::soaDrvSeatDeviceNormal::FieldType> SoaDriverSeatSkeletonImpl::DeviceNormalGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaDriverSeat.soaDrvSeatDeviceNormal.Get] (+)\n";
	ara::core::Promise<fields::soaDrvSeatDeviceNormal::FieldType> promise;
	promise.set_value(this->mDeviceNormal);
	return promise.get_future();
}

ara::core::Future<fields::soaDrvSeatMotorDirection::FieldType> SoaDriverSeatSkeletonImpl::MotorDirectionGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaDriverSeat.soaDrvSeatMotorDirection.Get] (+)\n";
	ara::core::Promise<fields::soaDrvSeatMotorDirection::FieldType> promise;
	promise.set_value(this->mMotorDirection);
	return promise.get_future();
}

ara::core::Future<fields::soaDrvSeatMotorPosition::FieldType> SoaDriverSeatSkeletonImpl::MotorPositionGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaDriverSeat.soaDrvSeatMotorPosition.Get] (+)\n";
	ara::core::Promise<fields::soaDrvSeatMotorPosition::FieldType> promise;
	promise.set_value(this->mMotorPosition);
	return promise.get_future();
}

ara::core::Future<fields::soaDrvSeatSwVersion::FieldType> SoaDriverSeatSkeletonImpl::SwVersionGetter()
{
	LOG_DEBUG() << "[eevp.control.SoaDriverSeat.soaDrvSeatSwVersion.Get] (+)\n";
	ara::core::Promise<fields::soaDrvSeatSwVersion::FieldType> promise;
	promise.set_value(this->mSwVersion);
	return promise.get_future();
}

void SoaDriverSeatSkeletonImpl::RequestMoveDrvSeatHeight(const eevp::control::SoaSeatHeightDirection& dir)
{
	LOG_DEBUG() << "[eevp.control.SoaDriverSeat.RequestMoveDrvSeatHeight] (+)\n";
	SOA_Drvseat_SetHeight(Convert_SeatHeightDir_API2SOA(dir));
	LOG_DEBUG() << "[eevp.control.SoaDriverSeat.RequestMoveDrvSeatHeight] (-)\n";
}

void SoaDriverSeatSkeletonImpl::RequestMoveDrvSeatRecline(const eevp::control::SoaSeatReclineDirection& dir)
{
	LOG_DEBUG() << "[eevp.control.SoaDriverSeat.RequestMoveDrvSeatRecline] (+)\n";
	SOA_Drvseat_SetRecline(Convert_SeatReclineDir_API2SOA(dir));
	LOG_DEBUG() << "[eevp.control.SoaDriverSeat.RequestMoveDrvSeatRecline] (-)\n";
}

void SoaDriverSeatSkeletonImpl::RequestMoveDrvSeatSlide(const eevp::control::SoaSeatSlideDirection& dir)
{
	LOG_DEBUG() << "[eevp.control.SoaDriverSeat.RequestMoveDrvSeatSlide] (+)\n";
	SOA_Drvseat_SetSlide(Convert_SeatSlideDir_API2SOA(dir));
	LOG_DEBUG() << "[eevp.control.SoaDriverSeat.RequestMoveDrvSeatSlide] (-)\n";
}

void SoaDriverSeatSkeletonImpl::RequestMoveDrvSeatTilt(const eevp::control::SoaSeatTiltDirection& dir)
{
	LOG_DEBUG() << "[eevp.control.SoaDriverSeat.RequestMoveDrvSeatTilt] (+)\n";
	SOA_Drvseat_SetTilt(Convert_SeatTiltDir_API2SOA(dir));
	LOG_DEBUG() << "[eevp.control.SoaDriverSeat.RequestMoveDrvSeatTilt] (-)\n";
}

void SoaDriverSeatSkeletonImpl::UpdateDeviceNormal(bool notify)
{
	LOG_DEBUG() << "(+)\n";

	VehicleContext *context = VehicleContext::GetInstance();
	bool isUpdated = false;

	fields::soaDrvSeatDeviceNormal::FieldType value = Convert_DeviceNormal_SOA2API(context->mDeviceNormal);
	LOG_DEBUG() << "- new value = " << (int)value << "\n";

	if (this->mDeviceNormal != value)
	{
		isUpdated = true;
		this->mDeviceNormal = value;
	}

	if ((notify) && (isUpdated))
	{
		LOG_DEBUG() << "[eevp.control.SoaDriverSeat.soaDrvSeatDeviceNormal.Notify] (+)\n";
		soaDrvSeatDeviceNormal.Update(this->mDeviceNormal);
	}

	LOG_DEBUG() << "(-)\n";
}

void SoaDriverSeatSkeletonImpl::UpdateMotorDirection(bool notify)
{
	LOG_DEBUG() << "(+)\n";

	VehicleContext *context = VehicleContext::GetInstance();
	fields::soaDrvSeatMotorDirection::FieldType value;

	value.slide = Convert_SeatSlideDir_SOA2API(context->mDirSlide);
	value.recline = Convert_SeatReclineDir_SOA2API(context->mDirRecline);
	value.height = Convert_SeatHeightDir_SOA2API(context->mDirHeight);
	value.tilt = Convert_SeatTiltDir_SOA2API(context->mDirTilt);

	switch (context->mDirTilt)
	{
	case eDriverSeatDir_Up:
		value.tilt = eevp::control::SoaSeatTiltDirection::kUP;
		break;
	case eDriverSeatDir_Down:
		value.tilt = eevp::control::SoaSeatTiltDirection::kDOWN;
		break;
	case eDriverSeatDir_Stop:
	default:
		value.tilt = eevp::control::SoaSeatTiltDirection::kSTOP;
		break;
	}

	if ((this->mMotorDirection.slide != value.slide) ||
		(this->mMotorDirection.recline != value.recline) ||
		(this->mMotorDirection.height != value.height) ||
		(this->mMotorDirection.tilt != value.tilt))
	{
		this->mMotorDirection = value;
		if (notify)
		{
			LOG_DEBUG() << "[eevp.control.SoaDriverSeat.soaDrvSeatMotorDirection.Notify]\n";
			soaDrvSeatMotorDirection.Update(this->mMotorDirection);
		}
	}

	LOG_DEBUG() << "(-)\n";
}

void SoaDriverSeatSkeletonImpl::UpdateMotorPosition(bool notify)
{
	LOG_DEBUG() << "(+)\n";

	VehicleContext *context = VehicleContext::GetInstance();
	fields::soaDrvSeatMotorPosition::FieldType value;

	value.slidePosition = (uint8_t)context->mPosSlide;
	value.reclinePosition = (uint8_t)context->mPosRecline;
	value.heightPosition = (uint8_t)context->mPosHeight;
	value.tiltPosition = (uint8_t)context->mPosTilt;

	if ((this->mMotorPosition.slidePosition != value.slidePosition) ||
		(this->mMotorPosition.reclinePosition != value.reclinePosition) ||
		(this->mMotorPosition.heightPosition != value.heightPosition) ||
		(this->mMotorPosition.tiltPosition != value.tiltPosition))
	{
		this->mMotorPosition = value;
		if (notify)
		{
			LOG_DEBUG() << "[eevp.control.SoaDriverSeat.soaDrvSeatMotorPosition.Notify]\n";
			soaDrvSeatMotorPosition.Update(this->mMotorPosition);
		}
	}

	LOG_DEBUG() << "(-)\n";
}

SoaDriverSeatSkeletonImpl::~SoaDriverSeatSkeletonImpl()
{
	StopOfferService();
}


} // namespace control
} // namespace eevp
