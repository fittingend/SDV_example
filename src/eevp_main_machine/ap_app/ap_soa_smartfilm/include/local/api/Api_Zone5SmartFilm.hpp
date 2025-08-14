#ifndef __API_ZONE5_SMARTFILM_HPP__
#define __API_ZONE5_SMARTFILM_HPP__

#include <Common.h>
#include <SoaDataType.h>

extern void Zone5_SMARTFILM_Init(void);
extern void Zone5_SMARTFILM_Term(void);
extern void Zone5_SMARTFILM_SetOpacity(SmtfilmPos_e pos, int opacity);
extern void Zone5_SMARTFILM_SetAllOpacity(int opacity);
extern void Zone5_SMARTFILM_UpdateContext(void);


#endif // __API_ZONE5_SMARTFILM_HPP__
