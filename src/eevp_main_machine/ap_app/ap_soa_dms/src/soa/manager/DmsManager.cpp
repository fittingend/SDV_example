#include <Common.h>
#include <csignal>
#include <memory>
#include <MessageItem.hpp>
#include <VehicleContext.hpp>
#include <DmsManager.hpp>
#include <Api_SoaDms.hpp>
#include <Api_Zone5Dms.hpp>
#include <MulticastClientManager.hpp>

#include <Log.hpp>
#include <Debug_DataType2String.hpp>

class VehicleInfoListener : public MulticastClientListener {
public:
	void onReceived(int client_id, const std::uint8_t *data, const int &len) override
	{
		if (len >= 6)
		{
			int vehicleSpeed = (((int)data[0] << 8) & 0xFF00) | ((int)data[1] & 0x00FF);
			GearState_e gear = (GearState_e)data[2];
			AccState_e acc = (AccState_e)data[3];
			IgnState_e ign1 = (IgnState_e)data[4];
			IgnState_e ign2 = (IgnState_e)data[5];

			// LOG_DEBUG() << "vehicleSpeed=" << vehicleSpeed << ", gear=" << getStringEnum_GearState_e(gear) << ", acc=" << getStringEnum_AccState_e(acc) << ", ign1=" << getStringEnum_IgnState_e(ign1) << ", ign2=" << getStringEnum_IgnState_e(ign2) << "\n";
			DmsManager::GetInstance()->VehicleInfoReceived(vehicleSpeed, gear, acc, ign1, ign2);
		}
		else
		{
			LOG_ERROR() << "client_id=" << client_id << ", data_size=" << len << ", but it shall be larger than 6...\n";
		}
	}
};

DmsManager *DmsManager::mInst = nullptr;

DmsManager *DmsManager::GetInstance()
{
	if (DmsManager::mInst == nullptr)
	{
		DmsManager::mInst = new DmsManager();
	}

	return DmsManager::mInst;
}

void DmsManager::RemoveInstance()
{
	if (DmsManager::mInst != nullptr)
	{
		DmsManager::mInst->Stop();
		delete DmsManager::mInst;
		DmsManager::mInst = nullptr;
	}
}

DmsManager::DmsManager()
{
	this->mMulticastClientId = -1;
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		this->init();
		return 0;
	});
}

DmsManager::~DmsManager()
{
	LOG_DEBUG() << "(+)\n";

	std::shared_ptr<MulticastClient> client = MulticastClientManager::GetInstance()->GetClient(this->mMulticastClientId);
	if (client != nullptr)
	{
		client->StopClient();
	}

	MulticastClientManager::RemoveInstance();

	LOG_DEBUG() << "(-)\n";

}

void DmsManager::init()
{
	LOG_DEBUG() << "(+)\n";

	MulticastClientManager *multicast = MulticastClientManager::GetInstance();
	this->mMulticastClientId = multicast->CreateClient(MULTICAST_VEHICLEINFO_IP, MULTICAST_VEHICLEINFO_PORT);
	std::shared_ptr<MulticastClient> client = multicast->GetClient(this->mMulticastClientId);
	if (client != nullptr)
	{
		client->RegisterListener(std::make_shared<VehicleInfoListener>());
		client->RunClient();
	}

	LOG_DEBUG() << "(-)\n";
}

void DmsManager::cmdSetPower(const ClientLevel_e &level, const bool &on)
{
	LOG_DEBUG() << "(+) level=" << getStringEnum_ClientLevel_e(level) << ", on=" << getStringEnum_bool(on) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mPowerOn != on)
	{
		context->mPowerOn = on;
		SOA_DMS_UpdateDmsStatus();
	}

	Zone5_DMS_SetPower(on);

	LOG_DEBUG() << "(-)\n";
}

void DmsManager::onDeviceNormalChanged(const DeviceNormal_e &isNormal)
{
	LOG_DEBUG() << "(+) isNormal=" << getStringEnum_DeviceNormal_e(isNormal) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mDeviceNormal != isNormal)
	{
		context->mDeviceNormal = isNormal;
		SOA_DMS_UpdateDeviceNormal();
	}

	LOG_DEBUG() << "(-)\n";
}

void DmsManager::onDmsStatusChanged(const bool &powerOn, const DmsDriverInside_e &inside)
{
	LOG_DEBUG() << "(+) powerOn=" << getStringEnum_bool(powerOn) << ", inside=" << getStringEnum_DmsDriverInside_e(inside) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	bool isChanged = false;

	if (context->mPowerOn != powerOn)
	{
		context->mPowerOn = powerOn;
		isChanged = true;
	}

	if (context->mDriverInside != inside)
	{
		context->mDriverInside = inside;
		isChanged = true;
	}

	if (isChanged)
	{
		SOA_DMS_UpdateDmsStatus();
	}

	LOG_DEBUG() << "(-)\n";
}

