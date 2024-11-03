#include "bakkesmod/plugin/bakkesmodplugin.h"
#include <string>
#include <iostream>
#include <curl/curl.h>
#include <json/json.h>
#include <sapi.h> // Windows TTS

class AICoachPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
public:
    virtual void onLoad();
    virtual void onUnload();

private:
    void PollGameData();
    void SendDataToLocalAI(const std::string& prompt);
    void PlayAudioFeedback(const std::string& feedbackText);

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
        userp->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    float pollInterval = 5.0f; // Poll every 5 seconds
};

void AICoachPlugin::onLoad()
{
    gameWrapper->SetTimeout([this](GameWrapper* gw) {
        PollGameData();
    }, pollInterval);
}

void AICoachPlugin::onUnload() {}

void AICoachPlugin::PollGameData()
{
    auto game = gameWrapper->GetGameEventAsServer();
    if (!game) return;

    Vector position = game->GetCars().at(0)->GetLocation();
    float boost = game->GetCars().at(0)->GetBoost();

    Json::Value gameData;
    gameData["position"]["x"] = position.X;
    gameData["position"]["y"] = position.Y;
    gameData["position"]["z"] = position.Z;
    gameData["boost"] = boost;

    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, gameData);

    SendDataToLocalAI(jsonString);
}

void AICoachPlugin::SendDataToLocalAI(const std::string& prompt)
{
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5000/generate");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, prompt.c_str());

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::string response_data;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            Json::CharReaderBuilder reader;
            Json::Value jsonResponse;
            std::istringstream stream(response_data);
            std::string feedbackText;

            if (Json::parseFromStream(reader, stream, &jsonResponse, nullptr)) {
                feedbackText = jsonResponse["response"].asString();
                PlayAudioFeedback(feedbackText);
            }
        }

        curl_easy_cleanup(curl);
    }
}

void AICoachPlugin::PlayAudioFeedback(const std::string& feedbackText)
{
    ISpVoice* pVoice = NULL;
    if (FAILED(::CoInitialize(NULL))) return;

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
    if (SUCCEEDED(hr)) {
        std::wstring wideText = std::wstring(feedbackText.begin(), feedbackText.end());
        pVoice->Speak(wideText.c_str(), SPF_ASYNC, NULL);
        pVoice->Release();
        pVoice = NULL;
    }
    ::CoUninitialize();
}
