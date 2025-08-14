#include <Zone1ReservedAProxyImpl.hpp>
#include <Zone2ReservedAProxyImpl.hpp>
#include <Zone3ReservedAProxyImpl.hpp>
#include <Zone4ReservedAProxyImpl.hpp>
#include <VehicleContext.hpp>
#include <Api_ZoneReserved.hpp>
#include <Api_SoaReserved.hpp>
#include <Log.hpp>

static DeviceNormal_e getTotalResult()
{
	VehicleContext *context = VehicleContext::GetInstance();

	if ((context->mZone1Connected != true) || (context->mZone1PowerNormal != eDeviceNormal_Ok))
		return eDeviceNormal_Abnormal;
	if ((context->mZone2Connected != true) || (context->mZone2PowerNormal != eDeviceNormal_Ok))
		return eDeviceNormal_Abnormal;
	if ((context->mZone3Connected != true) || (context->mZone3PowerNormal != eDeviceNormal_Ok))
		return eDeviceNormal_Abnormal;
	if ((context->mZone4Connected != true) || (context->mZone4PowerNormal != eDeviceNormal_Ok))
		return eDeviceNormal_Abnormal;

	return eDeviceNormal_Ok;
}

static void updateTotalResult()
{
	DeviceNormal_e currTotalResult = getTotalResult();
	VehicleContext *context = VehicleContext::GetInstance();

	if (context->mTotalResult != currTotalResult)
	{
		context->mTotalResult = currTotalResult;
		SOA_ReservedA_UpdatePowerDeviceNormal();
	}
}

class Zone1Linstener : public zone1::reserved::a::IZone1ReservedAListener
{
	void notifyDeviceNormal(const std::uint8_t &fieldValue)
	{
		LOG_DEBUG() << "Zone1Linstener " << (int)fieldValue << "\n";
		VehicleContext *context = VehicleContext::GetInstance();
		bool is_changed = false;
		DeviceNormal_e normal;

		if (context->mZone1Connected != true)
		{
			context->mZone1Connected = true;
			is_changed = true;
		}

		switch (fieldValue)
		{
		case 0:
			normal = eDeviceNormal_Ok;
			break;
		case 1:
		default:
			normal = eDeviceNormal_Abnormal;
			break;
		}

		if (context->mZone1PowerNormal != normal)
		{
			context->mZone1PowerNormal = normal;
			is_changed = true;
		}

		if (is_changed)
		{
			LOG_DEBUG() << "Zone1Linstener changed" << (int)fieldValue << "\n";
			updateTotalResult();
		}
	}
};

class Zone2Linstener : public zone2::reserved::a::IZone2ReservedAListener
{
	void notifyDeviceNormal(const std::uint8_t &fieldValue)
	{
		LOG_DEBUG() << "Zone2Linstener " << (int)fieldValue << "\n";
		VehicleContext *context = VehicleContext::GetInstance();
		bool is_changed = false;
		DeviceNormal_e normal;

		if (context->mZone2Connected != true)
		{
			context->mZone2Connected = true;
			is_changed = true;
		}

		switch (fieldValue)
		{
		case 0:
			normal = eDeviceNormal_Ok;
			break;
		case 1:
		default:
			normal = eDeviceNormal_Abnormal;
			break;
		}

		if (context->mZone2PowerNormal != normal)
		{
			context->mZone2PowerNormal = normal;
			is_changed = true;
		}

		if (is_changed)
		{
			LOG_DEBUG() << "Zone2Linstener changed" << (int)fieldValue << "\n";
			updateTotalResult();
		}
	}
};

class Zone3Linstener : public zone3::reserved::a::IZone3ReservedAListener
{
	void notifyDeviceNormal(const std::uint8_t &fieldValue)
	{
		LOG_DEBUG() << "Zone3Linstener " << (int)fieldValue << "\n";
		VehicleContext *context = VehicleContext::GetInstance();
		bool is_changed = false;
		DeviceNormal_e normal;

		if (context->mZone3Connected != true)
		{
			context->mZone3Connected = true;
			is_changed = true;
		}

		switch (fieldValue)
		{
		case 0:
			normal = eDeviceNormal_Ok;
			break;
		case 1:
		default:
			normal = eDeviceNormal_Abnormal;
			break;
		}

		if (context->mZone3PowerNormal != normal)
		{
			context->mZone3PowerNormal = normal;
			is_changed = true;
		}

		if (is_changed)
		{
			LOG_DEBUG() << "Zone3Listener changed" << (int)fieldValue << "\n";
			updateTotalResult();
		}
	}
};

class Zone4Linstener : public zone4::reserved::a::IZone4ReservedAListener
{
	void notifyDeviceNormal(const std::uint8_t &fieldValue)
	{
		LOG_DEBUG() << "Zone4Linstener " << (int)fieldValue << "\n";
		VehicleContext *context = VehicleContext::GetInstance();
		bool is_changed = false;
		DeviceNormal_e normal;

		if (context->mZone4Connected != true)
		{
			context->mZone4Connected = true;
			is_changed = true;
		}

		switch (fieldValue)
		{
		case 0:
			normal = eDeviceNormal_Ok;
			break;
		case 1:
		default:
			normal = eDeviceNormal_Abnormal;
			break;
		}

		if (context->mZone4PowerNormal != normal)
		{
			context->mZone4PowerNormal = normal;
			is_changed = true;
		}

		if (is_changed)
		{
			LOG_DEBUG() << "Zone4Linstener changed" << (int)fieldValue << "\n";
			updateTotalResult();
		}
	}
};

