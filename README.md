# Spotify Terminal Controller

A backend C++ application that interfaces with the Spotify Web API to control 
music playback directly from the command line. Built to deepen my understanding 
of REST APIs, OAuth 2.0 authentication flows, and systems-level programming.

## What It Does
Authenticates with Spotify using OAuth 2.0 and lets you control playback 
from your terminal — play, pause, skip tracks, and see what's currently playing, 
all through real API calls.

## Technical Highlights
- Implemented OAuth 2.0 authorization code flow from scratch in C++
- Made HTTP requests to REST API endpoints using libcurl
- Parsed and handled JSON responses using nlohmann/json
- Managed build system and third-party dependencies using CMake and vcpkg
- Handled API authentication, token exchange, and session management

## Tech Stack
- C++17
- Spotify Web API (REST)
- OAuth 2.0
- libcurl
- nlohmann/json
- CMake

## Setup
1. Clone this repository
2. Install vcpkg and run: 
vcpkg install curl:x64-mingw-dynamic
vcpkg install nlohmann-json:x64-mingw-dynamic
3. Create a Spotify app at developer.spotify.com
4. Replace `YOUR_CLIENT_ID` and `YOUR_CLIENT_SECRET` in `main.cpp`
5. Build with CMake and run

## Commands
| Command | Action |
|---------|--------|
| now | Show currently playing song |
| play | Resume playback |
| pause | Pause playback |
| next | Skip to next track |
| prev | Go to previous track |
| quit | Exit the program |