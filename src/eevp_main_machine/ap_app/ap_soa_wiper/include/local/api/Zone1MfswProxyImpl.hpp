#ifndef __ZONE1_MFSW_PROXY_IMPL_H__
#define __ZONE1_MFSW_PROXY_IMPL_H__

#include <list>
#include <mutex>
#include <SoaDataType.h>
#include <IZone1MfswListener.hpp>
#include <zone1/mfsw/input/zone1mfswinput_proxy.h>

namespace zone1
{
namespace mfsw
{
namespace input
{
class Zone1MfswProxyImpl {
public:
	static Zone1MfswProxyImpl *GetInstance();
	static void DeleteInstance();

	Zone1MfswProxyImpl();
	~Zone1MfswProxyImpl();

	void setEventListener(const std::shared_ptr<IZone1MfswListener> _listener);
	bool init();

	bool isServiceFound(void);

private:
	static Zone1MfswProxyImpl *s_mInst;

	void FindServiceCallback(
		ara::com::ServiceHandleContainer<zone1::mfsw::input::proxy::Zone1MfswInputProxy::HandleType> container,
		ara::com::FindServiceHandle findHandle);

	/// @brief Subscribe Event
	void SubscribeEvent();
	/// @brief Subscribe Field
	void SubscribeField();
	/// @brief Unsubscribe Event
	void UnsubscribeEvent();
	/// @brief Unsubscribe Field
	void UnsubscribeField();

	// callback func
	void cbZone1Mfsw();

	// Check Data Changed;
	bool checkToNeedLog(MfswWiperSw_e wiper_sw, MfswWasherSw_e washer_sw, MfswWiperMistSw_e mist_sw, MfswWiperIntStepSw_e int_sw);
	bool checkToNeedUpdate(MfswWiperSw_e wiper_sw, MfswWasherSw_e washer_sw, MfswWiperMistSw_e mist_sw, MfswWiperIntStepSw_e int_sw);

	bool mServiceFound;
	std::mutex mMutexForListener;
	std::list<std::shared_ptr<IZone1MfswListener>> mListenerList;
	std::shared_ptr<zone1::mfsw::input::proxy::Zone1MfswInputProxy> mProxy;
	std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;

	bool mIsLogged;
	uint64_t mLastLoggedTick;

	bool mIsNotified;
	uint64_t mLastNotifiedTick;
	MfswWiperSw_e mLastWiperSw;
	MfswWasherSw_e mLastWasherSw;
	MfswWiperMistSw_e mLastMistSw;
	MfswWiperIntStepSw_e mLastIntStep;
};

}
}
}

#endif // __ZONE1_MFSW_PROXY_IMPL_H__