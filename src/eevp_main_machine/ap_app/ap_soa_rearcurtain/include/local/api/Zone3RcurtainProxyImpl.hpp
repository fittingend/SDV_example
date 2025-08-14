#ifndef __ZONE3_RCURTAIN_PROXY_IMPL_H__
#define __ZONE3_RCURTAIN_PROXY_IMPL_H__

#include <thread>

#include <list>
#include "zone3/rcurtain/control/zone3rcurtain_proxy.h"
#include "IZone3RearCurtainListener.hpp"

namespace zone3 {
namespace rcurtain {
namespace control {

class Zone3RearCurtainProxyImpl {
public:
	static Zone3RearCurtainProxyImpl *GetInstance();
	static void DeleteInstance();

	Zone3RearCurtainProxyImpl();
	~Zone3RearCurtainProxyImpl();

	void setEventListener(const std::shared_ptr<zone3::rcurtain::control::IZone3RearCurtainListener> _listener);
	bool init();

	// method
	void controlMotor(const uint8_t & motorDir);

	// field getter
	bool getterRcurtainStatus(zone3::rcurtain::control::EcuRcurtainStatus& status);

	bool isServiceFound(void);

private:
	static Zone3RearCurtainProxyImpl *s_mInst;

	void FindServiceCallback(
		ara::com::ServiceHandleContainer<zone3::rcurtain::control::proxy::Zone3RcurtainProxy::HandleType> container,
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
	void cbZone3RctnStatus();

	bool mServiceFound;
	std::list<std::shared_ptr<zone3::rcurtain::control::IZone3RearCurtainListener>> mListenerList;
	std::shared_ptr<zone3::rcurtain::control::proxy::Zone3RcurtainProxy> mProxy;
	std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

}
}
}

#endif