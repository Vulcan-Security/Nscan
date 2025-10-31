#ifndef FUNCTIONALITY_HPP
#define FUNCTIONALITY_HPP
#include <string>
#include <vector>
#include <array>

struct Device {
    std::string ip;
    bool is_alive;
};

// Global Variables
extern std::string ip;
extern std::array<int, 11> commonPorts;
extern std::vector<int> livePorts;
extern std::vector<int> deadPorts;
extern std::vector<std::string> liveIP;
extern std::vector<std::string> deadIP;
extern std::vector<std::string> gateway;

// Functions
void clear_screen();
void packetSniffer();
bool isValidIPv4(const std::string& ip);
bool isPortOpen(const char* ip, int port);
void tcp_port_scanner(const std::string& ip);
void show_progress(int current, int total, const std::string& message);
void tcpHostDescovery();
void arpHostDescovery();
std::string getOurIP();
std::string getGatewayIP();

#endif // FUNCTIONALITY_HPP
