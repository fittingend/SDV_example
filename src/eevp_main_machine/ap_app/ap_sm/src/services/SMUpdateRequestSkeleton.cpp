/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#include <Log.hpp>
#include <services/SMUpdateRequestSkeleton.hpp>
#include <services/StateManager.hpp>

using namespace ara::sm;
using namespace ara::sm::skeleton;
using namespace state_manager;

const ara::core::InstanceSpecifier kRootSwComponent{"SM/PARA/UpdateRequest"};

namespace sm_update_request
{

SMUpdateRequestSkeleton::SMUpdateRequestSkeleton()
    : UpdateRequestSkeleton(kRootSwComponent, ara::com::MethodCallProcessingMode::kEventSingleThread),
    in_session_{false}
{
}

SMUpdateRequestSkeleton::~SMUpdateRequestSkeleton()
{
}

bool SMUpdateRequestSkeleton::Initialize()
{
    auto result = OfferService();
    if (!result.HasValue()) {
        logger().LogWarn() << "Failed to OfferSerivce with " << result.Error().Message();
        return false;
    }

    return true;
}

void SMUpdateRequestSkeleton::Release()
{
    StopOfferService();
}

ara::core::Future<UpdateRequestSkeleton::PrepareRollbackOutput>
SMUpdateRequestSkeleton::PrepareRollback(const FunctionGroupList& fgList)
{
    // Application Errors
    //  kRejected : Requested operation was rejected due to State Managements/machines internal state.
    //  kRollbackFailed : Rollback step of update failed.

    logger().LogDebug() << "Called PrepareRollback";
    ara::core::Promise<PrepareRollbackOutput> p;
    PrepareRollbackOutput output{};

    if (!in_session_) {
        logger().LogWarn() << "Called PrepareRollback, but it's not in session";
        p.SetError(ara::core::ErrorCode(SMErrorDomainErrc::kRejected));
    } else {
        auto it = std::find_if(fgList.begin(), fgList.end(), [](ara::sm::FGNameType v) {
            return v == kMachineFg;
        });
        if (it != fgList.end()) {
            // No plan to change MachinFG to Off state
            logger().LogWarn() << "No allow to change MachineFG to Off";
            p.SetError(ara::core::ErrorCode(SMErrorDomainErrc::kRejected));
        } else {
            for (const auto& it: fgList) {
                ErrorCode ret = StateManager::getInstance().RequestMode(it, kOff);
                if (ret == kSuccess) {
                    p.set_value(output);
                } else {
                    p.SetError(ara::core::ErrorCode(SMErrorDomainErrc::kRollbackFailed));
                }
            }
        }
    }

    return p.get_future();
}

ara::core::Future<UpdateRequestSkeleton::PrepareUpdateOutput>
SMUpdateRequestSkeleton::PrepareUpdate(const FunctionGroupList& fgList)
{
    // Application Errors
    //  kRejected : Requested operation was rejected due to State Managements/machines internal state.
    //  kPrepareFailed : Preparation step of update failed.

    logger().LogDebug() << "Called PrepareUpdate";
    ara::core::Promise<PrepareUpdateOutput> p;
    PrepareUpdateOutput output{};

    if (!in_session_) {
        logger().LogWarn() << "Called PrepareUpdate, but it's not in session";
        p.SetError(ara::core::ErrorCode(SMErrorDomainErrc::kRejected));
    } else {
        auto it = std::find_if(fgList.begin(), fgList.end(), [](ara::sm::FGNameType v) {
            return v == kMachineFg;
        });
        if (it != fgList.end()) {
            // No plan to change MachinFG to Off state
            logger().LogWarn() << "No allow to change MachineFG to Off";
            p.SetError(ara::core::ErrorCode(SMErrorDomainErrc::kRejected));
        } else {
            for (const auto& it: fgList) {
                ErrorCode ret = StateManager::getInstance().RequestMode(it, kOff);
                if (ret == kSuccess) {
                    p.set_value(output);
                } else {
                    p.SetError(ara::core::ErrorCode(SMErrorDomainErrc::kPrepareFailed));
                }
            }
        }
    }

    return p.get_future();
}

ara::core::Future<UpdateRequestSkeleton::VerifyUpdateOutput>
SMUpdateRequestSkeleton::VerifyUpdate(const FunctionGroupList& fgList)
{
    // Application Errors
    // kRejected : Requested operation was rejected due to State Managements/machines internal state.
    // kVerifyFailed : Verification step of update failed

    logger().LogDebug() << "Called VerifyUpdate";
    ara::core::Promise<VerifyUpdateOutput> p;
    VerifyUpdateOutput output{};

    if (!in_session_) {
        logger().LogWarn() << "Called VerifyUpdate, but it's not in session";
        p.SetError(ara::core::ErrorCode(SMErrorDomainErrc::kRejected));
    } else {
        auto it = std::find_if(fgList.begin(), fgList.end(), [](ara::sm::FGNameType v) {
            return v == kMachineFg;
        });
        if (it != fgList.end()) {
            // No plan to change MachinFG to Off state
            logger().LogWarn() << "No allow to change MachineFG to Off";
            p.SetError(ara::core::ErrorCode(SMErrorDomainErrc::kRejected));
        } else {
            for (const auto& it: fgList) {
                ErrorCode ret = StateManager::getInstance().RequestMode(it, kVerify);
                if (ret == kSuccess) {
                    p.set_value(output);
                } else {
                    p.SetError(ara::core::ErrorCode(SMErrorDomainErrc::kVerifyFailed));
                }
            }
        }
    }

    return p.get_future();
}

ara::core::Future<UpdateRequestSkeleton::ResetMachineOutput> SMUpdateRequestSkeleton::ResetMachine()
{
    logger().LogDebug() << "Called ResetMachine";
    ara::core::Promise<UpdateRequestSkeleton::ResetMachineOutput> p;

    logger().LogWarn() << "Called ResetMachine, It's not supported";
    // Won't support it for a while, might be for good
    p.SetError(ara::core::ErrorCode(SMErrorDomainErrc::kRejected));

    return p.get_future();
}

ara::core::Future<UpdateRequestSkeleton::StartUpdateSessionOutput> SMUpdateRequestSkeleton::StartUpdateSession()
{
    logger().LogDebug() << "Called StartUpdateSession";
    ara::core::Promise<StartUpdateSessionOutput> p;
    StartUpdateSessionOutput output{};

    if (!in_session_) {
        in_session_ = true;
        p.set_value(output);
    } else {
        logger().LogWarn() << "Called StartUpdateSession while it's in session";
        p.SetError(ara::core::ErrorCode(SMErrorDomainErrc::kRejected));
    }

    return p.get_future();
}

void SMUpdateRequestSkeleton::StopUpdateSession()
{
    logger().LogDebug() << "Called StopUpdateSession";
    if (!in_session_) {
        logger().LogWarn() << "Called StopUpdateSession while it's not in session";
    }
    in_session_ = false;
}

} // namespace sm_update_request