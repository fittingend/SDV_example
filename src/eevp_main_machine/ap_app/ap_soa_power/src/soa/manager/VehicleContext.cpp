#include <Common.h>
#include <SoaDataType.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <string.h>
#include <VehicleContext.hpp>
#include <Log.hpp>

#define VEHICLE_CONTEXT_CONF_FILE "verhicle_context.conf"
#define CONF_FILE_HEAD "VC_CONF"
#define CONF_FILE_VERSION 1

VehicleContext *VehicleContext::mInst = nullptr;

VehicleContext *VehicleContext::GetInstance()
{
	if (VehicleContext::mInst == nullptr)
	{
		VehicleContext::mInst = new VehicleContext();
	}

	return VehicleContext::mInst;
}

VehicleContext::VehicleContext()
{
	this->mZone1Connected = false;
	this->mZone1PowerNormal = eDeviceNormal_Abnormal;
	this->mZone2Connected = false;
	this->mZone2PowerNormal = eDeviceNormal_Abnormal;
	this->mZone3Connected = false;
	this->mZone3PowerNormal = eDeviceNormal_Abnormal;
	this->mZone4Connected = false;
	this->mZone4PowerNormal = eDeviceNormal_Abnormal;

	this->mTotalResult = eDeviceNormal_Abnormal;
}

VehicleContext::~VehicleContext()
{

}

int VehicleContext::Load()
{
	struct stat file_stat;
	if (stat(DATAFILE_DIR_PATH, &file_stat) != 0)
	{
		LOG_ERROR() << "[VehicleContext::Load] path [" << DATAFILE_DIR_PATH << "] not exists...\n";
		return -1;
	}

	if (!S_ISDIR(file_stat.st_mode))
	{
		LOG_ERROR() << "[VehicleContext::Load] path [" << DATAFILE_DIR_PATH << "] not directory...\n";
		return -1;
	}

	std::stringstream ss;
	ss << DATAFILE_DIR_PATH << "/" << VEHICLE_CONTEXT_CONF_FILE;
	const char *file_path = ss.str().c_str();
	if (stat(file_path, &file_stat) != 0)
	{
		LOG_ERROR() << "[VehicleContext::Load] path [" << file_path << "] not exists...\n";
		return -1;
	}

	if (!S_ISREG(file_stat.st_mode))
	{
		LOG_ERROR() << "[VehicleContext::Load] path [" << file_path << "] not file...\n";
		return -1;
	}

	std::ifstream ifs(file_path, std::ios::binary);
	if (!ifs)
	{
		LOG_ERROR() << "[VehicleContext::Load] file [" << file_path << "] not opened for reading...\n";
		return -1;
	}

	unsigned char buffer[strlen(CONF_FILE_HEAD) + 1];
	ifs.read(reinterpret_cast<char *>(buffer), strlen(CONF_FILE_HEAD));
	if (memcmp(buffer, CONF_FILE_HEAD, sizeof(CONF_FILE_HEAD)) != 0)
	{
		LOG_ERROR() << "[VehicleContext::Load] file [" << file_path << "] is not VehicleContext Config file...\n";
		ifs.close();
		return -1;
	}

	int version;
	ifs.read(reinterpret_cast<char *>(&version), sizeof(version));
	if (version != CONF_FILE_VERSION)
	{
		LOG_ERROR() << "[VehicleContext::Load] version different: file(" << version << "), Current(" << CONF_FILE_VERSION << ")\n";
	}

	ifs.close();
	return 0;
}

int VehicleContext::Save()
{
	struct stat file_stat;
	if (stat(DATAFILE_DIR_PATH, &file_stat) != 0)
	{
		LOG_ERROR() << "[VehicleContext::Save] path [" << DATAFILE_DIR_PATH << "] not exists...\n";
		return -1;
	}

	if (!S_ISDIR(file_stat.st_mode))
	{
		LOG_ERROR() << "[VehicleContext::Save] path [" << DATAFILE_DIR_PATH << "] not directory...\n";
		return -1;
	}

	std::stringstream ss;
	ss << DATAFILE_DIR_PATH << "/" << VEHICLE_CONTEXT_CONF_FILE;
	const char *file_path = ss.str().c_str();

	std::ofstream ofs(file_path, std::ios::binary | std::ios::out);
	if (!ofs)
	{
		LOG_ERROR() << "[VehicleContext::Save] file [" << file_path << "] not opened for writing...\n";
		return -1;
	}

	ofs.write(CONF_FILE_HEAD, strlen(CONF_FILE_HEAD));

	int version = CONF_FILE_VERSION;
	ofs.write(reinterpret_cast<const char *>(&version), sizeof(version));

	ofs.close();
	return 0;
}


//
// For Debugging
//

#include <Debug_DataType2String.hpp>

std::string VehicleContext::DebugInfo()
{
	std::ostringstream ss;

	ss << "VehicleContext:\n";
	ss << "- Total Result=" << ((this->mTotalResult == eDeviceNormal_Ok) ? "OK" : "Abnormal") << "\n";
	ss << "- zone1: connected=" << ((this->mZone1Connected) ? "Yes" : "No") << ", state=" << getStringEnum_DeviceNormal_e(this->mZone1PowerNormal) << "\n";
	ss << "- zone2: connected=" << ((this->mZone2Connected) ? "Yes" : "No") << ", state=" << getStringEnum_DeviceNormal_e(this->mZone2PowerNormal) << "\n";
	ss << "- zone3: connected=" << ((this->mZone3Connected) ? "Yes" : "No") << ", state=" << getStringEnum_DeviceNormal_e(this->mZone3PowerNormal) << "\n";
	ss << "- zone4: connected=" << ((this->mZone4Connected) ? "Yes" : "No") << ", state=" << getStringEnum_DeviceNormal_e(this->mZone4PowerNormal) << "\n";
	return ss.str();
}