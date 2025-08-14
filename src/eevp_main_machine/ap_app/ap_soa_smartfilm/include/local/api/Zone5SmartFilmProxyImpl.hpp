#ifndef __ZONE5_SMARTFILM_PROXY_IMPL_HPP__
#define __ZONE5_SMARTFILM_PROXY_IMPL_HPP__

#include <list>
#include <memory>

#include <zone5/smtfilm/control/zone5smtfilm_proxy.h>
#include <IZone5SmartFilmListener.hpp>

namespace zone5
{
namespace smtfilm
{
namespace control
{
class Zone5SmartFilmProxyImpl {
public:
	static Zone5SmartFilmProxyImpl *GetInstance();
	static void DeleteInstance();

	Zone5SmartFilmProxyImpl();
	~Zone5SmartFilmProxyImpl();

	void setEventListener(const std::shared_ptr<zone5::smtfilm::control::IZone5SmartFilmListener> _listener);
	bool init();

	// method
	void SetOpacity(SmtfilmPos_e pos, int opacity);
	void SetAllOpacity(int opacity);

	// field getter
	bool getterDeviceNormal(DeviceNormal_e &isNormal);
	bool getterSmtfilmOpacities(SmtfilmOpacities_t &opacities);

	bool isServiceFound(void);

private:
	static Zone5SmartFilmProxyImpl *s_mInst;

	void FindServiceCallback(
		ara::com::ServiceHandleContainer<zone5::smtfilm::control::proxy::Zone5SmtFilmProxy::HandleType> container,
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
	void cbZone5SmartFilmDeviceNormal();
	void cbZone5SmartFilmOpacities();

	bool mServiceFound;
	std::list<std::shared_ptr<zone5::smtfilm::control::IZone5SmartFilmListener>> mListenerList;
	std::shared_ptr<zone5::smtfilm::control::proxy::Zone5SmtFilmProxy> mProxy;
	std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

}
}
}



#endif // __ZONE5_SMARTFILM_PROXY_IMPL_HPP__