/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <mutex>
#include <ara/sm/impl_type_functiongrouplist.h>
#include <ara/exec/process_state_client.h>
#include <ara/exec/state_client.h>
#include <ara/exec/function_group.h>
#include <ara/core/error_code.h>
#include <ara/core/vector.h>
#include <ara/core/string.h>

namespace state_manager
{

// State
const ara::core::String kOff{"Off"};
const ara::core::String kOn{"On"};
const ara::core::String kVerify{"Verify"};

const ara::core::String kMachineFg{"MachineFG"};

const ara::core::String kMachineStartup{"Startup"};     // for future
const ara::core::String kMachineShutdown{"Shutdown"};   // for future
const ara::core::String kMachineRestart{"Restart"};     // for future

// Function groups
const ara::core::String kFGSC{"SC"};        // Standard Controller

// Subscription Function groups (beyless)
const ara::core::String kSFG01{"SFG01"};
const ara::core::String kSFG02{"SFG02"};
const ara::core::String kSFG03{"SFG03"};
const ara::core::String kSFG04{"SFG04"};
const ara::core::String kSFG05{"SFG05"};
const ara::core::String kSFG06{"SFG06"};
const ara::core::String kSFG07{"SFG07"};
const ara::core::String kSFG08{"SFG08"};
const ara::core::String kSFG09{"SFG09"};
const ara::core::String kSFG10{"SFG10"};
const ara::core::String kSFG11{"SFG11"};
const ara::core::String kSFG12{"SFG12"};
const ara::core::String kSFG13{"SFG13"};
const ara::core::String kSFG14{"SFG14"};
const ara::core::String kSFG15{"SFG15"};
const ara::core::String kSFG16{"SFG16"};
const ara::core::String kSFG17{"SFG17"};
const ara::core::String kSFG18{"SFG18"};
const ara::core::String kSFG19{"SFG19"};
const ara::core::String kSFG20{"SFG20"};

enum Mode : std::uint8_t    // for future
{
    kNone,
    kInUpdating
};

enum ErrorCode : std::uint8_t
{
    kSuccess = 0,
    kFailed = 1,
    kRejected = 2,  // for future
};

class StateManager final
{
    public:
        static StateManager& getInstance() {
            static StateManager instance{};
            return instance;
        }

        bool Initialize();
        void Release();

        ErrorCode RequestMode(const ara::core::String &group, const ara::core::String &state);
        ErrorCode RequestModes(const ara::sm::FunctionGroupList &groups, const ara::core::String &state);

    private:
        std::mutex lock_;
        std::unique_ptr<ara::exec::StateClient> stateClient_;
        std::unique_ptr<ara::exec::ProcessStateClient> processStateClient_;
        Mode mode_; // for future

        StateManager();
        StateManager(const StateManager&) = delete;
        StateManager& operator=(const StateManager&) = delete;

        bool ChangeState(const ara::core::String &group, const ara::core::String &state);
};

} // namespace state_manager

#endif // STATE_MANAGER_HPP