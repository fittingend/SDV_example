#ifndef __ZONE1_DRVSEAT_PROXY_IMPL_H__
#define __ZONE1_DRVSEAT_PROXY_IMPL_H__

#include <list>
#include <memory>
#include <Common.h>
#include <SoaDataType.h>
#include <zone1/drvseat/control/zone1drvseat_proxy.h>
#include <IZone1DrvSeatListener.hpp>

namespace zone1
{
namespace drvseat
{
namespace control
{

class Zone1DriverSeatProxyImpl {
public:
	static Zone1DriverSeatProxyImpl *GetInstance();
	static void DeleteInstance();

	Zone1DriverSeatProxyImpl();
	~Zone1DriverSeatProxyImpl();

	void setEventListener(const std::shared_ptr<zone1::drvseat::control::IZone1DrvseatListener> _listener);
	bool init();

	// method
	void controlMotor(const zone1::drvseat::control::EcuDrvSeatMotorInst& inst);

	// field getter
	bool getterMotorDir(zone1::drvseat::control::EcuDrvSeatMotorDirection& dir);
	bool getterMotorPos(zone1::drvseat::control::EcuDrvseatMotorPosition& pos);

	bool isServiceFound(void);

private:
	static Zone1DriverSeatProxyImpl *s_mInst;

	void FindServiceCallback(
		ara::com::ServiceHandleContainer<zone1::drvseat::control::proxy::Zone1DrvSeatProxy::HandleType> container,
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
	void cbZone1MotorDir();
	void cbZone1MotorPos();

	bool mServiceFound;
	//std::shared_ptr<zone1::drvseat::control::IZone1DrvseatListener> mListener;
	std::list<std::shared_ptr<zone1::drvseat::control::IZone1DrvseatListener>> mListenerList;
	std::shared_ptr<zone1::drvseat::control::proxy::Zone1DrvSeatProxy> mProxy;
	std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

}
}
}

#endif // __ZONE1_DRVSEAT_PROXY_IMPL_H__