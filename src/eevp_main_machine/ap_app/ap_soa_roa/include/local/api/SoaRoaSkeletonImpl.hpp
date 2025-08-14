#ifndef __SOA_ROA_SKELETON_IMPL_HPP__
#define __SOA_ROA_SKELETON_IMPL_HPP__

#include "eevp/control/soaroa_skeleton.h"
#include "ara/log/logger.h"

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

class SoaRoaSkeletonImpl : public eevp::control::skeleton::SoaRoaSkeleton
{
public:
	SoaRoaSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);
	~SoaRoaSkeletonImpl();

	void StartRoa() override;
	void StopRoa() override;
	void ResetRoaDetectInfo() override;
	void SetRoaMode(const eevp::control::SoaRoaMode& mode) override;

	void UpdateDeviceNormal(bool notify = true);
	void UpdateRunningState(bool notify = true);
	void UpdateRoaMode(bool notify = true);
	void UpdateDetectState(bool notify = true);
	void UpdateDetectCount(bool notify = true);
	void UpdateSensorError(bool notify = true);

private:
	/// @brief Field
	fields::soaRoaDeviceNormal::FieldType mDeviceNormal;
	fields::soaRoaRunningState::FieldType mRunningState;
	fields::soaRoaMode::FieldType mMode;
	fields::soaRoaDetectState::FieldType mDetectState;
	fields::soaRoaDetectCount::FieldType mDetectCount;
	fields::soaRoaSensorError::FieldType mSensorError;
	fields::soaRoaSwVersion::FieldType mSwVersion;

	/// @brief Logger
	ara::log::Logger& mLogger;

	ara::core::Future<fields::soaRoaDeviceNormal::FieldType> getter_DeviceNormal();
	ara::core::Future<fields::soaRoaRunningState::FieldType> getter_RunningState();
	ara::core::Future<fields::soaRoaMode::FieldType> getter_Mode();
	ara::core::Future<fields::soaRoaDetectState::FieldType> getter_DetectState();
	ara::core::Future<fields::soaRoaDetectCount::FieldType> getter_DetectCount();
	ara::core::Future<fields::soaRoaSensorError::FieldType> getter_SensorError();
	ara::core::Future<fields::soaRoaSwVersion::FieldType> getter_SwVersion();

};
} // namespace control
} // namespace eevp
#endif