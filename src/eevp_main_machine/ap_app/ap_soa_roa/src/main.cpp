#include <Common.h>
#include <thread>
#include <csignal>

#include <ManagerBase.hpp>
#include <RoaManager.hpp>
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

static void initServices(void)
{
	(void)util::timer::SimpleTimer::GetInstance();
	(void)RoaManager::GetInstance();
}

static void termServices(void)
{
	(void)util::timer::SimpleTimer::DestoryInstance();
	(void)RoaManager::RemoveInstance();
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
