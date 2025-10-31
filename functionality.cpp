#include "functionality.hpp"
#include "menus.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <thread>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <csignal>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Initialize global variables
std::array<int, 11> commonPorts = {20,21,22,23,25,53,80,110,143,443,3389};
std::vector<int> livePorts;
std::vector<int> deadPorts;

//----------------------------------------------------------------------------------------
//-----------------------------------Core Functionality-----------------------------------
//----------------------------------------------------------------------------------------

void clear_screen() {
    std::cout << "\033[2J\033[1;1H"; // ANSI escape codes to clear screen
}

//Progress Bar Function
void show_progress(int current, int total, const std::string& message) {
    float percentage = (float)current / total * 100;
    std::cout << "\r" << message << ": " << current << "/" << total 
              << " (" << std::fixed << std::setprecision(1) << percentage << "%)";
    std::cout.flush();
}

// IP Validator
bool isValidIPv4(const std::string& ip) {
    std::regex ipv4_regex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    return std::regex_match(ip, ipv4_regex);
}

// Port checking function
bool isPortOpen(const char* ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_pton(AF_INET, ip, &target.sin_addr);

    bool isOpen = (connect(sock, (struct sockaddr*)&target, sizeof(target)) == 0);
    close(sock);
    return isOpen;
}


//---------------------------------------------------------------------

// TCP Helper Functions

// Check if host is alive using TCP
bool isHostAlive(const std::string& ip) {
    std::vector<int> testPorts = {22, 80, 443, 135, 445}; // Common ports

    for (int port : testPorts) {
        if (isPortOpen(ip.c_str(), port)) {
            return true;
        }
    }
    return false;
}

// Get our own IP address
std::string getOurIP() {
    std::string command = "hostname -I | awk '{print $1}'";
    FILE* pipe = popen(command.c_str(), "r");
    char buffer[128];
    std::string result = "Unknown";

    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result = buffer;
        result.erase(result.find_last_not_of(" \n\r\t") + 1);
    }
    pclose(pipe);
    return result;
}

// Get gateway IP
std::string getGatewayIP() {
    std::string command = "ip route | grep default | awk '{print $3}'";
    FILE* pipe = popen(command.c_str(), "r");
    char buffer[128];
    std::string result = "Unknown";

    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result = buffer;
        result.erase(result.find_last_not_of(" \n\r\t") + 1);
    }
    pclose(pipe);
    return result;
}

// Identify device type based on open ports
std::string identifyDevice(const std::string& ip) {
    if (isPortOpen(ip.c_str(), 22)) return "Linux/Server (SSH)";
    if (isPortOpen(ip.c_str(), 3389)) return "Windows (RDP)";
    if (isPortOpen(ip.c_str(), 445)) return "Windows (SMB)";
    if (isPortOpen(ip.c_str(), 80) || isPortOpen(ip.c_str(), 443)) return "Web Server";
    return "Unknown Device";
}

// Get hostname
std::string getHostname(const std::string& ip) {
    std::string command = "nslookup " + ip + " 2>/dev/null | grep 'name =' | awk '{print $4}'";
    FILE* pipe = popen(command.c_str(), "r");
    char buffer[128];
    std::string result = "Unknown";

    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result = buffer;
        result.erase(result.find_last_not_of(" \n\r\t") + 1);
        if (result.back() == '.') result.pop_back();
    }
    pclose(pipe);
    return result;
}

// Scan common services
void scanCommonServices(const std::string& ip) {
    std::vector<std::pair<int, std::string>> services = {
        {21, "FTP"}, {22, "SSH"}, {23, "Telnet"}, {25, "SMTP"},
        {53, "DNS"}, {80, "HTTP"}, {110, "POP3"}, {143, "IMAP"},
        {443, "HTTPS"}, {445, "SMB"}, {3389, "RDP"}
    };

    for (auto& service : services) {
        if (isPortOpen(ip.c_str(), service.first)) {
            std::cout << "    - Port " << service.first << " (" << service.second << ")\n";
        }
    }
}



