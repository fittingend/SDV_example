/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#include <thread>
#include <ara/core/vector.h>
#include <ara/core/string.h>

#include <Log.hpp>
#include <services/StateManager.hpp>

using namespace ara::core;
using namespace ara::exec;

namespace state_manager
{

constexpr std::int32_t kSetStateRetryCount = 20;
constexpr std::int32_t kSetStateWaitTime = 100;  // milliseconds

static void UndefinedStateCallback(FunctionGroup &fg)
{
    logger().LogError() << "Called UndefinedStateCallback : " << fg.GetName();
}

static void ProcessStateCallback(ProcessStateClient::ProcessInformation& processInformation)
{
    logger().LogError() << "Called ProcessStateCallback: "
                        << processInformation.name.data() << ", "
                        << processInformation.functionGroup.data();
}

StateManager::StateManager()
    : lock_{},
      stateClient_{std::make_unique<StateClient>(UndefinedStateCallback)},
      processStateClient_{std::make_unique<ProcessStateClient>(ProcessStateCallback)},
      mode_{Mode::kNone}
{
}

bool StateManager::Initialize()
{
    try {
        auto future = stateClient_->GetInitialMachineStateTransitionResult();
        auto fResult = future.GetResult();
        if (!fResult) {
            logger().LogError() << "Failed to initialize Startup state with " << fResult.Error();
            return false;
        }
    } catch (std::exception &e) {
        logger().LogError() << "Failed to initialize Startup state with " << e.what();
        return false;
    } catch (...) {
        logger().LogError() << "Failed to initialize Startup state with unknown reason";
        return false;
    }

    std::vector<String> initialFGs{kFGSC,
                                   kSFG01, kSFG02, kSFG03, kSFG04, kSFG05, kSFG06, kSFG07, kSFG08, kSFG09, kSFG10,
                                   kSFG11, kSFG12, kSFG13, kSFG14, kSFG15, kSFG16, kSFG17, kSFG18, kSFG19, kSFG20};
    for (const auto& it : initialFGs) {
        if (!ChangeState(it, kOn)) {
            return false;
        }
    }

    return true;
}

void StateManager::Release()
{
    // TBD : need to implement to change function-groups to off
}

bool StateManager::ChangeState(const ara::core::String &group, const ara::core::String &state)
{
    logger().LogInfo() << "Request to change to " << group << "," << state << ")";

    Result<FunctionGroup::CtorToken> fgResult = FunctionGroup::Preconstruct(group);
    FunctionGroup::CtorToken fgToken(fgResult.ValueOrThrow());
    FunctionGroup fg(std::move(fgToken));

    String stateStr = group + "/" + state;
    Result<FunctionGroupState::CtorToken> fgsResult = FunctionGroupState::Preconstruct(fg, stateStr);
    FunctionGroupState::CtorToken fgsToken(fgsResult.ValueOrThrow());
    FunctionGroupState fgs(std::move(fgsToken));

    try {
        auto retry_count = 0;
        for (; retry_count < kSetStateRetryCount; retry_count++) {
            auto future = stateClient_->SetState(fgs);
            // FIXME : wait_for is not working properly
            // future.wait_for(std::chrono::milliseconds(kSetStateWaitTime));
            std::this_thread::sleep_for(std::chrono::milliseconds(kSetStateWaitTime));
            auto fResult = future.GetResult();
            if (fResult.HasValue()) {
                logger().LogInfo() << "Changed FG(" << group << "," << state << ")";
                break;
            } else if (fResult.Error() == ExecErrc::kInTransitionToSameState) {
                logger().LogDebug() << "Retry to change FG(" << group << "," << state << ")";
            } else {
                logger().LogError() << "Skip to change FG(" << group << "," << state << ")";
                break;
            }
        }

        if (retry_count == kSetStateRetryCount) {
            logger().LogError() << "Failed to change FG(" << group << "," << state << ")" << " with retry count";
        }

        // let's do return true always even though Functiongroup change has failed
        // until eevp has a scenario for it.
        return true;
    } catch (std::exception &e) {
        logger().LogError() << "Failed to change FG(" << group << "," << state << ") with " << e.what();
        return false;
    } catch (...) {
        logger().LogError() << "Failed to change FG(" << group << "," << state << ") with unknow reason";
        return false;
    }

    return true;
}

ErrorCode StateManager::RequestMode(const ara::core::String &group, const ara::core::String &state)
{
    std::lock_guard<std::mutex> lock{lock_};

    if (!ChangeState(group, state)) {
        return kFailed;
    }

    return kSuccess;
}

ErrorCode StateManager::RequestModes(const ara::sm::FunctionGroupList &groups, const ara::core::String &state)
{
    std::lock_guard<std::mutex> lock{lock_};
    for (const auto& it : groups) {
        if (!ChangeState(it, state)) {
            return ErrorCode::kFailed;
        }
    }

    return kSuccess;
}

} // namespace state_manager