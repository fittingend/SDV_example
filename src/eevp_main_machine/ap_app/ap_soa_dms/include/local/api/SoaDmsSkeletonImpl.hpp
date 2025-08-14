#ifndef __SOA_DMS_SKELETON_IMPL_HPP__
#define __SOA_DMS_SKELETON_IMPL_HPP__

#include "eevp/control/soadms_skeleton.h"
#include "ara/log/logger.h"

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

class SoaDmsSkeletonImpl : public eevp::control::skeleton::SoaDmsSkeleton
{
public:
	SoaDmsSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);
	~SoaDmsSkeletonImpl();

	void SetDmsPower(const bool& on) override;

	void UpdateDeviceNormal(bool notify = true);
	void UpdateDmsStatus(bool notify = true);
	void UpdateDriverStatus(bool notify = true);
	void UpdateSwVersion(bool notify = true);
	void NotifyOnDriverNotFocusing();
	void NotifyOnDriverNotWaken();
	void NotifyOnDriverNotWatchingFront();

private:
	/// @brief Field
	fields::soaDmsDeviceNormal::FieldType mDeviceNormal;
	fields::soaDmsStatus::FieldType mDmsStatus;
	fields::soaDmsDriverStatus::FieldType mDriverStatus;
	fields::soaDmsSwVersion::FieldType mSwVersion;

	/// @brief Logger
	ara::log::Logger& mLogger;

	ara::core::Future<fields::soaDmsDeviceNormal::FieldType> DeviceNormalGetter();
	ara::core::Future<fields::soaDmsStatus::FieldType> DmsStatusGetter();
	ara::core::Future<fields::soaDmsDriverStatus::FieldType> DriverStatusGetter();
	ara::core::Future<fields::soaDmsSwVersion::FieldType> SwVersionGetter();
};
} // namespace control
} // name

#endif // __SOA_DMS_SKELETON_IMPL_HPP__