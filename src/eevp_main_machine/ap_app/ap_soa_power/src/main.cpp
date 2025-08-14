#include <Common.h>
#include <thread>
#include <csignal>

#include <ManagerBase.hpp>
#include <Sleeper.hpp>
#include <VehicleContext.hpp>
#include <TelnetServer.hpp>
#include <CmdLists.hpp>
#include <PaconSetting.hpp>
#include <Log.hpp>

using namespace std;

extern void DevicesInit(void);

static Sleeper s_sleeper;

static void stopApp(void)
{
	s_sleeper.WakeUp();
}

static void signalHandler(std::int32_t signal)
{
	stopApp();
}

int main(int argc, char *argv[])
{
	std::signal(SIGINT, signalHandler);
	std::signal(SIGTERM, signalHandler);

	VehicleContext *context = VehicleContext::GetInstance();

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

	// TODO: your code here
	DevicesInit();

	s_sleeper.Sleep();

#if defined(DEBUG_TELNET)
	telnetServer->Stop();
	TelnetServer::DestroyInstance();
#endif

	ManagerBase::StopAllManagers();

	(void)PaconSetting::RemoveInstance();
	return 0;
}
