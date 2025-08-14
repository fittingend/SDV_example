#ifndef __IZONE1_MFSW_LISTENER_HPP__
#define __IZONE1_MFSW_LISTENER_HPP__

#include <SoaDataType.h>

class IZone1MfswListener {
public:
	virtual ~IZone1MfswListener() {};
	virtual void notifyMfsw(MfswWiperSw_e wiperSw, MfswWasherSw_e washerSw, MfswWiperIntStepSw_e intSw, MfswWiperMistSw_e mistSw) = 0;
};

#endif // __IZONE1_MFSW_LISTENER_HPP__
