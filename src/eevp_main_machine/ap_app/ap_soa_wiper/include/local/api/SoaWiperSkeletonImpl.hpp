#ifndef __SOA_WIPER_SKELETON_IMPL_HPP__
#define __SOA_WIPER_SKELETON_IMPL_HPP__

#include "eevp/control/soawiper_skeleton.h"
#include "ara/log/logger.h"

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

class SoaWiperSkeletonImpl : public eevp::control::skeleton::SoaWiperSkeleton
{
public:
	SoaWiperSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);
	~SoaWiperSkeletonImpl();

	void RequestWiperOperation(const eevp::control::SoaWiperMode& mode) override;
	void SetWiperAutoSpeed(const bool& isAutoSpeed) override;

	void UpdateDeviceNormal(bool notify = true);
	void UpdateWiperStatus(bool notify = true);

private:
	/// @brief Field
	fields::soaWiperDeviceNormal::FieldType mDeviceNormal;
	fields::soaWiperStatus::FieldType mWiperStatus;
	fields::soaWiperSwVersion::FieldType mSwVersion;

	/// @brief Logger
	ara::log::Logger& mLogger;

	ara::core::Future<fields::soaWiperDeviceNormal::FieldType> getter_DeviceNormal();
	ara::core::Future<fields::soaWiperStatus::FieldType> getter_WiperStatus();
	ara::core::Future<fields::soaWiperSwVersion::FieldType> getter_SwVersion();
};
} // namespace control
} // namespace eevp
#endif