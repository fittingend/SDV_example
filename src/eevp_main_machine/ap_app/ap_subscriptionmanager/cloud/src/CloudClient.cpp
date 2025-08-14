#include "CloudClient.h"

namespace eevp {
namespace subscription {
namespace cloud {

CloudClient::CloudClient() :
        mLogger(ara::log::CreateLogger("SUBM", "CLUD", ara::log::LogLevel::kInfo)),
        accessToken(""),
        refreshToken("") {
    pCloudHelper = CloudHelper::getInstance();
}

CloudClient::~CloudClient() {
}

void
CloudClient::init() {
    pCloudHelper->readConfig(LOGINURL, PUSHURL, PULLURL, REFRESHURL, SUBURL);
    pCloudHelper->readSubInfo(savelist);
}

bool
CloudClient::Login() {
    mLogger.LogInfo() << __func__;

    if (!loginRestHandler.init()) {
        return false;
    }

    bool result = false;

    try {
        loginRestHandler.set_header_content("Content-Type", "application/json");

        std::string url = LOGINURL;
        std::string vin;
        std::string password;
        std::string response;

        if (pCloudHelper->getLoginInfo(vin, password)) {
            nlohmann::json loginData;
            loginData["vin"]       = vin;
            loginData["password"]  = password;

            std::string requestBody = loginData.dump();
            auto ret = loginRestHandler.post_request(url, requestBody, &response);
            if (ret == CURLE_OK) {
                nlohmann::json jsonResponse = nlohmann::json::parse(response);
                std::string status  = jsonResponse.at("status").get<std::string>();
                std::string message = jsonResponse.at("message").get<std::string>();
                mLogger.LogInfo() << "response : "  << "[status]"   << status.c_str()
                                                  << "[message]"  << message.c_str();

                if (status != "FAIL") {
                    auto value      = jsonResponse.at("values");
                    accessToken     = value.at("accessToken").get<std::string>();
                    refreshToken    = value.at("refreshToken").get<std::string>();
                    result = true;
                }
            }
        } else {
            mLogger.LogInfo() << __func__ << "config file error";
        }
    } catch (const std::exception& e) {
        mLogger.LogError() << __func__ << e.what();
    }

    loginRestHandler.clear();
    return result;
}

void
CloudClient::RefreshToken() {
    mLogger.LogInfo() << __func__;

    if (!refreshRestHandler.init()) {
        return;
    }

    try {
        refreshRestHandler.set_header_content("Content-Type", "application/json");

        std::string url = REFRESHURL;
        std::string response;

        nlohmann::json refreshData;
        refreshData["refreshToken"] = refreshToken;

        std::string requestBody = refreshData.dump();
        auto ret = refreshRestHandler.post_request(url, requestBody, &response);
        if (ret == CURLE_OK) {
            nlohmann::json jsonResponse = nlohmann::json::parse(response);
            std::string status  = jsonResponse.at("status").get<std::string>();
            std::string message = jsonResponse.at("message").get<std::string>();
            mLogger.LogInfo() << "response : "  << "[status]"   << status.c_str()
                                              << "[message]"  << message.c_str();

            if (status == "FAIL") {
                return;
            }

            auto value      = jsonResponse.at("values");
            accessToken     = value.at("accessToken").get<std::string>();
            refreshToken    = value.at("refreshToken").get<std::string>();
        }
    } catch (const std::exception& e) {
        mLogger.LogError() << __func__ << e.what();
    }

    refreshRestHandler.clear();
}

ServerResponse
CloudClient::PushInfo(eevp::ota::subscription::type::swClusterInfoList list) {
    mLogger.LogInfo() << __func__;

    ServerResponse result = ServerResponse::Error;

    if (!pushRestHandler.init()) {
        return result;
    }

    try {
        std::string bearer = "Bearer " + accessToken;
        pushRestHandler.set_header_content("Authorization", bearer);
        pushRestHandler.set_header_content("Content-Type", "application/json");

        std::string url = PUSHURL;
        std::string response;

        nlohmann::json pushData;
        for (const auto& info : list) {
            pushData["appDetails"].push_back({
                { "appName",    std::string(info.appName.c_str())  },
                { "appVersion", std::string(info.version.c_str())  }
            });
        }

        std::string requestBody = pushData.dump();
        auto ret = pushRestHandler.post_request(url, requestBody, &response);
        if (ret == CURLE_OK) {
            nlohmann::json jsonResponse = nlohmann::json::parse(response);
            std::string status  = jsonResponse.at("status").get<std::string>();
            std::string message = jsonResponse.at("message").get<std::string>();
            mLogger.LogInfo() << "response : "  << "[status]"   << status.c_str()
                                                    << "[message]"  << message.c_str();

            if (message == "The requested token is invalid.")
                result = ServerResponse::TokenExpired;
            else if (status == "FAIL")
                result = ServerResponse::Error;
            else
                result = ServerResponse::Ok;
        } else {
            result = ServerResponse::Error;
        }
    } catch (const std::exception& e) {
        mLogger.LogError() << __func__ << e.what();
        result = ServerResponse::Error;
    }

    pushRestHandler.clear();
    return result;
}

ServerResponse
CloudClient::PullInfo(
                eevp::type::String& _accessToken,
                eevp::type::String& _refreshToken,
                eevp::ota::subscription::type::downloadPackageInfoList& _list) {
    mLogger.LogInfo() << __func__;
    _list.clear();

    ServerResponse result = ServerResponse::Error;

    if (!pullRestHandler.init()) {
        return result;
    }

    try {
        std::string bearer = "Bearer " + accessToken;
        pullRestHandler.set_header_content("Authorization", bearer);
        pullRestHandler.set_header_content("Content-Type", "application/json");

        std::string url = PULLURL;
        std::string response;

        auto ret = pullRestHandler.get_request(url, &response);
        if (ret == CURLE_OK) {
            nlohmann::json jsonResponse = nlohmann::json::parse(response);
            std::string status  = jsonResponse.at("status").get<std::string>();
            std::string message = jsonResponse.at("message").get<std::string>();
            mLogger.LogInfo() << "response : "  << "[status]"   << status.c_str()
                                                << "[message]"  << message.c_str();

            if (message == "The requested token is invalid.") {
                result = ServerResponse::TokenExpired;
            }
            else if (status == "FAIL") {
                result = ServerResponse::Error;
            }
            else {
                const auto& arr = jsonResponse["values"];
                for (const auto& el : arr) {
                    eevp::type::String appName     = el.value("appName", "");
                    eevp::type::String actionId    = el.value("actionId", "");
                    eevp::type::String actionType  = el.value("actionType", "");
                    eevp::type::String version     = el.value("version", "");
                    std::uint32_t softwareType     = el.value("softwareType", 0u);

                    eevp::type::String jsonFileName, jsonDownloadUrl, jsonHash;
                    eevp::type::String tarFileName,  tarDownloadUrl,  tarHash;

                    if (el.contains("appFiles") && el["appFiles"].is_array()) {
                        for (const auto& fileEl : el["appFiles"]) {
                            std::string fileName    = fileEl.value("fileName", "");
                            std::string fileType    = fileEl.value("appFileType", "");
                            std::string downloadUrl = fileEl.value("downloadUrl", "");
                            std::string hash        = fileEl.value("hash", "");

                            if (fileType.find("TAR")         != std::string::npos) {
                                tarDownloadUrl  = downloadUrl;
                                tarHash         = hash;
                                tarFileName     = fileName;
                            } else if (fileType.find("JSON") != std::string::npos) {
                                jsonDownloadUrl = downloadUrl;
                                jsonHash        = hash;
                                jsonFileName    = fileName;
                            }
                        }
                    }
                    eevp::ota::subscription::type::downloadPackageInfo info;
                    info.appName            = appName;
                    info.actionId           = actionId;
                    info.actionType         = actionType;
                    info.version            = version;
                    info.softwareType       = softwareType;
                    info.tarDownloadUrl     = tarDownloadUrl;
                    info.tarHash            = tarHash;
                    info.tarFileName        = tarFileName;
                    info.jsonDownloadUrl    = jsonDownloadUrl;
                    info.jsonHash           = jsonHash;
                    info.jsonFileName       = jsonFileName;
                    _list.push_back(info);
                }

                _accessToken  = accessToken;
                _refreshToken = refreshToken;
                result = ServerResponse::Ok;
            }
        }
    } catch (const std::exception& e) {
        mLogger.LogError() << __func__ << e.what();
        result = ServerResponse::Error;
    }

    pullRestHandler.clear();
    return result;
}


ServerResponse
CloudClient::SubInfo(std::vector<eevp::subscription::type::SubscriptionInfo>& list) {
    mLogger.LogInfo() << __func__;

    ServerResponse result = ServerResponse::Error;

    if (!subRestHandler.init()) {
        list = savelist;
        return result;
    }

    try {
        std::string bearer = "Bearer " + accessToken;
        subRestHandler.set_header_content("Authorization", bearer);
        subRestHandler.set_header_content("Content-Type", "application/json");

        std::string url = SUBURL;
        std::string response;

        auto ret = subRestHandler.get_request(url, &response);
        if (ret == CURLE_OK) {
            nlohmann::json jsonResponse = nlohmann::json::parse(response);
            std::string status  = jsonResponse.at("status").get<std::string>();
            std::string message = jsonResponse.at("message").get<std::string>();
            mLogger.LogInfo() << "response : "  << "[status]"   << status.c_str()
                                                << "[message]"  << message.c_str();

            if (message == "The requested token is invalid.") {
                list = savelist;
                result = ServerResponse::TokenExpired;
            }
            else if (status == "FAIL") {
                list = savelist;
                result = ServerResponse::Error;
            }
            else {
                const auto& arr = jsonResponse["values"];
                std::vector<eevp::subscription::type::SubscriptionInfo> templist;

                for (const auto& el : arr) {
                    eevp::type::String appName     = el.value("appName", "");
                    eevp::type::String status      = el.value("status", "");

                    eevp::subscription::type::SubscriptionInfo info;
                    info.appName            = appName;
                    info.isSubscription     = (status == "ACTIVE");
                    templist.push_back(info);
                }

                if (!templist.empty()) {
                    savelist = templist;
                }

                list = savelist;
                pCloudHelper->saveSubInfo(list);
                result = ServerResponse::Ok;
            }
        }
    } catch (const std::exception& e) {
        mLogger.LogError() << __func__ << e.what();
    }

    subRestHandler.clear();
    return result;
}

} /// namespace cloud
} /// namespace subscription
} /// namespace eevp
