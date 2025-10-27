#include "menus.hpp"
#include "functionality.hpp"
#include <iostream>

void acs2_menu_art() {
    std::cout << " ___    __                                                 _____        _____                  \n";
    std::cout << "|   \\  |  |   ________   ______    _______    ___   __    |_   _|      /  __ \\ _     _       \n";
    std::cout << "|    \\ |  | /  ______| /   ____\\  /   __   \\  |  \\  | |     | | _ __   | /  \\/| |_ _| |_    \n";
    std::cout << "|  \\  \\|  | \\_______   |   /     |   |__|   | | \\ \\ | |     | || '_ \\  | |  |_   _|_   _|   \n";
    std::cout << "|  |\\     |   ______ \\ |   \\____ |  |____|  | | |\\ \\| |    _| || | | | | \\__/\\|_|   |_|  \n";
    std::cout << "|__| \\____| \\________| \\_______/ |__|    |  | |_| \\___|    \\___/_| |_|  \\____/            \n";
    std::cout << "-----------------------------------------------------------------------------------------------------\n";
    std::cout << "-------------------------------Network Scanning Tool By Kanax01 In C++-------------------------------\n";
    std::cout << "-----------------------------------------------------------------------------------------------------\n";
}

void hostDescoveryMenu() {
    clear_screen();
    acs2_menu_art();

    int choice;
    
    std::cout << "\n=== Host Discovery ===\n";
    std::cout << "1. TCP Ping Sweep\n";
    std::cout << "2. Arp Scan\n";
    std::cout << "3. Both\n";
    std::cout << "4. Back\n";
    std::cout << "Choose (1-4): ";

    std::cin >> choice;

    switch(choice) {
        case 1:
            tcp_scan();
            break;
        case 2:
            arp_network_discovery();
            break;
        case 3:
            tcpAndARP();
            break;
        case 4:
            main_menu();
            break;
        default:
            std::cout << "Invalid choice! Try again.\n";
            hostDescoveryMenu();
            break;
    }
}

void portScanningMenu() {
    clear_screen();
    acs2_menu_art();

    int choice;
    std::string ip;

    std::cout << "\n=== Port Scanner ===\n";
    std::cout << "Enter target IP: ";
    std::cin >> ip;

    std::cout << "1. Fast Scan\n";
    std::cout << "2. Custom Port Range\n";
    std::cout << "3. Custom Port # Scan From File\n";  // Not implemented yet
    std::cout << "4. Full Port Scan\n";
    std::cout << "5. Back\n";
    std::cout << "Pick (1-5): ";
    std::cin >> choice;

    switch(choice) {
        case 1:
        case 2:
        case 4:
            tcp_port_scanner(ip);
            break;
        case 3:
            std::cout << "Not Implemented Yet\n";
            portScanningMenu();
            break;
        case 5:
            main_menu();
            break;
        default:
            std::cout << "Invalid choice!\n";
            portScanningMenu();
            break;
    }
}

void main_menu() {
    clear_screen();
    acs2_menu_art();

    int choice;

    std::cout << "\n=== Welcome ===\n";
    std::cout << "1. Host Discovery\n";
    std::cout << "2. Port Scanning\n";
    std::cout << "3. Map Network\n";
    std::cout << "4. Exit\n";
    std::cout << "Choose option (1-4): ";
        
    std::cin >> choice;
        
    switch(choice) {
        case 1:
            hostDescoveryMenu();
            break;
        case 2:
            portScanningMenu();
            break;
        case 3:
            mapNetwork();
            break;
        case 4:
            std::cout << "Exiting...\n";
            return;
        default:
            std::cout << "Invalid choice! Try again.\n";
            main_menu();
            break;
    }
}