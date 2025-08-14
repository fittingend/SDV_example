#ifndef __SOA_MLM_SKELETON_IMPL_HPP__
#define __SOA_MLM_SKELETON_IMPL_HPP__

#include "eevp/control/soadriverseat_skeleton.h"
#include "ara/log/logger.h"

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

class SoaDriverSeatSkeletonImpl : public eevp::control::skeleton::SoaDriverSeatSkeleton
{
public:
	SoaDriverSeatSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);
	~SoaDriverSeatSkeletonImpl();

	void RequestMoveDrvSeatHeight(const eevp::control::SoaSeatHeightDirection& dir) override;
	void RequestMoveDrvSeatRecline(const eevp::control::SoaSeatReclineDirection& dir) override;
	void RequestMoveDrvSeatSlide(const eevp::control::SoaSeatSlideDirection& dir) override;
	void RequestMoveDrvSeatTilt(const eevp::control::SoaSeatTiltDirection& dir) override;

	void UpdateDeviceNormal(bool notify = true);
	void UpdateMotorDirection(bool notify = true);
	void UpdateMotorPosition(bool notify = true);

private:
	/// @brief Field
	fields::soaDrvSeatDeviceNormal::FieldType mDeviceNormal;
	fields::soaDrvSeatMotorDirection::FieldType mMotorDirection;
	fields::soaDrvSeatMotorPosition::FieldType mMotorPosition;
	fields::soaDrvSeatSwVersion::FieldType mSwVersion;

	/// @brief Logger
	ara::log::Logger& mLogger;

	ara::core::Future<fields::soaDrvSeatDeviceNormal::FieldType> DeviceNormalGetter();
	ara::core::Future<fields::soaDrvSeatMotorDirection::FieldType> MotorDirectionGetter();
	ara::core::Future<fields::soaDrvSeatMotorPosition::FieldType> MotorPositionGetter();
	ara::core::Future<fields::soaDrvSeatSwVersion::FieldType> SwVersionGetter();
};
} // namespace control
} // namespace eevp




#endif