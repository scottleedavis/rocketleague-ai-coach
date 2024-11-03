#include "plugin.h"

// Called when the plugin is loaded into BakkesMod
void BasicLoggerPlugin::onLoad()
{
    cvarManager->log("BasicLoggerPlugin loaded successfully!");

    // Start logging every second
    StartLogging();
}

// Called when the plugin is unloaded from BakkesMod
void BasicLoggerPlugin::onUnload()
{
    cvarManager->log("BasicLoggerPlugin unloaded.");
}

// Function that logs events every second
void BasicLoggerPlugin::StartLogging()
{
    gameWrapper->SetTimeout([this](GameWrapper* gw) {
        LogEvent();
        StartLogging(); // Recursively call to keep logging every second
    }, 1.0f); // 1.0f = 1 second interval
}

// Function to log a message to the console/log file
void BasicLoggerPlugin::LogEvent()
{
    std::string logMessage = "Event logged at " + std::to_string(std::time(nullptr));
    cvarManager->log(logMessage); // Logs to BakkesMod console and log file
}
