#ifndef __ZONE3_RESERVE_A_PROXY_IMPL_H__
#define __ZONE3_RESERVE_A_PROXY_IMPL_H__

#include <list>
#include <memory>

#include <zone3/reserved/a/zone3rsva_proxy.h>
#include <IZone3ReservedAListener.hpp>

namespace zone3
{
namespace reserved
{
namespace a
{

class Zone3ReservedAProxyImpl {
public:
	static Zone3ReservedAProxyImpl *GetInstance();
	static void DeleteInstance();

	Zone3ReservedAProxyImpl();
	~Zone3ReservedAProxyImpl();

	void setEventListener(const std::shared_ptr<zone3::reserved::a::IZone3ReservedAListener> _listener);
	bool init();

	// field getter
	bool getterDeviceNormal(uint8_t &isNormal);

	bool isServiceFound(void);

private:
	static Zone3ReservedAProxyImpl *s_mInst;

	void FindServiceCallback(
		ara::com::ServiceHandleContainer<zone3::reserved::a::proxy::Zone3RsvAProxy::HandleType> container,
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
	//std::shared_ptr<zone3::reserved::a::IZone3ReservedAListener> mListener;
	std::list<std::shared_ptr<zone3::reserved::a::IZone3ReservedAListener>> mListenerList;
	std::shared_ptr<zone3::reserved::a::proxy::Zone3RsvAProxy> mProxy;
	std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

}
}
}

#endif // __IZONE3_RESERVE_A_PROXY_IMPL_H__