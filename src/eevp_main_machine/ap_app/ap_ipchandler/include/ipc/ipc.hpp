/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#ifndef IPC_H
#define IPC_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 4)

struct __attribute__((aligned(8))) CpInfo
{
    std::uint8_t system_ctrl_req;
    char brand[1];                  // RESERVED
    char vehicle[3];                // RESERVED
    std::uint8_t region;            // RESERVED
    std::uint8_t ecotype;           // RESERVED
    std::uint8_t driverside;        // RESERVED
    std::uint8_t mild_48v;          // RESERVED
    std::uint8_t connectivity;      // RESERVED
    char part_no[10];               // RESERVED
    char supplier_code[4];          // RESERVED
    char manufact_date[4];          // RESERVED
    char serial_no[10];             // RESERVED
    std::uint8_t hw_version[3];
    std::uint8_t mcu_sw_version[5];
    char eth_db_version[6];         // RESERVED
    char can_db_version[6];         // RESERVED
    char oem_sw_version[3];         // RESERVED
    char oem_hw_version[3];         // RESERVED
    std::uint16_t vbat_level;       // RESERVED
    std::uint8_t acc;
    std::uint8_t ign1;
    std::uint8_t ign3;
    std::uint16_t acc_cnt;          // RESERVED
    std::uint16_t ign1_cnt;         // RESERVED
    std::uint16_t ign3_cnt;         // RESERVED
    std::uint64_t acc_on_time;      // RESERVED
    std::uint64_t ign1_on_time;     // RESERVED
    std::uint64_t ign3_on_time;     // RESERVED
    std::uint16_t boot_cycle_cnt;   // RESERVED
    std::uint8_t ert_act;           // RESERVED
    std::uint8_t dcu_wk;            // RESERVED
    std::uint8_t ccic_wk;           // RESERVED
    std::uint8_t bltn_cam_wk;       // RESERVED
    std::uint8_t bus_load_hsb1;     // RESERVED
    std::uint8_t bus_load_hsb2;     // RESERVED
    std::uint8_t bus_load_hsm;      // RESERVED
    std::uint8_t bus_load_fdp1;     // RESERVED
    std::uint8_t bus_load_fdp2;     // RESERVED
    std::uint8_t bus_load_fdc;      // RESERVED
    std::uint8_t bus_load_fde;      // RESERVED
    std::uint8_t bus_load_hsi;      // RESERVED
    std::uint8_t bus_load_hsd;      // RESERVED
    std::uint8_t bus_status_hsb1;   // RESERVED
    std::uint8_t bus_status_hsb2;   // RESERVED
    std::uint8_t bus_status_hsm;    // RESERVED
    std::uint8_t bus_status_fdp1;   // RESERVED
    std::uint8_t bus_status_fdp2;   // RESERVED
    std::uint8_t bus_status_fdc;    // RESERVED
    std::uint8_t bus_status_fde;    // RESERVED
    std::uint8_t bus_status_hsi;    // RESERVED
    std::uint8_t bus_status_hsd;    // RESERVED
    std::uint8_t current_time_mode;     // RESERVED
    std::uint64_t current_time_offset;  // RESERVED
    std::int16_t current_utc_offset;    // RESERVED
};

struct __attribute__((aligned(8))) ApInfo
{
    std::uint8_t system_ctrl_resp;
    std::uint8_t ap_sw_version[5];
    std::uint8_t hwire_req_dcu_rst;     // RESERVED
    std::uint8_t hwire_req_icu_wk;      // RESERVED
    std::uint8_t hwire_req_esw_rst;     // RESERVED
    std::uint8_t hwire_req_dcu_phy_rst; // RESERVED
    std::uint8_t hwire_req_hu_phy_rst;  // RESERVED
    std::uint8_t hwire_req_esw_wk;      // RESERVED
    std::uint8_t hwire_req_dcu_phy_wk;  // RESERVED
    std::uint8_t hwire_req_hu_phy_wk;   // RESERVED
};

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#ifdef ENABLE_IPC_SHM
#include <ipc-shm/ipc-a53i-cfg.h>
#include <ipc-shm/ipc-api.h>
#include <ipc-shm/ipc-typedef.h>
#else
typedef unsigned char ipc_return_t;
#define ipc_ret_ok 0
#endif

typedef void (*IPC_RxCallback) (void *buf, std::size_t size);
ipc_return_t IPCSendApInfo(const ApInfo *ap_info);
void RegisterIPCRxCallback(IPC_RxCallback callback);

#endif  // IPC_H
