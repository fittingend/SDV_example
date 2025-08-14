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
	this->mIsNormal = eDeviceNormal_Abnormal;
	this->mRunningState = eRoaRunningState_Off;
	this->mRoaMode = eRoaMode_Advanced;
	this->mDetectCount = 0;
	this->mSensorError = eRoaSensorError_Ok;
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

	// Implement the load function using ifs:

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

	// Implement the load function using ifs:

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
	ss << "- DeviceIsNormal : " << getStringEnum_DeviceNormal_e(this->mIsNormal) << "\n";
	ss << "- Running State : " << getStringEnum_RoaRunningState_e(this->mRunningState) << "\n";
	ss << "- ROA mode : " << getStringEnum_RoaMode_e(this->mRoaMode) << "\n";
	ss << "- Detect Count : " << this->mDetectCount << "\n";
	ss << "- Running State : " << getStringEnum_RoaRunningState_e(this->mRunningState) << "\n";
	ss << "- Sensor Error : " << getStringEnum_RoaSensorError_e(this->mSensorError) << "\n";

	return ss.str();
}