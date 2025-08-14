#include <Common.h>
#include <thread>
#include <csignal>

#include <ManagerBase.hpp>
#include <DmsManager.hpp>
#include <SimpleTimer.hpp>
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
	(void)util::timer::SimpleTimer::GetInstance();
	(void)DmsManager::GetInstance();
}

static void termServices(void)
{
	(void)util::timer::SimpleTimer::DestoryInstance();
	(void)DmsManager::RemoveInstance();
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
		termServices();
		return 1;
	}

#if defined(DEBUG_TELNET)
	TelnetServer *telnetServer = TelnetServer::GetInstance();
	telnetServer->Start();
	RegisterEcuApi();
	RegisterSoaApi();
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
