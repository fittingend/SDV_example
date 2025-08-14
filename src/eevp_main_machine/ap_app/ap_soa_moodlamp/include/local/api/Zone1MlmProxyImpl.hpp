#ifndef __ZONE1_MLM_PROXY_IMPL_H__
#define __ZONE1_MLM_PROXY_IMPL_H__

#include <memory>
#include <list>

#include <zone1/mlm/control/impl_type_ecumlmstate.h>
#include <zone1/mlm/control/zone1mlm_proxy.h>
#include <IZone1MlmListener.hpp>

namespace zone1
{
namespace mlm
{
namespace control
{

class Zone1MlmProxyImpl {
public:
	static Zone1MlmProxyImpl *GetInstance();
	static void DeleteInstance();

	Zone1MlmProxyImpl();
	~Zone1MlmProxyImpl();

	void setEventListener(const std::shared_ptr<zone1::mlm::control::IZone1MlmListener> _listener);
	bool init();

	// method
	void setMlmBrightness(const std::uint8_t& brightness);
	void setMlmColor(const std::uint8_t& colorIndex);

	// field getter
	bool getterMlmStatus(zone1::mlm::control::EcuMlmState& state);

	bool isServiceFound(void);

private:
	static Zone1MlmProxyImpl *s_mInst;

	void FindServiceCallback(
		ara::com::ServiceHandleContainer<zone1::mlm::control::proxy::Zone1MlmProxy::HandleType> container,
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
	void cbZone1MlmStatus();

	bool mServiceFound;
	std::list<std::shared_ptr<zone1::mlm::control::IZone1MlmListener>> mListenerList;
	std::shared_ptr<zone1::mlm::control::proxy::Zone1MlmProxy> mProxy;
	std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

}
}
}

#endif // __ZONE1_MLM_PROXY_IMPL_H__