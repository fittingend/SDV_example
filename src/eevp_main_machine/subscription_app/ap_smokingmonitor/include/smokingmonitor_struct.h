#pragma once

struct __attribute__((packed)) Socket_Header
{
    uint16_t SEQ;
    uint8_t Retry_Cnt;
    uint16_t Length;
    uint16_t Res;
};

struct __attribute__((packed)) Socket_Data
{
    std::uint32_t VehicleUniqueSnr;
    std::uint32_t AppType; //moved
    std::uint8_t AppVer[6];
    std::uint8_t Date[8];          //moved
    std::uint8_t SmokingStatus;
};