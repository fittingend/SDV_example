#ifndef IZo__ZONE2_RESERVE_A_PROXY_IMPL_H__
#define IZo__ZONE2_RESERVE_A_PROXY_IMPL_H__

#include <list>
#include <memory>

#include <zone2/reserved/a/zone2rsva_proxy.h>
#include <IZone2ReservedAListener.hpp>

namespace zone2
{
namespace reserved
{
namespace a
{

class Zone2ReservedAProxyImpl {
public:
	static Zone2ReservedAProxyImpl *GetInstance();
	static void DeleteInstance();

	Zone2ReservedAProxyImpl();
	~Zone2ReservedAProxyImpl();

	void setEventListener(const std::shared_ptr<zone2::reserved::a::IZone2ReservedAListener> _listener);
	bool init();

	// field getter
	bool getterDeviceNormal(uint8_t &isNormal);

	bool isServiceFound(void);

private:
	static Zone2ReservedAProxyImpl *s_mInst;

	void FindServiceCallback(
		ara::com::ServiceHandleContainer<zone2::reserved::a::proxy::Zone2RsvAProxy::HandleType> container,
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
	//std::shared_ptr<zone2::reserved::a::IZone2ReservedAListener> mListener;
	std::list<std::shared_ptr<zone2::reserved::a::IZone2ReservedAListener>> mListenerList;
	std::shared_ptr<zone2::reserved::a::proxy::Zone2RsvAProxy> mProxy;
	std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

}
}
}

#endif // __IZONE2_RESERVE_A_PROXY_IMPL_H__