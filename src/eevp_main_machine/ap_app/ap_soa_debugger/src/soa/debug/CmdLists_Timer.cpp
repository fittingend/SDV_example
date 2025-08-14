#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <memory>
#include <sstream>
#include <string>
#include <string.h>
#include <algorithm>
#include <Common.h>
#include <SimpleTimer.hpp>
#include <Log.hpp>

using namespace util::timer;

static void s_runItemFunction(SimpleTimerItem *item);

class DebugTimerItem : public SimpleTimerItem
{
private:
    void run_func(DebugTimerItem *item)
    {
        LOG_DEBUG() << "Run : Key Value (" << item->mKey << ")\n";
    }

public:
    DebugTimerItem()
    {
        auto timer_item_runner = [this](SimpleTimerItem *val) {
            DebugTimerItem *item = (DebugTimerItem *)val;
            return this->run_func(this);
        };

        this->SetFunction(s_runItemFunction);
        // this->SetFunction(timer_item_runner);
    }

    ~DebugTimerItem()
    {
        LOG_DEBUG() << "~DebugTimerItem\n";
    }

    void RunFunc()
    {
        LOG_DEBUG() << "Run : Key Value (" << this->mKey << ")\n";
    }
};

static void s_runItemFunction(SimpleTimerItem *item)
{
    DebugTimerItem *item_d = (DebugTimerItem *)item;
    LOG_DEBUG() << "Run : Key Value (" << item_d->GetKey() << ")\n";
}

static int cmd_timer_insert(std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: timer insert [tick_time]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

/*
    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });
*/

    std::string arg = std::move(argList.front());
    argList.pop_front();

    int num;
    std::stringstream ss2(arg);
    ss2 >> num;
    if (ss2.fail())
    {
        ss << "Usage: timer insert [tick_time]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    DebugTimerItem *item = new DebugTimerItem();
    item->SetDuration((std::uint64_t)num);
    uint64_t key_value = item->GetKey();
    SimpleTimer::GetInstance()->InsertItem(item);

    ss << "Item (Key=" << key_value << ") inserted...";
    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

static int cmd_timer_delete(std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: timer insert [tick_time]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

/*
    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });
*/

    std::string arg = std::move(argList.front());
    argList.pop_front();

    int num;
    std::stringstream ss2(arg);
    ss2 >> num;
    if (ss2.fail())
    {
        ss << "Usage: timer delete [key_number]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    SimpleTimer::GetInstance()->DeleteItem((uint64_t)num);
    ss << "key_number [" << num << "] item deleted.";
    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

static int cmd_timer_list(std::list<std::string> &argList)
{
    std::stringstream ss;

    ss << SimpleTimer::GetInstance()->GetTimerStatus();
    CmdMgr::GetInstance()->Print(ss.str());
    return 0;
}


//
//  Root CMD Functions
//

static int cmd_SoaTimer(void *item, std::list<std::string> argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        return -1;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

    if (cmd == "insert")
    {
        return cmd_timer_insert(argList);
    }
    else if (cmd == "delete")
    {
        return cmd_timer_delete(argList);
    }
    else if (cmd == "list")
    {
        return cmd_timer_list(argList);
    }
    else
    {
        ss << "No Sub-Cmd(" << cmd << ") in the CMD [timer]";
        CmdMgr::GetInstance()->Println(ss.str());
        return -1;
    }

    return 0;
}

void RegisterCmdTimer()
{
    std::shared_ptr<SimpleTimer> inst = SimpleTimer::GetInstance();

    CmdMgr::GetInstance()->InsertCommand("timer",
            "timer [insert|delete|list] (args...)",
            "debug timer",
            "debug timer",
            cmd_SoaTimer, nullptr);
}