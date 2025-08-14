#ifndef __SOA_POWER_SKELETON_IMPL_HPP__
#define __SOA_POWER_SKELETON_IMPL_HPP__

#include "eevp/control/soapower_skeleton.h"

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

class SoaPowerSkeletonImpl : public eevp::control::skeleton::SoaPowerSkeleton
{
public:
	SoaPowerSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);
	~SoaPowerSkeletonImpl();

	void UpdatePowerDeviceNormal(void);

private:
	/// @brief Field
	fields::soaPowerDeviceNormal::FieldType mPowerDeviceNormal;
	fields::soaPowerSwVersion::FieldType mSwVersion;

	ara::core::Future<fields::soaPowerDeviceNormal::FieldType> PowerNormalGetter();
	ara::core::Future<fields::soaPowerSwVersion::FieldType> SwVersionGetter();
};
} // namespace control
} // namespace eevp




#endif // __SOA_POWER_SKELETON_IMPL_HPP__