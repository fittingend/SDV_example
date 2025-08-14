#ifndef __DEBUG_DATATYPE_TO_STRING_H__
#define __DEBUG_DATATYPE_TO_STRING_H__

#include <string>
#include <Common.h>
#include <SoaDataType.h>

extern std::string getStringEnum_bool(bool val);
extern std::string getStringEnum_DeviceNormal_e(DeviceNormal_e val);
extern std::string getStringEnum_ClientLevel_e(ClientLevel_e val);
extern std::string getStringEnum_PowerState_e(PowerState_e val);

// DMS
extern std::string getStringEnum_DmsDriverInside_e(DmsDriverInside_e val);
extern std::string getStringEnum_DmsFatigue_e(DmsFatigue_e val);
extern std::string getStringEnum_DmsGazingDir_e(DmsGazingDir_e val);
extern std::string getStringEnum_DmsOnPhone_e(DmsOnPhone_e val);
extern std::string getStringEnum_DmsSmoking_e(DmsSmoking_e val);

// HVAC
extern std::string getStringEnum_HvacAirSource_e(HvacAirSource_e val);
extern std::string getStringEnum_HvacBlowDir_e(HvacBlowDir_e val);
extern std::string getString_HvacStatus_t(const HvacStatus_t &val);
extern std::string getString_HvacTemps_t(const HvacTemps_t &val);

// SmartFilm
extern std::string getStringEnum_SmtfilmPos_e(const SmtfilmPos_e &val);
extern std::string getString_SmtfilmOpacities_t(const SmtfilmOpacities_t &val);
#endif