#pragma once
#include <cstdint>
#include <string>

namespace eevp {
namespace cloud {

// 서버 응답을 파싱하여 담을 구조체
struct CloudResponse {
    std::uint8_t recommendedOpacity; // 서버가 추천하는 새 투명도
    std::string messageToDriver;     // 운전자에게 보여줄 메시지
};

} // namespace cloud
} // namespace eevp
