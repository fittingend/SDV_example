#ifndef __SOA_HVAC_SKELETON_IMPL_HPP__
#define __SOA_HVAC_SKELETON_IMPL_HPP__

#include "eevp/control/soahvac_skeleton.h"
#include "ara/log/logger.h"

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

class SoaHvacSkeletonImpl : public eevp::control::skeleton::SoaHvacSkeleton
{
public:
	SoaHvacSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);
	~SoaHvacSkeletonImpl();

	void SetHvacPower(const bool& on) override;
	void SetAcOnOff(const bool& on) override;
	void SetAutoTemp(const bool& on) override;
	void SetSync(const bool& on) override;
	void SetDriverTemp(const std::int8_t& temperature) override;
	void SetPassengerTemp(const std::int8_t& temperature) override;
	void SetBlowingForce(const std::uint8_t& force) override;
	void SetBlowingDirection(const eevp::control::SoaHvacBlowDir& dir) override;
	void SetAirSource(const eevp::control::SoaHvacAirSource& src) override;
	void SetForwardDefrostOn(const bool& on) override;
	void SetBackwardDefrostOn(const bool& on) override;

	void UpdateDeviceNormal(bool notify = true);
	void UpdateHvacSetting(bool notify = true);
	void UpdateHvacStatus(bool notify = true);
	void UpdateHvacTemps(bool notify = true);
	void UpdateSwVersion(bool notify = true);

private:
	/// @brief Field
	fields::soaHvacDeviceNormal::FieldType mDeviceNormal;
	fields::soaHvacSetting::FieldType mSetting;
	fields::soaHvacStatus::FieldType mStatus;
	fields::soaHvacTemps::FieldType mTemps;
	fields::SoaHvacSwVersion::FieldType mSwVersion;

	/// @brief Logger
	ara::log::Logger& mLogger;

	ara::core::Future<fields::soaHvacDeviceNormal::FieldType> DeviceNormalGetter();
	ara::core::Future<fields::soaHvacSetting::FieldType> HvacSettingGetter();
	ara::core::Future<fields::soaHvacStatus::FieldType> HvacStatusGetter();
	ara::core::Future<fields::soaHvacTemps::FieldType> HvacTempsGetter();
	ara::core::Future<fields::SoaHvacSwVersion::FieldType> SwVersionGetter();
};
} // namespace control
} // name

#endif // __SOA_HVAC_SKELETON_IMPL_HPP__
