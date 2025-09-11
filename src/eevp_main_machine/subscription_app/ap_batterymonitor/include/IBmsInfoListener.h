#ifndef IBMS_INFO_LISTENER_H
#define IBMS_INFO_LISTENER_H
#include "eevp/bmsinfo/impl_type_struct_bmsinfo.h"

namespace eevp {
namespace bmsinfo {
namespace service {

class IBmsInfoListener {
public:
    virtual ~IBmsInfoListener() {};
    virtual void ems_BmsInfo(const eevp::bmsinfo::Struct_BmsInfo& bmsInfo) = 0;
   

};

} /// namespace service
} /// namespace bmsinfo
} /// namespace eevp

#endif /* IBMS_INFO_LISTENER_H */
