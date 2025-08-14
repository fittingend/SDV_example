#ifndef IROA_LISTENER_H
#define IROA_LISTENER_H

namespace eevp {
namespace control {
namespace roa {

class IRoaListener {
public:
    virtual ~IRoaListener() {};
    virtual void notifySoaRoaDetectCount(std::uint8_t& value) = 0;
   

};

} /// namespace roa
} /// namespace control
} /// namespace eevp

#endif /* IROA_LISTENER_H */