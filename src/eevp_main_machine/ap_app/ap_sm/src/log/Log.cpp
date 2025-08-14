/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#include <Log.hpp>

ara::log::Logger& logger() noexcept
{
    static ara::log::Logger& logger = ara::log::CreateLogger("SM", "Default");
    return logger;
}