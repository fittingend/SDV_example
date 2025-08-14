#ifndef IDMS_LISTENER_H
#define IDMS_LISTENER_H

#include <SoaDataType.h>

namespace eevp {
namespace control {
namespace dms {

class IDmsListener {
public:
    virtual ~IDmsListener() {};

    virtual void notifyIsDeviceNormal(const DeviceNormal_e &fieldValue) = 0;
    virtual void notifyDmsStatus(const bool &powerOn, const DmsDriverInside_e &inside) = 0;
    virtual void notifyDriverStatus(const DmsFatigue_e &fatigue, const DmsGazingDir_e &dir, const DmsOnPhone_e &phone, const DmsSmoking_e &smoking) = 0;
    virtual void getIsDeviceNormal() = 0;
};

} /// namespace driverseat
} /// namespace control
} /// namespace eevp

#endif /* IDMS_LISTENER_H */