//<----------------TCP IP Scanner Menu--------------------->
void tcpHostDescovery() {
    std::string start;
    std::string network;

    std::cout << "\n=== TCP Host Discovery ===\n";
    std::cout << "Enter Network (e.g., 192.168.1): ";
    std::cin >> network;

    std::cout << "Start? (y/n): ";
    std::cin >> start;
    if (start != "y") {
        if (start != "n") {
            std::cout << "Invalid Choice...\n";
            tcpHostDescovery();
        }
        std::cout << "Going Back...\n";
        hostDescoveryMenu();
        return;
    }

    // Get our own IP and gateway first
    std::string ourIP = getOurIP();
    std::string gateway = getGatewayIP();

    std::cout << "Scanning network " << network << ".0/24...\n";
    std::cout << "Our IP: " << ourIP << "\n";
    std::cout << "Gateway: " << gateway << "\n\n";

    // Scan all IPs in the network
    for (int i = 1; i < 255; i++) {
        std::string targetIP = network + "." + std::to_string(i);

        if (isHostAlive(targetIP)) {
            std::string deviceType = identifyDevice(targetIP);
            std::string hostname = getHostname(targetIP);

            std::cout << "Live Host: " << targetIP;
            if (targetIP == ourIP) std::cout << " [OUR DEVICE]";
            if (targetIP == gateway) std::cout << " [ROUTER]";
            std::cout << "\n";

            std::cout << "  Hostname: " << hostname << "\n";
            std::cout << "  Device Type: " << deviceType << "\n";
            std::cout << "  Open Services:\n";
            scanCommonServices(targetIP);
            std::cout << "-----------------------------------\n";
        }

        show_progress(i, 254, "Scanning");
    }
    std::cout << "\nScan complete!\n";
}


// Arp Helpers

// Read ARP table and return live hosts
std::vector<std::string> readARPTable() {
    std::vector<std::string> liveHosts;
    std::string command = "arp -a | grep -v incomplete";
    FILE* pipe = popen(command.c_str(), "r");

    if (!pipe) return liveHosts;

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        std::string line(buffer);
        // Remove newline and add to results
        line.erase(line.find_last_not_of("\n\r") + 1);
        liveHosts.push_back(line);
    }
    pclose(pipe);
    return liveHosts;
}

// Extract MAC address from ARP entry
std::string extractMACFromARP(const std::string& arpEntry) {
    // ARP entry format: "ip (ip) at mac [interface]"
    size_t atPos = arpEntry.find(" at ");
    if (atPos == std::string::npos) return "Unknown";

    size_t bracketPos = arpEntry.find(" [", atPos);
    if (bracketPos == std::string::npos) return "Unknown";

    return arpEntry.substr(atPos + 4, bracketPos - (atPos + 4));
}


//<------------------ARP Host Descovery--------------------->

void arpHostDescovery() {
    std::string start;
    std::string network;

    std::cout << "\n=== ARP Host Discovery ===\n";
    std::cout << "Enter Network (e.g., 192.168.1): ";
    std::cin >> network;

    std::cout << "Start? (y/n): ";
    std::cin >> start;
    if (start != "y") {
        if (start != "n") {
            std::cout << "Invalid Choice...\n";
            arpHostDescovery();
        }
        std::cout << "Going Back...\n";
        hostDescoveryMenu();
        return;
    }

    // Get own IP and gateway first
    std::string ourIP = getOurIP();
    std::string gateway = getGatewayIP();

    std::cout << "Scanning network " << network << ".0/24 using ARP...\n";
    std::cout << "Our IP: " << ourIP << "\n";
    std::cout << "Gateway: " << gateway << "\n\n";

    // First, ping all hosts to populate ARP table
    std::cout << "Populating ARP table...\n";
    for (int i = 1; i < 255; i++) {
        std::string targetIP = network + "." + std::to_string(i);
        std::string command = "ping -c 1 -W 1 " + targetIP + " > /dev/null 2>&1 &";
        system(command.c_str()); // figure out why compiler errors here
        show_progress(i, 254, "Pinging");
    }
    std::cout << "\n";

    // Wait for pings to complete
    std::cout << "Waiting for responses...\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Read ARP table to find live hosts
    std::cout << "Reading ARP table...\n";
    std::vector<std::string> liveHosts = readARPTable();

    // Display results
    std::cout << "\n=== ARP Scan Results ===\n";
    for (const auto& host : liveHosts) {
        // Parse IP from ARP entry
        std::string ip = host.substr(0, host.find(' '));

        std::cout << "Live Host: " << ip;
        if (ip == ourIP) std::cout << " [OUR DEVICE]";
        if (ip == gateway) std::cout << " [ROUTER]";
        std::cout << "\n";

        // Get additional info
        std::string hostname = getHostname(ip);
        std::string deviceType = identifyDevice(ip);

        std::cout << "  Hostname: " << hostname << "\n";
        std::cout << "  Device Type: " << deviceType << "\n";
        std::cout << "  MAC Address: " << extractMACFromARP(host) << "\n";

        // Scan common services
        std::cout << "  Open Services:\n";
        scanCommonServices(ip);
        std::cout << "-----------------------------------\n";
    }

    std::cout << "Found " << liveHosts.size() << " live hosts\n";
    std::cout << "Press enter to continue...";
    std::cin.ignore();
    std::cin.get();
    hostDescoveryMenu();
}



