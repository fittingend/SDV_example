#ifndef __ZONE4_WIPER_PROXY_IMPL_H__
#define __ZONE4_WIPER_PROXY_IMPL_H__

#include <list>
#include <memory>

#include <zone4/wiper/control/zone4wiper_proxy.h>
#include <IZone4WiperListener.hpp>

namespace zone4
{
namespace wiper
{
namespace control
{

class Zone4WiperProxyImpl {
public:
	static Zone4WiperProxyImpl *GetInstance();
	static void DeleteInstance();

	Zone4WiperProxyImpl();
	~Zone4WiperProxyImpl();

	void setEventListener(const std::shared_ptr<zone4::wiper::control::IZone4WiperListener> _listener);
	bool init();

	// method
	void controlWiper(const std::uint8_t& mode);

	// field getter
	bool getterWiperOpStatus(zone4::wiper::control::EcuWiperOperationStatus& opStatus);

	bool isServiceFound(void);

private:
	static Zone4WiperProxyImpl *s_mInst;

	void FindServiceCallback(
		ara::com::ServiceHandleContainer<zone4::wiper::control::proxy::Zone4WiperProxy::HandleType> container,
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
	void cbZone4WiperOpStatus();

	bool mServiceFound;
	//std::shared_ptr<zone4::wiper::control::IZone4WiperListener> mListener;
	std::list<std::shared_ptr<zone4::wiper::control::IZone4WiperListener>> mListenerList;
	std::shared_ptr<zone4::wiper::control::proxy::Zone4WiperProxy> mProxy;
	std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

}
}
}

#endif // __ZONE4_WIPER_PROXY_IMPL_H__