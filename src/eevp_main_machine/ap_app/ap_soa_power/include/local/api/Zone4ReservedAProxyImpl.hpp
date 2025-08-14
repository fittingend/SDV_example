#ifndef __ZONE4_RESERVE_A_PROXY_IMPL_H__
#define __ZONE4_RESERVE_A_PROXY_IMPL_H__

#include <list>
#include <memory>

#include <zone4/reserved/a/zone4rsva_proxy.h>
#include <IZone4ReservedAListener.hpp>

namespace zone4
{
namespace reserved
{
namespace a
{

class Zone4ReservedAProxyImpl {
public:
	static Zone4ReservedAProxyImpl *GetInstance();
	static void DeleteInstance();

	Zone4ReservedAProxyImpl();
	~Zone4ReservedAProxyImpl();

	void setEventListener(const std::shared_ptr<zone4::reserved::a::IZone4ReservedAListener> _listener);
	bool init();

	// field getter
	bool getterDeviceNormal(uint8_t &isNormal);

	bool isServiceFound(void);

private:
	static Zone4ReservedAProxyImpl *s_mInst;

	void FindServiceCallback(
		ara::com::ServiceHandleContainer<zone4::reserved::a::proxy::Zone4RsvAProxy::HandleType> container,
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
	//std::shared_ptr<zone4::reserved::a::IZone4ReservedAListener> mListener;
	std::list<std::shared_ptr<zone4::reserved::a::IZone4ReservedAListener>> mListenerList;
	std::shared_ptr<zone4::reserved::a::proxy::Zone4RsvAProxy> mProxy;
	std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

}
}
}

#endif // __IZONE4_RESERVE_A_PROXY_IMPL_H__