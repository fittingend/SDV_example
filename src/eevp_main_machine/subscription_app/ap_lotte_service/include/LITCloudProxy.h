#pragma once

#include "CloudDataTypes.h"
#include "rest_api.h"
#include "eevp/control/soadms_proxy.h"
#include "eevp/subscription/type/impl_type_subscriptioninfo.h"
#include <ara/core/optional.h>
#include <memory>

namespace eevp {
namespace control {

class LITCloudProxy {
public:
    LITCloudProxy();
    bool init();

    // API 1: 시선 데이터 전송 및 응답 처리
    ara::core::Optional<eevp::cloud::CloudResponse> sendGazingData(const eevp::control::SoaDmsGazingDir& direction);

    // API 2: 구독 상태 이벤트 전송
    void sendSubscriptionEvent(const eevp::subscription::type::SubscriptionInfo& subInfo);

private:
    // API별로 RestApi 핸들러 분리
    std::unique_ptr<RestApi> mGazingApiHandler;
    std::unique_ptr<RestApi> mEventApiHandler;

    // API별로 URL 분리
    std::string mGazingApiUrl;
    std::string mEventApiUrl;

    ara::core::Optional<eevp::cloud::CloudResponse> parseResponse(const std::string& jsonResponse);
};

} // namespace control
} // namespace eevp
