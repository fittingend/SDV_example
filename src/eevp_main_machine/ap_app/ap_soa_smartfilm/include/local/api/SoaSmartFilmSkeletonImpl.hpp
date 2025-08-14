#ifndef __SOA_SMARTFILM_SKELETON_IMPL_HPP__
#define __SOA_SMARTFILM_SKELETON_IMPL_HPP__

#include "eevp/control/soasmartfilm_skeleton.h"
#include "ara/log/logger.h"

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

class SoaSmartFilmSkeletonImpl : public eevp::control::skeleton::SoaSmartFilmSkeleton
{
public:
	SoaSmartFilmSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);
	~SoaSmartFilmSkeletonImpl();

	void SetOpacity(const eevp::control::SoaFilmPos& pos, const std::uint8_t& opacity) override;
	void SetAllOpacities(const std::uint8_t& opacity) override;
	void SetAuto(const bool& on) override;

	void UpdateDeviceNormal(bool notify = true);
	void UpdateSmtfilmAuto(bool notify = true);
	void UpdateSmtfilmOpacities(bool notify = true);
	void UpdateSwVersion(bool notify = true);

private:
	/// @brief Field
	fields::soaFilmDeviceNormal::FieldType mDeviceNormal;
	fields::soaFilmAuto::FieldType mSmtFilmAuto;
	fields::soaFilmOpacities::FieldType mSmtFilmOpacities;
	fields::soaFilmSwVersion::FieldType mSwVersion;

	/// @brief Logger
	ara::log::Logger& mLogger;

	ara::core::Future<fields::soaFilmDeviceNormal::FieldType> DeviceNormalGetter();
	ara::core::Future<fields::soaFilmAuto::FieldType> SmtfilmAutoGetter();
	ara::core::Future<fields::soaFilmOpacities::FieldType> SmtfilmOpacitiesGetter();
	ara::core::Future<fields::soaFilmSwVersion::FieldType> SwVersionGetter();
};
} // namespace control
} // name

#endif // __SOA_SMARTFILM_SKELETON_IMPL_HPP__