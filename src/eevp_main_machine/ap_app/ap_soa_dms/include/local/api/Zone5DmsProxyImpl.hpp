#ifndef __ZONE5_DMS_PROXY_IMPL_HPP__
#define __ZONE5_DMS_PROXY_IMPL_HPP__

#include <list>
#include <memory>

#include <zone5/dms/control/zone5dms_proxy.h>
#include <IZone5DmsListener.hpp>
#include <SoaDataType.h>

namespace zone5
{
namespace dms
{
namespace control
{

class Zone5DmsProxyImpl {
public:
	static Zone5DmsProxyImpl *GetInstance();
	static void DeleteInstance();

	Zone5DmsProxyImpl();
	~Zone5DmsProxyImpl();

	void setEventListener(const std::shared_ptr<zone5::dms::control::IZone5DmsListener> _listener);
	bool init();

	// method
	void SetPower(bool on);

	// field getter
	bool getterDeviceNormal(DeviceNormal_e &isNormal);
	bool getterDmsStatus(bool &powerOn, DmsDriverInside_e &inside);
	bool getterDmsDriverStatus(DmsFatigue_e &fatigue, DmsGazingDir_e &dir, DmsOnPhone_e &phone, DmsSmoking_e &smoking);

	bool isServiceFound(void);

private:
	static Zone5DmsProxyImpl *s_mInst;

	void FindServiceCallback(
		ara::com::ServiceHandleContainer<zone5::dms::control::proxy::Zone5DmsProxy::HandleType> container,
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
	void cbZone5DmsDeviceNormal();
	void cbZone5DmsDriverStatus();
	void cbZone5DmsStatus();

	bool mServiceFound;
	std::list<std::shared_ptr<zone5::dms::control::IZone5DmsListener>> mListenerList;
	std::shared_ptr<zone5::dms::control::proxy::Zone5DmsProxy> mProxy;
	std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

}
}
}



#endif // __ZONE5_DMS_PROXY_IMPL_HPP__