void DmsManager::onDriverStatusChanged(const DmsGazingDir_e &dir, const DmsFatigue_e &fatigue, const DmsSmoking_e &smoking, const DmsOnPhone_e &phone)
{
	LOG_DEBUG() << "(+) dir=" << getStringEnum_DmsGazingDir_e(dir) << ", fatigue=" << getStringEnum_DmsFatigue_e(fatigue) << ", smoking=" << getStringEnum_DmsSmoking_e(smoking) << ", phone=" << getStringEnum_DmsOnPhone_e(phone) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	bool isChanged = false;

	if (context->mGazingDir != dir)
	{
		context->mGazingDir = dir;
		isChanged = true;
	}

	if (context->mFatigue != fatigue)
	{
		context->mFatigue = fatigue;
		isChanged = true;
	}

	if (context->mIsSmoking != smoking)
	{
		context->mIsSmoking = smoking;
		isChanged = true;
	}

	if (context->mIsOnPhone != phone)
	{
		context->mIsOnPhone = phone;
		isChanged = true;
	}

	if (isChanged)
	{
		SOA_DMS_UpdateDriverStatus();
	}

	LOG_DEBUG() << "(-)\n";
}



void DmsManager::onVehicleInfoReceived(const int &vehicleSpeed, const GearState_e &gear, const AccState_e &acc, const IgnState_e &ign1, const IgnState_e &ign2)
{
	LOG_DEBUG() << "(+) vehicleSpeed=" << vehicleSpeed << ", gear=" << getStringEnum_GearState_e(gear) << ", acc=" << getStringEnum_AccState_e(acc) << ", ign1=" << getStringEnum_IgnState_e(ign1) << ", ign2=" << getStringEnum_IgnState_e(ign2) << "\n";

	LOG_DEBUG() << "(-)\n";
}

void DmsManager::SetPower(const ClientLevel_e &level, const bool &on)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		ClientLevel_e _level = (ClientLevel_e)arg1;
		bool _on = (bool)arg2;
		this->cmdSetPower(_level, _on);
		return 0;
	}, (int)level, (int)on);
	// LOG_DEBUG() << "(-)\n";
}

void DmsManager::DeviceNormalChanged(const DeviceNormal_e &isNormal)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		DeviceNormal_e _isNormal = (DeviceNormal_e)arg1;
		this->onDeviceNormalChanged(_isNormal);
		return 0;
	}, (int)isNormal);
	// LOG_DEBUG() << "(-)\n";
}

void DmsManager::DmsStatusChanged(const bool &powerOn, const DmsDriverInside_e &inside)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		bool _powerOn = (bool)arg1;
		DmsDriverInside_e _inside = (DmsDriverInside_e)arg2;
		this->onDmsStatusChanged(_powerOn, _inside);
		return 0;
	}, (int)powerOn, (int)inside);
	// LOG_DEBUG() << "(-)\n";
}

void DmsManager::DriverStatusChanged(const DmsGazingDir_e &dir, const DmsFatigue_e &fatigue, const DmsSmoking_e &smoking, const DmsOnPhone_e &phone)
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		DmsGazingDir_e _dir = (DmsGazingDir_e)arg1;
		DmsFatigue_e _fatigue = (DmsFatigue_e)arg2;
		DmsSmoking_e _smoking = (DmsSmoking_e)arg3;
		DmsOnPhone_e _phone = (DmsOnPhone_e)arg4;
		this->onDriverStatusChanged(_dir, _fatigue, _smoking, _phone);
		return 0;
	}, (int)dir, (int)fatigue, (int)smoking, (int)phone);
	// LOG_DEBUG() << "(-)\n";
}

void DmsManager::VehicleInfoReceived(const int &vehicleSpeed, const GearState_e &gear, const AccState_e &acc, const IgnState_e &ign1, const IgnState_e &ign2)
{
	int _arg2 = (((int)gear & 0x00FF) << 8) | ((int)acc & 0x00FF);
	int _arg3 = (((int)ign1 & 0x00FF) << 8) | ((int)ign2 & 0x00FF);

	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		int _vehicleSpeed = arg1;
		GearState_e _gear = (GearState_e)((arg2 >> 8) & 0x00FF);
		AccState_e _acc = (AccState_e)(arg2 && 0x00FF);
		IgnState_e _ign1 = (IgnState_e)((arg3 >> 8) & 0x00FF);
		IgnState_e _ign2 = (IgnState_e)(arg3 & 0x00FF);
		this->onVehicleInfoReceived(_vehicleSpeed, _gear, _acc, _ign1, _ign2);
		return 0;
	}, vehicleSpeed, _arg2, _arg3);

}

void DmsManager::NotifyOnDriverNotFocusing()
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		SOA_DMS_NotifyOnDriverNotFocusing();
		return 0;
	});
	// LOG_DEBUG() << "(-)\n";
}

void DmsManager::NotifyOnDriverNotWaken()
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		SOA_DMS_NotifyOnDriverNotWaken();
		return 0;
	});
	// LOG_DEBUG() << "(-)\n";
}

void DmsManager::NotifyOnDriverNotWatchingFront()
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		SOA_DMS_NotifyOnDriverNotWatchingFront();
		return 0;
	});
	// LOG_DEBUG() << "(-)\n";
}
