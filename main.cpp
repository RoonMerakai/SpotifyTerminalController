#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// My Spotify credentials
const std::string CLIENT_ID = "319d0109ff6e4c3ebe1d9597484ae15b";
const std::string CLIENT_SECRET = "ee1d39e9a4314f9589e3c1a1cce4fed8";
const std::string REDIRECT_URI = "http://127.0.0.1:3000";

std::string access_token = "";

// Callback function for curl to store response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    s->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Make a GET request to Spotify API
std::string spotifyGet(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        struct curl_slist* headers = nullptr;
        std::string auth = "Authorization: Bearer " + access_token;
        headers = curl_slist_append(headers, auth.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return response;
}

// Make a PUT request (for play/pause/skip)
void spotifyPut(const std::string& url, const std::string& body = "") {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        struct curl_slist* headers = nullptr;
        std::string auth = "Authorization: Bearer " + access_token;
        headers = curl_slist_append(headers, auth.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        if (!body.empty()) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        } else {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
        }
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

// Make a POST request (for skip next)
void spotifyPost(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        struct curl_slist* headers = nullptr;
        std::string auth = "Authorization: Bearer " + access_token;
        headers = curl_slist_append(headers, auth.c_str());
        headers = curl_slist_append(headers, "Content-Length: 0");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

// Show currently playing song
void nowPlaying() {
    std::string response = spotifyGet("https://api.spotify.com/v1/me/player/currently-playing");
    if (response.empty()) {
        std::cout << "Nothing is currently playing.\n";
        return;
    }
    try {
        auto j = json::parse(response);
        std::string song = j["item"]["name"];
        std::string artist = j["item"]["artists"][0]["name"];
        bool is_playing = j["is_playing"];
        std::cout << "\n Now " << (is_playing ? "Playing" : "Paused") << ": " 
                  << song << " - " << artist << "\n";
    } catch (...) {
        std::cout << "Could not get current song info.\n";
    }
}

// Pause playback
void pause() {
    spotifyPut("https://api.spotify.com/v1/me/player/pause");
    std::cout << " Paused.\n";
}

// Resume playback
void play() {
    spotifyPut("https://api.spotify.com/v1/me/player/play");
    std::cout << " Playing.\n";
}

// Skip to next track
void skipNext() {
    spotifyPost("https://api.spotify.com/v1/me/player/next");
    std::cout << " Skipped to next track.\n";
}

// Skip to previous track
void skipPrev() {
    spotifyPost("https://api.spotify.com/v1/me/player/previous");
    std::cout << " Went back to previous track.\n";
}

// Get the auth URL for the user to visit
void printAuthUrl() {
    std::string url = "https://accounts.spotify.com/authorize"
        "?client_id=" + CLIENT_ID +
        "&response_type=code"
        "&redirect_uri=http%3A%2F%2F127.0.0.1%3A3000"
        "&scope=user-read-playback-state%20user-modify-playback-state%20user-read-currently-playing";
    std::cout << "\nOpen this URL in your browser to log in:\n\n" << url << "\n\n";
    std::cout << "After logging in, you'll be redirected to a page that won't load.\n";
    std::cout << "Copy the 'code' value from the URL and paste it here: ";
}

// Exchange auth code for access token
bool getAccessToken(const std::string& code) {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        std::string postdata = "grant_type=authorization_code&code=" + code +
            "&redirect_uri=http%3A%2F%2F127.0.0.1%3A3000" +
            "&client_id=" + CLIENT_ID +
            "&client_secret=" + CLIENT_SECRET;
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_URL, "https://accounts.spotify.com/api/token");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    try {
        auto j = json::parse(response);
        access_token = j["access_token"];
        std::cout << "\n Login successful!\n";
        return true;
    } catch (...) {
        std::cout << " Failed to get access token. Check your credentials.\n";
        return false;
    }
}

int main() {
    std::cout << "== Spotify Terminal Controller ==\n";

    // Step 1: Login
    printAuthUrl();
    std::string code;
    std::cin >> code;

    if (!getAccessToken(code)) return 1;

    // Step 2: Main control loop
    std::string cmd;
    std::cout << "\nCommands: now | play | pause | next | prev | quit\n";
    while (true) {
        std::cout << "\n> ";
        std::cin >> cmd;
        if (cmd == "now")   nowPlaying();
        else if (cmd == "play")  play();
        else if (cmd == "pause") pause();
        else if (cmd == "next")  skipNext();
        else if (cmd == "prev")  skipPrev();
        else if (cmd == "quit")  break;
        else std::cout << "Unknown command. Try: now | play | pause | next | prev | quit\n";
    }
    return 0;
}