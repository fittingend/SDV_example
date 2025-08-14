#ifndef ___SOA_VEHICLEINFO_SKELETON_IMPL_HPP___
#define ___SOA_VEHICLEINFO_SKELETON_IMPL_HPP___

#include "eevp/control/soavehicleinfo_skeleton.h"
#include "ara/log/logger.h"

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

class SoaVehicleInfoSkeletonImpl : public eevp::control::skeleton::SoaVehicleInfoSkeleton
{
public:
	SoaVehicleInfoSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);
	~SoaVehicleInfoSkeletonImpl();

    void UpdateVehicleInfo(bool notify = true);


private:
	fields::soaVehicleInfo::FieldType mVehicleInfo;

	ara::core::Future<fields::soaVehicleInfo::FieldType> getter_VehicleInfo();
	
};
} // namespace control
} // namespace eevp
#endif // ___SOA_VEHICLEINFO_SKELETON_IMPL_HPP___