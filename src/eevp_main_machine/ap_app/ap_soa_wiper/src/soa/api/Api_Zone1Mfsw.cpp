#include <Common.h>
#include <SoaDataType.h>
#include <Api_Zone1Mfsw.hpp>
#include <Zone1MfswProxyImpl.hpp>
#include <IZone1MfswListener.hpp>
#include <WiperManager.hpp>

#include <Log.hpp>
#include <Debug_DataType2String.hpp>

using namespace zone1::mfsw::input;

class VehicleInfoListener : public IZone1MfswListener {
private:
	bool mIsNotified;
	MfswWiperSw_e mLastWiperSw;
	MfswWasherSw_e mLastWasherSw;
	MfswWiperIntStepSw_e mLastIntSw;
	MfswWiperMistSw_e mLastMistSw;

public:
	VehicleInfoListener()
	{
		this->mIsNotified = false;
		this->mLastWiperSw = eMfswWiperSw_Unknown;
		this->mLastWasherSw = eMfswWasherSw_Unknown;
		this->mLastIntSw = eMfswWiperIntStepSw_Unknown;
		this->mLastMistSw = eMfswWiperMistSw_Unknown;
	}

	void notifyMfsw(MfswWiperSw_e wiperSw, MfswWasherSw_e washerSw, MfswWiperIntStepSw_e intSw, MfswWiperMistSw_e mistSw) override
	{
		bool notify_info = true;

		if (this->mIsNotified)
		{
			notify_info = ((this->mLastWiperSw != wiperSw) || (this->mLastWasherSw != washerSw) || (this->mLastIntSw != intSw) || (this->mLastMistSw != mistSw));
		}

		if (notify_info)
		{
		// LOG_INFO() << "wiperSw=" << getStringEnum_MfswWiperSw_e(wiperSw) << ", washerSw=" << getStringEnum_MfswWasherSw_e(washerSw) << ", intSw=" << getStringEnum_MfswWiperIntStepSw_e(intSw) << ", mistSw=" << getStringEnum_MfswWiperMistSw_e(mistSw) << "\n";
		}
	}
};

void Zone1_MFSW_Init(void)
{
	auto *inst = zone1::mfsw::input::Zone1MfswProxyImpl::GetInstance();
	inst->setEventListener(std::make_shared<VehicleInfoListener>());
	inst->init();
}

void Zone1_MFSW_Term(void)
{
	Zone1MfswProxyImpl::DeleteInstance();
}
