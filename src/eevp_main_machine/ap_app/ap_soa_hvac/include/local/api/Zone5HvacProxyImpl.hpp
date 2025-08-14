#ifndef __ZONE5_HVAC_PROXY_IMPL_HPP__
#define __ZONE5_HVAC_PROXY_IMPL_HPP__

#include <list>
#include <memory>

#include <SoaDataType.h>
#include <zone5/hvac/control/zone5hvac_proxy.h>
#include <IZone5HvacListener.hpp>

namespace zone5
{
namespace hvac
{
namespace control
{

class Zone5HvacProxyImpl {
public:
	static Zone5HvacProxyImpl *GetInstance();
	static void DeleteInstance();

	Zone5HvacProxyImpl();
	~Zone5HvacProxyImpl();

	void setEventListener(const std::shared_ptr<zone5::hvac::control::IZone5HvacListener> _listener);
	bool init();

	// method
	void SetHvacPower(bool on);
	void SetAcOnOff(bool on);
	void SetAutoTemp(bool on);
	void SetSync(bool on);
	void SetDriverTemp(int temperature);
	void SetPassengerTemp(int temperature);
	void SetBlowingForce(int force);
	void SetBlowingDirection(HvacBlowDir_e dir);
	void SetAirSource(HvacAirSource_e src);
	void SetForwardDefrostOn(bool on);
	void SetBackwardDefrostOn(bool on);

	// field getter
	bool getterDeviceNormal(DeviceNormal_e &isNormal);
	bool getterHvacSetting(HvacStatus_t &setting);
	bool getterHvacStatus(HvacStatus_t &status);
	bool getterHvacTemps(HvacTemps_t &temps);

	bool isServiceFound(void);

private:
	static Zone5HvacProxyImpl *s_mInst;

	void FindServiceCallback(
		ara::com::ServiceHandleContainer<zone5::hvac::control::proxy::Zone5HvacProxy::HandleType> container,
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
	void cbZone5HvacDeviceNormal();
	void cbZone5HvacSetting();
	void cbZone5HvacStatus();
	void cbZone5HvacTemps();

	bool mServiceFound;
	std::list<std::shared_ptr<zone5::hvac::control::IZone5HvacListener>> mListenerList;
	std::shared_ptr<zone5::hvac::control::proxy::Zone5HvacProxy> mProxy;
	std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

}
}
}



#endif // __ZONE5_HVAC_PROXY_IMPL_HPP__