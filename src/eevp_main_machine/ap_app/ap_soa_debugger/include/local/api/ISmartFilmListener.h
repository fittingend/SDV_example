#ifndef __ISMARTFILM_LISTENER_H__
#define __ISMARTFILM_LISTENER_H__

#include <Common.h>
#include <SoaDataType.h>

namespace eevp {
namespace control {
namespace smartfilm {

class ISmartFilmListener {
public:
    virtual ~ISmartFilmListener() {};

    virtual void notifyIsDeviceNormal(const DeviceNormal_e &fieldValue) = 0;
    virtual void notifySmartFilmAutoMode(const bool &autoMode) = 0;
    virtual void notifySmartFilmOpacities(const SmtfilmOpacities_t &opacities) = 0;
};

} /// namespace smartfilm
} /// namespace control
} /// namespace eevp

#endif /* __ISMARTFILM_LISTENER_H__ */