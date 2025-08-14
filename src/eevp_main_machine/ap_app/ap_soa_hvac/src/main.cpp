#include <Common.h>
#include <thread>
#include <csignal>

#include <ManagerBase.hpp>
#include <HvacManager.hpp>
#include <Sleeper.hpp>
#include <VehicleContext.hpp>
#include <TelnetServer.hpp>
#include <CmdLists.hpp>
#include <PaconSetting.hpp>
#include <Log.hpp>

using namespace std;

static Sleeper s_sleeper;

static void stopApp(void)
{
	s_sleeper.WakeUp();
}

static void signalHandler(std::int32_t signal)
{
	stopApp();
}

static void initServices(void)
{
	// init HVAC manager:
	(void)HvacManager::GetInstance();
}

static void termServices(void)
{
	// terminate HVAC manager:
	(void)HvacManager::RemoveInstance();
}


int main(int argc, char *argv[])
{
	std::signal(SIGINT, signalHandler);
	std::signal(SIGTERM, signalHandler);

	VehicleContext *context = VehicleContext::GetInstance();

	initServices();

	if (!PaconSetting::GetInstance()->StartPacon())
	{
		(void)PaconSetting::RemoveInstance();
		return 1;
	}

#if defined(DEBUG_TELNET)
	TelnetServer *telnetServer = TelnetServer::GetInstance();
	telnetServer->Start();
	RegisterEcuApi();
	RegisterCmdContext();
#endif

	s_sleeper.Sleep();

#if defined(DEBUG_TELNET)
	telnetServer->Stop();
	TelnetServer::DestroyInstance();
#endif

	(void)PaconSetting::RemoveInstance();
	termServices();
	return 0;
}
