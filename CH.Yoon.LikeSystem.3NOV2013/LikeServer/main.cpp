#include "like_server.h"
#include <json/json.h>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <cstdio>


bool GetPort(const char* sport, int& port) {
    char* end;
    port = strtol(sport, &end, 10);
    if (port == 0) {
        printf("[ERROR] port number is invalid value.\n");
        return false;
    }
    if ((port == LONG_MAX || port == LONG_MIN) && errno == ERANGE) {
        printf("[ERROR] port number is out of range.\n");
        return false;
    }
    return true;
}

bool ReadJson(const char* filepath, Json::Value& root) {
    if (!boost::filesystem::exists(filepath)) {
        printf("[ERROR] \"%s\" file not exists.\n", filepath);
        return false;
    }

    FILE* fp = fopen(filepath, "r");
    if (!fp) {
        printf("[ERROR] \"%s\" file couldn't open.\n", filepath);
        return false;
    }
    std::string json;
    char buffer[1024];
    size_t read_bytes = 0;
    do {
        read_bytes = fread(buffer, sizeof(char), 1024, fp);
        json.append(buffer, buffer + read_bytes);
    } while (read_bytes == 1024); 
    fclose(fp);

    Json::Reader reader;
    if (!reader.parse(json.c_str(), root, false)) {
        printf("[ERROR] \"%s\" file format is not correct.\n", filepath);
        return false;
    }

    return true;
}

int main(const int argc, const char** argv) {

    if (argc < 4) {
        printf("Usage: YServer <ip> <port> <setting/file/path>\n");
        return -1;
    }

    const char* ip = argv[1];
    const char* sport = argv[2];
    const std::string& filepath = argv[3];

    int port = 0;
    if (!GetPort(sport, port)) {
        printf("[ERROR] getting port number failed from \"%s\".\n", sport);
        return -1;
    }

    Json::Value root(Json::arrayValue);
    if (!ReadJson(filepath.c_str(), root)) {
        printf("[ERROR] read json failed from \"%s\" file.\n", filepath.c_str());
        return -1;
    }

    try {
        boost::asio::io_service io_service;
        // const Tcp::endpoint endpoint(Tcp::v4(), port);
        const Tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip), port);
        LikeServer server(root, io_service, endpoint);
        printf("[INFO] run server.\n", filepath.c_str());
        io_service.run();
    } catch (std::exception& e) {
        printf("[EXCEPTION] %s\n", e.what());
    }

    return 0;
}