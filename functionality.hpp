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
extern std::array<int, 11> commonPorts;
extern std::vector<int> livePorts;
extern std::vector<int> deadPorts;
extern std::vector<string> liveIP;
extern std::vector<string> deadIP;
extern std::vector<string> gateway;

// Functions
void clear_screen();
bool isValidIPv4(const std::string& ip);
bool isPortOpen(const char* ip, int port);
void tcp_port_scanner(const std::string& ip);
void arpHostDescovery();
void tcpAndARP();
void mapNetwork();
void show_progress(int current, int total, const std::string& message);
void tcpHostDescovery();
void tcp_port_scanner();


#endif // FUNCTIONALITY_HPP