void ZONE_RESERVED_Init(void)
{
	VehicleContext *context = VehicleContext::GetInstance();
	auto *zone1_rsv_inst = zone1::reserved::a::Zone1ReservedAProxyImpl::GetInstance();
	zone1_rsv_inst->setEventListener(std::make_shared<Zone1Linstener>());
	if (zone1_rsv_inst->init())
	{
		context->mZone1Connected = true;
		uint8_t is_normal = -1;

#if 0
		if (zone1_rsv_inst->getterDeviceNormal(is_normal))
		{
			context->mZone1PowerNormal = (is_normal == 0) ? eDeviceNormal_Ok : eDeviceNormal_Abnormal;
		}
#endif
	}

	auto *zone2_rsv_inst = zone2::reserved::a::Zone2ReservedAProxyImpl::GetInstance();
	zone2_rsv_inst->setEventListener(std::make_shared<Zone2Linstener>());
	if (zone2_rsv_inst->init())
	{
		context->mZone2Connected = true;
		uint8_t is_normal = -1;

#if 0
		if (zone2_rsv_inst->getterDeviceNormal(is_normal))
		{
			context->mZone2PowerNormal = (is_normal == 0) ? eDeviceNormal_Ok : eDeviceNormal_Abnormal;
		}
#endif
	}

	auto *zone3_rsv_inst = zone3::reserved::a::Zone3ReservedAProxyImpl::GetInstance();
	zone3_rsv_inst->setEventListener(std::make_shared<Zone3Linstener>());
	if (zone3_rsv_inst->init())
	{
		context->mZone3Connected = true;
		uint8_t is_normal = -1;

#if 0
		if (zone3_rsv_inst->getterDeviceNormal(is_normal))
		{
			context->mZone3PowerNormal = (is_normal == 0) ? eDeviceNormal_Ok : eDeviceNormal_Abnormal;
		}
#endif
	}

	auto *zone4_rsv_inst = zone4::reserved::a::Zone4ReservedAProxyImpl::GetInstance();
	zone4_rsv_inst->setEventListener(std::make_shared<Zone4Linstener>());
	if (zone4_rsv_inst->init())
	{
		context->mZone4Connected = true;
		uint8_t is_normal = -1;

#if 0
		if (zone4_rsv_inst->getterDeviceNormal(is_normal))
		{
			context->mZone4PowerNormal = (is_normal == 0) ? eDeviceNormal_Ok : eDeviceNormal_Abnormal;
		}
#endif
	}

	updateTotalResult();
}

void ZONE_RESERVED_Term(void)
{
	zone1::reserved::a::Zone1ReservedAProxyImpl::DeleteInstance();
	zone2::reserved::a::Zone2ReservedAProxyImpl::DeleteInstance();
	zone3::reserved::a::Zone3ReservedAProxyImpl::DeleteInstance();
	zone4::reserved::a::Zone4ReservedAProxyImpl::DeleteInstance();
}

void ZONE_RESERVED_UpdateContext(void)
{
	VehicleContext *context = VehicleContext::GetInstance();
	std::uint8_t is_normal;

	context->mZone1Connected = false;
	context->mZone1PowerNormal = eDeviceNormal_Abnormal;
	context->mZone2Connected = false;
	context->mZone2PowerNormal = eDeviceNormal_Abnormal;
	context->mZone3Connected = false;
	context->mZone3PowerNormal = eDeviceNormal_Abnormal;
	context->mZone4Connected = false;
	context->mZone4PowerNormal = eDeviceNormal_Abnormal;

	auto *inst1 = zone1::reserved::a::Zone1ReservedAProxyImpl::GetInstance();
	if ((inst1) && (inst1->getterDeviceNormal(is_normal)))
	{
		context->mZone1Connected = true;
		context->mZone1PowerNormal = (is_normal == 0) ? eDeviceNormal_Ok : eDeviceNormal_Abnormal;
	}

	auto *inst2 = zone2::reserved::a::Zone2ReservedAProxyImpl::GetInstance();
	if ((inst2) && (inst2->getterDeviceNormal(is_normal)))
	{
		context->mZone2Connected = true;
		context->mZone2PowerNormal = (is_normal == 0) ? eDeviceNormal_Ok : eDeviceNormal_Abnormal;
	}

	auto *inst3 = zone3::reserved::a::Zone3ReservedAProxyImpl::GetInstance();
	if ((inst3) && (inst3->getterDeviceNormal(is_normal)))
	{
		context->mZone3Connected = true;
		context->mZone3PowerNormal = (is_normal == 0) ? eDeviceNormal_Ok : eDeviceNormal_Abnormal;
	}

	auto *inst4 = zone4::reserved::a::Zone4ReservedAProxyImpl::GetInstance();
	if ((inst4) && (inst4->getterDeviceNormal(is_normal)))
	{
		context->mZone4Connected = true;
		context->mZone4PowerNormal = (is_normal == 0) ? eDeviceNormal_Ok : eDeviceNormal_Abnormal;
	}

	updateTotalResult();
}