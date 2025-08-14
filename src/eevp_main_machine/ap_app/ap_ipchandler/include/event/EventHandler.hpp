/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <thread>
#include <vector>
#include <mutex>
#include <event2/event.h>
#include <services/RoutingIPCSkeleton.hpp>

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
        std::shared_ptr<routingipc::IPCRoutingIPCSkeleton> routingIPCSkeleton_;

#ifdef ENABLE_TEST
    public:
        std::shared_ptr<routingipc::IPCRoutingIPCSkeleton> GetIPCSkeleton() { return routingIPCSkeleton_; }
#endif
};

} // aa_event

#endif // EVENT_HANDLER_HPP