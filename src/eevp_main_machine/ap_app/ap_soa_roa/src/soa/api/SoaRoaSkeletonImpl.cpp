#include <Common.h>
#include <SoaDataType.h>
#include <VehicleContext.hpp>
#include <SoaRoaSkeletonImpl.hpp>
#include <SoaRoaConverter.hpp>
#include <Api_SoaRoa.hpp>

#include <Log.hpp>
#include <Debug_DataType2String.hpp>

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

SoaRoaSkeletonImpl::SoaRoaSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode) :
	SoaRoaSkeleton(instanceSpec, mode) ,
	mLogger(ara::log::CreateLogger("SOA", "ROA", ara::log::LogLevel::kInfo))
{
	auto deviceNormal_getHandler = [this]() {
		return this->getter_DeviceNormal();
	};

	auto runningState_getHandler = [this]() {
		return this->getter_RunningState();
	};

	auto mode_getHandler = [this]() {
		return this->getter_Mode();
	};

	auto detectState_getHandler = [this]() {
		return this->getter_DetectState();
	};

	auto detectCount_getHandler = [this]() {
		return this->getter_DetectCount();
	};

	auto sensorError_getHandler = [this]() {
		return this->getter_SensorError();
	};

	auto sw_getHandler = [this]() {
		return this->getter_SwVersion();
	};

	this->UpdateDeviceNormal(false);
	this->UpdateRunningState(false);
	this->UpdateRoaMode(false);
	this->UpdateDetectState(false);
	this->UpdateDetectCount(false);
	this->UpdateSensorError(false);
	this->mSwVersion = (std::uint8_t)SW_VERSION;

	soaRoaDeviceNormal.RegisterGetHandler(deviceNormal_getHandler);
	soaRoaRunningState.RegisterGetHandler(runningState_getHandler);
	soaRoaMode.RegisterGetHandler(mode_getHandler);
	soaRoaDetectState.RegisterGetHandler(detectState_getHandler);
	soaRoaDetectCount.RegisterGetHandler(detectCount_getHandler);
	soaRoaSensorError.RegisterGetHandler(sensorError_getHandler);
	soaRoaSwVersion.RegisterGetHandler(sw_getHandler);
}

ara::core::Future<fields::soaRoaDeviceNormal::FieldType> SoaRoaSkeletonImpl::getter_DeviceNormal()
{
	ara::core::Promise<fields::soaRoaDeviceNormal::FieldType> promise;
	promise.set_value(this->mDeviceNormal);
	return promise.get_future();
}

ara::core::Future<fields::soaRoaRunningState::FieldType> SoaRoaSkeletonImpl::getter_RunningState()
{
	ara::core::Promise<fields::soaRoaRunningState::FieldType> promise;
	promise.set_value(this->mRunningState);
	return promise.get_future();
}

ara::core::Future<fields::soaRoaMode::FieldType> SoaRoaSkeletonImpl::getter_Mode()
{
	ara::core::Promise<fields::soaRoaMode::FieldType> promise;
	promise.set_value(this->mMode);
	return promise.get_future();
}

ara::core::Future<fields::soaRoaDetectState::FieldType> SoaRoaSkeletonImpl::getter_DetectState()
{
	ara::core::Promise<fields::soaRoaDetectState::FieldType> promise;
	promise.set_value(this->mDetectState);
	return promise.get_future();
}

ara::core::Future<fields::soaRoaDetectCount::FieldType> SoaRoaSkeletonImpl::getter_DetectCount()
{
	ara::core::Promise<fields::soaRoaDetectCount::FieldType> promise;
	promise.set_value(this->mDetectCount);
	return promise.get_future();
}

ara::core::Future<fields::soaRoaSensorError::FieldType> SoaRoaSkeletonImpl::getter_SensorError()
{
	ara::core::Promise<fields::soaRoaSensorError::FieldType> promise;
	promise.set_value(this->mSensorError);
	return promise.get_future();
}

