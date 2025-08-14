#ifndef __IHVAC_LISTENER_H__
#define __IHVAC_LISTENER_H__

#include <Common.h>
#include <SoaDataType.h>

namespace eevp {
namespace control {
namespace hvac {

class IHvacListener {
public:
    virtual ~IHvacListener() {};

    virtual void notifyIsDeviceNormal(const DeviceNormal_e &fieldValue) = 0;
    virtual void notifyHvacSetting(HvacStatus_t &setting) = 0;
    virtual void notifyHvacStatus(HvacStatus_t &status) = 0;
    virtual void notifyHvacTemps(HvacTemps_t &temps) = 0;
};

} /// namespace hvac
} /// namespace control
} /// namespace eevp

#endif /* __IHVAC_LISTENER_H__ */