//<-------------TCP Port Scanner Functionality--------------->
void tcp_port_scanner(const std::string& ip) {
    int portChoice;
    std::string start;

    // Clear previous results
    livePorts.clear();
    deadPorts.clear();

    std::cout << "Select scan type:\n";
    std::cout << "1. Scan Common Ports\n";
    std::cout << "2. Range Scan\n";
    std::cout << "3. Full Port Scan\n";
    std::cout << "4. Back\n";
    std::cout << "Choice: ";
    std::cin >> portChoice;

    if (portChoice < 4 || portChoice > 1) {
        std::cout << "Invalid Choice\n";
    }

    if (portChoice == 1) { // Scan All Common Ports
        std::cout << "Scanning common ports on " << ip << "...\n";

        for (int port : commonPorts) {
            bool isUp = isPortOpen(ip.c_str(), port);
            if (isUp) {
                std::cout << "Port " << port << " is OPEN" << std::endl;
                livePorts.push_back(port);
            } else {
                deadPorts.push_back(port);
            }
        }
    }
    else if (portChoice == 2) { // Range Scan
        int num1, num2;
        std::cout << "Enter First Port Number: ";
        std::cin >> num1;
        std::cout << "Enter Second Port Number: ";
        std::cin >> num2;

        std::cout << "Start? (y/n): ";
        std::cin >> start;
        if (start != "y") {
            return;
        }

        std::cout << "Scanning ports " << num1 << " to " << num2 << " on " << ip << "...\n";
        for (int port = num1; port <= num2; port++) {
            bool isUp = isPortOpen(ip.c_str(), port);
            if (isUp) {
                std::cout << "Port " << port << " is OPEN" << std::endl;
                livePorts.push_back(port);
            } else {
                deadPorts.push_back(port);
            }
            show_progress(port - num1 + 1, num2 - num1 + 1, "Scanning");
        }
        std::cout << "\n";
    }
    else if (portChoice == 3) { // Full Port Scan - All 65,535
        int maxPorts = 65535;
        std::cout << "Starting TCP Port Scan on IP: " << ip << "\n";
        for (int port = 1; port <= maxPorts; port++) {
            bool isUp = isPortOpen(ip.c_str(), port);
            if (isUp) {
                std::cout << "Port " << port << " is OPEN" << std::endl;
                livePorts.push_back(port);
            } else {
                deadPorts.push_back(port);
            }
            if (port % 1000 == 0) {
                show_progress(port, maxPorts, "Scanning");
            }
        }
        std::cout << "\n";
    }

    if (portChoice == 4) {
        std::cout << "Going Back...\n";
        main_menu();
    } 

    // Display Results
    std::cout << "TCP Port Scan Completed.\n";
    std::cout << "Live Ports:" << std::endl;
    for (int port : livePorts) {
        std::cout << "Port " << port << " is open" << std::endl;
    }
    std::cout << "-----------------------------------------------\n";
    std::cout << "Total open ports: " << livePorts.size() << std::endl;
    std::cout << "Total closed ports: " << deadPorts.size() << std::endl;

    std::cout << "Press enter to continue...";
    std::cin.ignore();
    std::cin.get();
    portScanningMenu();
}


void packetSniffer() {
    std::cout << "Coming Soon...\n";
    main_menu();
}
