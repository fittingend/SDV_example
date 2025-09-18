/**
 * @file LITCloudProxy.cpp
 * @brief 클라우드 서버와의 통신을 담당하는 프록시 클래스의 구현부
 */
#include "LITCloudProxy.h"
#include <ara/log/logger.h>

namespace eevp {
namespace control {

/**
 * @brief 생성자에서 각 API 통신을 담당할 RestApi 핸들러들을 생성합니다.
 */
LITCloudProxy::LITCloudProxy() {
    // API 1: 시선 데이터 전송용 핸들러
    mGazingApiHandler = std::make_unique<RestApi>();
    // API 2: 이벤트 전송용 핸들러
    mEventApiHandler = std::make_unique<RestApi>();
}

/**
 * @brief 각 RestApi 핸들러를 초기화하고, 통신할 서버의 URL을 설정합니다.
 * @return 모든 핸들러가 성공적으로 초기화되면 true를 반환합니다.
 */
bool LITCloudProxy::init() {
    // 1. 통신할 서버의 기본 URL을 설정합니다.
    std::string baseUrl = "http://your.cloud.server";
    // 2. 각 API의 전체 URL 경로를 설정합니다.
    mGazingApiUrl = baseUrl + "/api/gazing";
    mEventApiUrl = baseUrl + "/api/event";

    // 3. 각 RestApi 핸들러의 내부 (libcurl)를 초기화합니다.
    bool b1 = mGazingApiHandler->init();
    bool b2 = mEventApiHandler->init();
    return b1 && b2;
}

/**
 * @brief 운전자의 시선 데이터를 서버로 전송하고, 서버의 응답을 반환합니다.
 * @param direction 운전자의 시선 방향 데이터
 * @return 서버로부터 받은 응답을 CloudResponse 구조체에 담아 반환합니다. 실패 시 비어있는 Optional을 반환합니다.
 */
ara::core::Optional<eevp::cloud::CloudResponse> LITCloudProxy::sendGazingData(const eevp::control::SoaDmsGazingDir& direction) {
    // 1. 서버로 보낼 데이터를 JSON 형식의 문자열로 만듭니다.
    std::string jsonData = "{ \"gazingDirection\": " + std::to_string(static_cast<int>(direction)) + " }";
    // 2. 서버의 응답 본문을 저장할 문자열 변수를 선언합니다.
    std::string responseBody;

    // 3. HTTP 요청 헤더를 설정합니다. (Body가 JSON 형식임을 명시)
    mGazingApiHandler->set_header_content("Content-Type", "application/json");
    // 4. RestApi 핸들러를 사용하여 서버에 POST 요청을 보냅니다.
    int http_code = mGazingApiHandler->post_request(mGazingApiUrl, jsonData, &responseBody);

    // 5. HTTP 응답 코드가 200 (성공)이고, 응답 본문이 비어있지 않은지 확인합니다.
    if (http_code == 200 && !responseBody.empty()) {
        // 6. 응답 본문(JSON)을 파싱하여 구조체로 변환한 뒤 반환합니다.
        return parseResponse(responseBody);
    }

    // 7. 통신에 실패했거나 응답이 비정상이면 비어있는 Optional을 반환합니다.
    return ara::core::nullopt;
}

/**
 * @brief 구독 상태 변경 이벤트를 서버로 전송합니다.
 * @param subInfo 구독 정보가 담긴 구조체
 */
void LITCloudProxy::sendSubscriptionEvent(const eevp::subscription::type::SubscriptionInfo& subInfo) {
    // 1. 구독 변경 상태를 JSON 형식의 문자열로 만듭니다.
    std::string jsonData = "{ \"appName\": \"" + subInfo.appName + "\", \"isSubscribed\": " + (subInfo.isSubscription ? "true" : "false") + " }";
    std::string responseBody; // 응답을 받을 변수 (여기서는 사용하지 않음)

    // 2. 이벤트 전송용 핸들러에 헤더를 설정하고 POST 요청을 보냅니다.
    mEventApiHandler->set_header_content("Content-Type", "application/json");
    mEventApiHandler->post_request(mEventApiUrl, jsonData, &responseBody);
}

/**
 * @brief 서버로부터 받은 JSON 응답 문자열을 CloudResponse 구조체로 파싱(변환)합니다.
 * @param jsonResponse 서버가 보낸 JSON 형식의 문자열
 * @return 파싱에 성공하면 데이터가 채워진 CloudResponse 구조체를, 실패하면 비어있는 Optional을 반환합니다.
 */
ara::core::Optional<eevp::cloud::CloudResponse> LITCloudProxy::parseResponse(const std::string& jsonResponse) {
    try {
        // 중요: 실제 프로젝트에서는 nlohmann/json 같은 검증된 JSON 라이브러리를 사용하여
        //       안전하게 파싱해야 합니다.
        // 예시: auto json = nlohmann::json::parse(jsonResponse);
        //       result.recommendedOpacity = json.at("recommendedOpacity");

        // 아래는 라이브러리 없이 파싱을 흉내 낸 테스트용 코드입니다.
        eevp::cloud::CloudResponse result;
        result.recommendedOpacity = 70; 
        result.messageToDriver = "Look Ahead!";
        return result;
    } catch (...) {
        // JSON 파싱 중 에러가 발생하면 비어있는 Optional을 반환합니다.
        return ara::core::nullopt;
    }
}

} // namespace control
} // namespace eevp
