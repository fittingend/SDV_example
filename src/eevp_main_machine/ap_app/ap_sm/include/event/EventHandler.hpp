/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <mutex>
#include <memory>
#include <event2/event.h>
#include <ara/core/vector.h>
#include <ara/core/string.h>

#include <event/Notifier.hpp>
#include <services/RoutingIPCProxy.hpp>
#include <services/SMUpdateRequestSkeleton.hpp>

namespace aa_event
{

class AAEventHandler;

struct TimeoutEventInfo
{
    aa_event::AAEventHandler *evnetHandler;
    aa_event::TimeoutEvent eventKind;
};

class AAEventHandler final
{
    public:
        AAEventHandler();
        ~AAEventHandler() = default;

        void Run();
        static void OnEvent(evutil_socket_t fd, short event, void* arg);
        static void OnTimeoutEvent(evutil_socket_t fd, short event, void* arg);

    private:
        std::unique_ptr<routingipc::IPCRoutingIPCProxy> ipcProxy_;
        std::unique_ptr<sm_update_request::SMUpdateRequestSkeleton> updateRequestSkeleton_;

        bool InitializeServices(aa_event::EventNotifer &eventReceived);
        void ReleaseServices();
};

} // aa_event

#endif // EVENT_HANDLER_HPP