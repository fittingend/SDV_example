#include <Common.h>
#include <SoaDataType.h>
#include <Api_Zone1VehicleInfo.hpp>
#include <Zone1VehicleInfoProxyImpl.hpp>
#include <IZone1VehicleInfoListener.hpp>
#include <SimpleTimer.hpp>
#include <WiperManager.hpp>
#include <MulticastServerManager.hpp>

#include <Log.hpp>
#include <Debug_DataType2String.hpp>

using namespace zone::input::service;

static int s_multicastServerId = -1;

class VehicleInfoListener : public IZone1VehicleInfoListener {
private:
	bool mIsNotified;
	int mLastVehicleSpeed;
	GearState_e mLastGear;
	AccState_e mLastAcc;
	IgnState_e mLastIgn1;
	IgnState_e mLastIgn2;
	uint64_t mLastNotifiedTick;

public:
	VehicleInfoListener()
	{
		this->mIsNotified = false;
		this->mLastVehicleSpeed = 0;
		this->mLastGear = eGearState_Unknown;
		this->mLastAcc = eAccState_Unknown;
		this->mLastIgn1 = eIgnState_Unknown;
		this->mLastIgn2 = eIgnState_Unknown;
		this->mLastNotifiedTick = 0;
	}

	void notifyVehicleInfo(int vehicleSpeed, GearState_e gear, AccState_e acc, IgnState_e ign1, IgnState_e ign2) override
	{
		// LOG_INFO() << "vehicleSpeed=" << vehicleSpeed << ", gear=" << getStringEnum_GearState_e(gear) << ", acc=" << getStringEnum_AccState_e(acc) << ", ign1=" << getStringEnum_IgnState_e(ign1) << ", ign2=" << getStringEnum_IgnState_e(ign2) << "\n";
		bool notify_info_by_change = false;

		if (this->mIsNotified)
		{
			notify_info_by_change = ((this->mLastGear != gear) || (this->mLastAcc != acc) || (this->mLastIgn1 != ign1) || (this->mLastIgn2 != ign2));
			if (!notify_info_by_change)
			{
				notify_info_by_change = (((vehicleSpeed != 0) && (this->mLastVehicleSpeed == 0)) || ((vehicleSpeed == 0) && (this->mLastVehicleSpeed != 0)));
			}
		}
		else
			{
			notify_info_by_change = true;
		}

		this->mIsNotified = true;
		this->mLastVehicleSpeed = vehicleSpeed;
		this->mLastGear = gear;
		this->mLastAcc = acc;
		this->mLastIgn1 = ign1;
		this->mLastIgn2 = ign2;
		this->mLastNotifiedTick = util::timer::SimpleTimer::GetSystemTick();

		if (s_multicastServerId > 0)
		{
			std::shared_ptr<MulticastServer> server = MulticastServerManager::GetInstance()->GetServer(s_multicastServerId);
			if (server != nullptr)
			{
				std::uint8_t data[8];
				data[0] = (std::uint8_t)((vehicleSpeed >> 8) & 0xff);
				data[1] = (std::uint8_t)(vehicleSpeed & 0xff);
				data[2] = (std::uint8_t)gear;
				data[3] = (std::uint8_t)acc;
				data[4] = (std::uint8_t)ign1;
				data[5] = (std::uint8_t)ign2;
				server->SendData(data, (int)sizeof(data));
			}
		}

		if (!notify_info_by_change)
		{
			// Notify Inside

		}
	}
};

void Zone1_VehicleInfo_Init(void)
{
	auto *inst = Zone1VehicleInfoProxyImpl::GetInstance();
	inst->setEventListener(std::make_shared<VehicleInfoListener>());
	inst->init();

	s_multicastServerId = MulticastServerManager::GetInstance()->CreateServer(MULTICAST_VEHICLEINFO_IP, MULTICAST_VEHICLEINFO_PORT);
}

void Zone1_VehicleInfo_Term(void)
{
	Zone1VehicleInfoProxyImpl::DeleteInstance();
}
