/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#ifndef SM_UPDATE_REQUEST_SKELETON_HPP
#define SM_UPDATE_REQUEST_SKELETON_HPP

#include <mutex>
#include <memory>
#include <condition_variable>

#include <event/Notifier.hpp>
#include <ara/sm/updaterequest_skeleton.h>

namespace sm_update_request
{

class SMUpdateRequestSkeleton : public ara::sm::skeleton::UpdateRequestSkeleton, public aa_event::EventRegister
{
    public:
        SMUpdateRequestSkeleton();
        ~SMUpdateRequestSkeleton();

    ara::core::Future<ara::sm::skeleton::UpdateRequestSkeleton::PrepareRollbackOutput> PrepareRollback(const ara::sm::FunctionGroupList& FunctionGroupList) override;
    ara::core::Future<ara::sm::skeleton::UpdateRequestSkeleton::PrepareUpdateOutput> PrepareUpdate(const ara::sm::FunctionGroupList& FunctionGroupList) override;
    ara::core::Future<ara::sm::skeleton::UpdateRequestSkeleton::ResetMachineOutput> ResetMachine() override;
    ara::core::Future<ara::sm::skeleton::UpdateRequestSkeleton::StartUpdateSessionOutput> StartUpdateSession() override;
    void StopUpdateSession() override;
    ara::core::Future<ara::sm::skeleton::UpdateRequestSkeleton::VerifyUpdateOutput> VerifyUpdate(const ara::sm::FunctionGroupList& FunctionGroupList) override;

    bool Initialize();
    void Release();

    protected:
        std::mutex lock_;
        bool in_session_;

};

} // namespace sm_update_request

#endif // SM_UPDATE_REQUEST_SKELETON_HPP