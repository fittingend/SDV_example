/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

#include <deque>

namespace aa_event
{

enum Event : std::uint8_t
{
    kNone = 0,
    kIPCRx = 1,     // Receive CP Info
};

enum TimeoutEvent : std::uint8_t
{
    kPeriodicTimer1Secs = 0,  // For future
};

class EventNotifier
{
    public:
        EventNotifier() = default;
        ~EventNotifier() = default;

        virtual void EventArrived(aa_event::Event eventKind) = 0;
};

class EventRegister
{
    public:
        EventRegister() :
            eventNotifier_{nullptr} {};
        ~EventRegister() = default;

        void RegisterEventHandler(EventNotifier &ev) {
            eventNotifier_ = &ev;
        }

    protected:
        EventNotifier* eventNotifier_;

        void SendEvent(aa_event::Event eventKind) {
            if (eventNotifier_ != nullptr) {
                eventNotifier_->EventArrived(eventKind);
            }
        }
};

} // aa_event

#endif // NOTIFIER_HPP