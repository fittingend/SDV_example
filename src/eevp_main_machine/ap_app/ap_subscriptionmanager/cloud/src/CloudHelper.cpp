#include "CloudHelper.h"

namespace eevp {
namespace subscription {
namespace cloud {

CloudHelper *CloudHelper::instance = NULL;

CloudHelper*
CloudHelper::getInstance() {
    if (instance == NULL) {
        instance = new CloudHelper();
    }

    return instance;
}

CloudHelper::CloudHelper() :
        mLogger(ara::log::CreateLogger("SUBM", "CHLP", ara::log::LogLevel::kInfo)) {
}

CloudHelper::~CloudHelper() {}

bool
CloudHelper::readConfig(std::string& loginurl, std::string& pushurl, std::string& pullurl, std::string& refreshurl, std::string& suburl) {
    mLogger.LogInfo() << __func__;

    std::ifstream inputFile(CONFIG_FILE);
    if (!inputFile.is_open()) {
        mLogger.LogInfo() << "Failed to open config.json";
        return false;
    }

    nlohmann::json j;
    try {
        inputFile >> j;
    } catch (const std::exception& e) {
        mLogger.LogInfo() << "Failed to parse JSON: " << e.what();
        return false;
    }

    try {
        serverUrl   = j.at("serverUrl").get<std::string>();
        vin         = j.at("vin").get<std::string>();
        password    = j.at("passWord").get<std::string>();
        localtest   = j.at("localtest").get<std::string>();
        loginurl    = j.at("loginurl").get<std::string>();
        pushurl     = j.at("pushurl").get<std::string>();
        pullurl     = j.at("pullurl").get<std::string>();
        refreshurl  = j.at("refreshurl").get<std::string>();
        suburl      = j.at("suburl").get<std::string>();

        mLogger.LogInfo() << "serverUrl: "  << serverUrl.c_str();
        mLogger.LogInfo() << "vin: "        << vin.c_str();
        mLogger.LogInfo() << "password: "   << password.c_str();
        mLogger.LogInfo() << "localtest: "  << localtest.c_str();
    } catch (const std::exception& e) {
        mLogger.LogInfo() << "Missing or invalid field: " << e.what();
        return false;
    }

    return true;
}

bool
CloudHelper::readSubInfo(std::vector<eevp::subscription::type::SubscriptionInfo>& list) {
    mLogger.LogInfo() << __func__;

    std::ifstream inputFile(SUBINFO_FILE);
    if (!inputFile.is_open()) {
        mLogger.LogInfo() << "Failed to open subinfo.json";
        return false;
    }

    nlohmann::json j;
    try {
        inputFile >> j;
    } catch (const std::exception& e) {
        mLogger.LogInfo() << "Failed to parse JSON: " << e.what();
        return false;
    }

    try {
        const auto& arr = j.at("values");
        for (const auto& el : arr) {
            if (!el.contains("appName") || !el.contains("status")) {
                mLogger.LogInfo() << "Missing key in element!";
                continue;
            }

            eevp::subscription::type::SubscriptionInfo info;
            info.appName        = el.at("appName").get<std::string>();
            info.isSubscription = el.at("status").get<bool>();
            list.push_back(info);
        }
    } catch (const std::exception& e) {
        mLogger.LogInfo() << "Missing or invalid field: " << e.what();
        return false;
    }

    return true;
}

bool
CloudHelper::getLoginInfo(std::string& _vin, std::string& _password) {
    mLogger.LogInfo() << __func__;
    if (!vin.empty() && !password.empty()) {
        _vin        = vin;
        _password   = password;
        return true;
    }
    return false;
}

bool
CloudHelper::saveSubInfo(std::vector<eevp::subscription::type::SubscriptionInfo> list) {
    mLogger.LogInfo() << __func__;

    try {
        nlohmann::json tempjson = nlohmann::json::array();
        for (const auto& info : list) {
            nlohmann::json obj;
            obj["appName"] = info.appName.c_str();
            obj["status"]  = info.isSubscription;
            tempjson.push_back(obj);
        }

        nlohmann::json savejson;
        savejson["values"] = tempjson;

        std::ofstream file(SUBINFO_FILE, std::ios::out | std::ios::trunc);
        if (file.is_open()) {
            file << savejson.dump(4);
            file.close();
        }

        sync();
        sync();
        sync();
    } catch (const std::exception& e) {
        mLogger.LogWarn() << e.what();
        return false;
    }

    return true;
}

} /// namespace cloud
} /// namespace subscription
} /// namespace eevp