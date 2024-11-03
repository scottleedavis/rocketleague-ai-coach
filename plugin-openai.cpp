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
    void SendDataToOpenAI(const std::string& gameData);
    void PlayAudioFeedback(const std::string& feedbackText);

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
        userp->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    float pollInterval = 5.0f; // Poll every 5 seconds
};

// Initialize and start polling game data
void AICoachPlugin::onLoad()
{
    gameWrapper->SetTimeout([this](GameWrapper* gw) {
        PollGameData();
    }, pollInterval);
}

// Unload plugin
void AICoachPlugin::onUnload()
{
    // Optional cleanup if necessary
}

// Poll game data
void AICoachPlugin::PollGameData()
{
    auto game = gameWrapper->GetGameEventAsServer();
    if (!game) return;

    // Collect game metrics
    Vector position = game->GetCars().at(0)->GetLocation();
    float boost = game->GetCars().at(0)->GetBoost();

    // Prepare JSON with game data
    Json::Value gameData;
    gameData["position"]["x"] = position.X;
    gameData["position"]["y"] = position.Y;
    gameData["position"]["z"] = position.Z;
    gameData["boost"] = boost;

    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, gameData);

    // Send data to OpenAI
    SendDataToOpenAI(jsonString);
}

// Send data to OpenAI API
void AICoachPlugin::SendDataToOpenAI(const std::string& gameData)
{
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string api_key = "YOUR_OPENAI_API_KEY";
        std::string url = "https://api.openai.com/v1/chat/completions";

        Json::Value requestBody;
        requestBody["model"] = "gpt-4"; // Specify the model
        requestBody["messages"] = Json::arrayValue;
        requestBody["messages"][0]["role"] = "system";
        requestBody["messages"][0]["content"] = "You are an experienced Rocket League coach providing in-game guidance.";
        requestBody["messages"][1]["role"] = "user";
        requestBody["messages"][1]["content"] = "Based on this in-game data, give real-time coaching suggestions: " + gameData;

        Json::StreamWriterBuilder writer;
        std::string jsonString = Json::writeString(writer, requestBody);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key).c_str());
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
                feedbackText = jsonResponse["choices"][0]["message"]["content"].asString();
                PlayAudioFeedback(feedbackText);
            }
        }

        curl_easy_cleanup(curl);
    }
}

// Play audio feedback using Windows TTS
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
