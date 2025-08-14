#ifndef __ZONE1_RESERVE_A_PROXY_IMPL_H__
#define __ZONE1_RESERVE_A_PROXY_IMPL_H__

#include <list>
#include <memory>

#include <zone1/reserved/a/zone1rsva_proxy.h>
#include <IZone1ReservedAListener.hpp>

namespace zone1
{
namespace reserved
{
namespace a
{

class Zone1ReservedAProxyImpl {
public:
	static Zone1ReservedAProxyImpl *GetInstance();
	static void DeleteInstance();

	Zone1ReservedAProxyImpl();
	~Zone1ReservedAProxyImpl();

	void setEventListener(const std::shared_ptr<zone1::reserved::a::IZone1ReservedAListener> _listener);
	bool init();

	// field getter
	bool getterDeviceNormal(uint8_t &isNormal);

	bool isServiceFound(void);

private:
	static Zone1ReservedAProxyImpl *s_mInst;

	void FindServiceCallback(
		ara::com::ServiceHandleContainer<zone1::reserved::a::proxy::Zone1RsvAProxy::HandleType> container,
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
	void cbDeviceNormal();

	bool mServiceFound;
	//std::shared_ptr<zone1::reserved::a::IZone1DrvseatListener> mListener;
	std::list<std::shared_ptr<zone1::reserved::a::IZone1ReservedAListener>> mListenerList;
	std::shared_ptr<zone1::reserved::a::proxy::Zone1RsvAProxy> mProxy;
	std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

}
}
}

#endif // __ZONE1_RESERVE_A_PROXY_IMPL_H__