ara::core::Future<fields::soaRoaSwVersion::FieldType> SoaRoaSkeletonImpl::getter_SwVersion()
{
	ara::core::Promise<fields::soaRoaSwVersion::FieldType> promise;
	promise.set_value(this->mSwVersion);
	return promise.get_future();
}

SoaRoaSkeletonImpl::~SoaRoaSkeletonImpl() {
	StopOfferService();
}

void SoaRoaSkeletonImpl::StartRoa()
{
	LOG_DEBUG() << "(+)\n";
	SOA_ROA_Start();
	LOG_DEBUG() << "(-)\n";
}

void SoaRoaSkeletonImpl::StopRoa()
{
	LOG_DEBUG() << "(+)\n";
	SOA_ROA_Stop();
	LOG_DEBUG() << "(-)\n";
}

void SoaRoaSkeletonImpl::ResetRoaDetectInfo()
{
	LOG_DEBUG() << "(+)\n";
	SOA_ROA_Reset();
	LOG_DEBUG() << "(-)\n";
}

void SoaRoaSkeletonImpl::SetRoaMode(const eevp::control::SoaRoaMode& mode)
{
	LOG_DEBUG() << "(+)\n";
	SOA_ROA_SetMode(Convert_RoaMode_API2SOA(mode));
	LOG_DEBUG() << "(-)\n";
}

void SoaRoaSkeletonImpl::UpdateDeviceNormal(bool notify)
{
	LOG_DEBUG() << "(+)\n";

	this->mDeviceNormal = Convert_DeviceNormal_SOA2API(VehicleContext::GetInstance()->mIsNormal);
	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaRoa.soaRoaDeviceNormal.Notify] (+)\n";
		soaRoaDeviceNormal.Update(this->mDeviceNormal);
	}

	LOG_DEBUG() << "(-)\n";
}

void SoaRoaSkeletonImpl::UpdateRunningState(bool notify)
{
	LOG_DEBUG() << "(+)\n";

	this->mRunningState = Convert_RunningState_SOA2API(VehicleContext::GetInstance()->mRunningState);
	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaRoa.soaRoaRunningState.Notify] (+)\n";
		soaRoaRunningState.Update(this->mRunningState);
	}

	LOG_DEBUG() << "(-)\n";
}

void SoaRoaSkeletonImpl::UpdateRoaMode(bool notify)
{
	LOG_DEBUG() << "(+)\n";

	this->mMode = Convert_RoaMode_SOA2API(VehicleContext::GetInstance()->mRoaMode);
	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaRoa.soaRoaMode.Notify] (+)\n";
		soaRoaMode.Update(this->mMode);
	}

	LOG_DEBUG() << "(-)\n";
}

void SoaRoaSkeletonImpl::UpdateDetectState(bool notify)
{
	LOG_DEBUG() << "(+)\n";

	this->mDetectState = Convert_DetectState_SOA2API(VehicleContext::GetInstance()->mDetectCount);
	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaRoa.soaRoaDetectState.Notify] (+)\n";
		soaRoaDetectState.Update(this->mDetectState);
	}

	LOG_DEBUG() << "(-)\n";
}

void SoaRoaSkeletonImpl::UpdateDetectCount(bool notify)
{
	LOG_DEBUG() << "(+)\n";

	this->mDetectCount = (std::uint8_t)VehicleContext::GetInstance()->mDetectCount;
	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaRoa.soaRoaDetectCount.Notify] (+)\n";
		soaRoaDetectCount.Update(this->mDetectCount);
	}

	LOG_DEBUG() << "(-)\n";
}

void SoaRoaSkeletonImpl::UpdateSensorError(bool notify)
{
	LOG_DEBUG() << "(+)\n";

	this->mSensorError = Convert_SensorError_SOA2API(VehicleContext::GetInstance()->mSensorError);
	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaRoa.soaRoaSensorError.Notify] (+)\n";
		soaRoaSensorError.Update(this->mSensorError);
	}

	LOG_DEBUG() << "(-)\n";
}

} // namespace control
} // namespace eevp
