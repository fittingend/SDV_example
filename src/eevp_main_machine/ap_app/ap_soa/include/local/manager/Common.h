#ifndef __COMMON_H__
#define __COMMON_H__

// Add all the functional definition in here

#define DEBUG_TELNET
//#define DISABLE_SOMEIP
#define ZONE_COMM_UDP
//#define ZONE_COMM_UDP_MULTICAST
//#define CHANGE_ZONEIP_BY_UDP_PACKETS
#define UDP_SEND_REPEAT
#define BLOCK_MOODLAMP_STATUS_FROM_ZONE1

#define USE_LOG
#define USE_LOG_CONSOLE
#define USL_LOG_ARA_LOG
#define USL_LOG_TELNET

#if defined(ZONE_COMM_UDP_MULTICAST)
    #define ZONE_UDP_MULTICAST_IP "239.255.255.250"
    #define ZONE_UDO_MULTICAST_PORT 8001
#else
    #define UDP_PORT 8001
    #define ZONE1_IP "10.100.0.50"
    #define ZONE1_PORT 8001
    #define ZONE2_IP "10.100.0.60"
    #define ZONE2_PORT 8001
    #define ZONE3_IP "10.100.0.70"
    #define ZONE3_PORT 8001
    #define ZONE4_IP "10.100.0.80"
    #define ZONE4_PORT 8001
    #define ZONE5_IP "10.100.0.90"
    #define ZONE5_PORT 8001
#endif

#define DATAFILE_DIR_PATH "./"
#define VEHICLE_CONTEXT_USE_ATOMIC
#define GENERATE_ENUM_STRINGS

#endif
