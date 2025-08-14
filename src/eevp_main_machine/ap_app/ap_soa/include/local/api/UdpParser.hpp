#ifndef __UDP_PARSER_HPP__
#define __UDP_PARSER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <VehicleContext.hpp>

class UdpParser
{
private:
    static UdpParser *mInst;
    VehicleContext *mContext;
    UdpParser();
    ~UdpParser();

public:
    static UdpParser *GetInstance();
    int ParseData(unsigned char *data);
};

#endif