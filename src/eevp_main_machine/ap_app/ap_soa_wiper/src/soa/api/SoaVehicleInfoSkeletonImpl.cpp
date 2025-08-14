#include <Common.h>
#include <SoaDataType.h>
#include "SoaVehicleInfoConverter.hpp"
#include "SoaVehicleInfoSkeletonImpl.hpp"
#include <VehicleContext.hpp>

#include <Log.hpp>
#include <Debug_DataType2String.hpp>

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {
SoaVehicleInfoSkeletonImpl::SoaVehicleInfoSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode) :
	SoaVehicleInfoSkeleton(instanceSpec, mode)
{
	auto vehicleInfo_getHandler = [this]() {
		return this->getter_VehicleInfo();
	};

	this->UpdateVehicleInfo(false);

	soaVehicleInfo.RegisterGetHandler(vehicleInfo_getHandler);
}

ara::core::Future<fields::soaVehicleInfo::FieldType> SoaVehicleInfoSkeletonImpl::getter_VehicleInfo()
{
	ara::core::Promise<fields::soaVehicleInfo::FieldType> promise;
	promise.set_value(this->mVehicleInfo);
	return promise.get_future();
}

SoaVehicleInfoSkeletonImpl::~SoaVehicleInfoSkeletonImpl()
{
	StopOfferService();
}

void SoaVehicleInfoSkeletonImpl::UpdateVehicleInfo(bool notify)
{
	LOG_DEBUG() << "(+)\n";

	this->mVehicleInfo.speed = (std::uint16_t)VehicleContext::GetInstance()->mVehicleSpeed;
	this->mVehicleInfo.gearState = Convert_GearState_SOA2API(VehicleContext::GetInstance()->mGear);
	this->mVehicleInfo.accIn = Convert_AccState_SOA2API(VehicleContext::GetInstance()->mAcc);
	this->mVehicleInfo.ign1In = Convert_IgnState_SOA2API(VehicleContext::GetInstance()->mIgn1);
	this->mVehicleInfo.ign2In = Convert_IgnState_SOA2API(VehicleContext::GetInstance()->mIgn2);

	if (notify)
	{
		LOG_DEBUG() << "[eevp.control.SoaWiper.soaVehicleInfo] notify\n";
		soaVehicleInfo.Update(this->mVehicleInfo);
	}
	
	LOG_DEBUG() << "(-)\n";
}

} // namespace control
} // namespace eevp
