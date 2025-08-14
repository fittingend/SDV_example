#ifndef __SOA_MLM_SKELETON_IMPL_HPP__
#define __SOA_MLM_SKELETON_IMPL_HPP__

#include "eevp/control/soamlm_skeleton.h"
#include "ara/log/logger.h"

using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

class SoaMlmSkeletonImpl : public eevp::control::skeleton::SoaMlmSkeleton
{
public:
	SoaMlmSkeletonImpl(
		ara::core::InstanceSpecifier instanceSpec,
		ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);
	~SoaMlmSkeletonImpl();

	void RequestMlmSetBrightness(const std::uint16_t& brightness) override;
	void RequestMlmSetMode(const eevp::control::SoaMlmMoodMode& mood) override;
	void RequestMlmSetRgbColor(const std::uint8_t& colorTableIndex) override;

	void UpdateStatus(bool notify = true);

private:
	/// @brief Field
	fields::soaMlmStatus::FieldType mStatus;
	fields::soaMlmSwVersion::FieldType mSwVersion;

	/// @brief Logger
	ara::log::Logger& mLogger;

	ara::core::Future<fields::soaMlmStatus::FieldType> soaMlmStatusGetter();
	ara::core::Future<fields::soaMlmSwVersion::FieldType> soaMlmSwVersionGetter();
};
} // namespace control
} // namespace eevp




#endif