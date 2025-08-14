#ifndef __ZONE2_ROA_PROXY_IMPL_HPP__
#define __ZONE2_ROA_PROXY_IMPL_HPP__

#include <list>
#include <memory>

#include <zone2/roa/control/zone2roa_proxy.h>
#include <IZone2RoaListener.hpp>

namespace zone2
{
namespace roa
{
namespace control
{

class Zone2RoaProxyImpl {
public:
	static Zone2RoaProxyImpl *GetInstance();
	static void DeleteInstance();

	Zone2RoaProxyImpl();
	~Zone2RoaProxyImpl();

	void setEventListener(const std::shared_ptr<zone2::roa::control::IZone2RoaListener> _listener);
	bool init();

	// method
	void controlRoa(const std::uint8_t& snsrRunReq);

	// field getter
	bool getterSnsrStatus(zone2::roa::control::EcmRoaSnsrStatus& status);

	bool isServiceFound(void);

private:
	static Zone2RoaProxyImpl *s_mInst;

	void FindServiceCallback(
		ara::com::ServiceHandleContainer<zone2::roa::control::proxy::Zone2RoaProxy::HandleType> container,
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
	void cbZone2SnsrStatus();

	bool mServiceFound;
	//std::shared_ptr<zone2::roa::control::IZone2RoaListener> mListener;
	std::list<std::shared_ptr<zone2::roa::control::IZone2RoaListener>> mListenerList;
	std::shared_ptr<zone2::roa::control::proxy::Zone2RoaProxy> mProxy;
	std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

}
}
}

#endif // __ZONE2_ROA_PROXY_IMPL_HPP__