#include "bakkesmod/plugin/bakkesmodplugin.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

class BasicLoggerPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
public:
    virtual void onLoad();
    virtual void onUnload();

private:
    void LogEvent();
    void StartLogging();
};
