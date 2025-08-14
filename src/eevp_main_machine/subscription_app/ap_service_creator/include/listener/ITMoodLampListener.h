#ifndef __ITMOODLAMPLISTENER_H__
#define __ITMOODLAMPLISTENER_H__

#include "lmp/mode/impl_type_soamimmoodemode.h"

namespace eevp
{
    namespace simulation
    {
        class ITMoodLampListener
        {
        public:
            virtual ~ITMoodLampListener() {};

            virtual void RequestMImSetBrightness(const std::uint16_t &Brightness) = 0;
            virtual void RequestMImSetMode(const lmp::mode::SoaMImMoodeMode &mood) = 0;
            virtual void RequestMImSetRGBColor(const std::uint8_t &ColorIndex) = 0;
        };
    } /// namespace control
} /// namespace eevp

#endif // __ITMOODLAMPLISTENER_H__