#include "menus.hpp"
#include "functionality.hpp"
#include <iostream>



void acs2_menu_art() {
    std::cout << "                 ___    ___                                                   \n";
    std::cout << "                |   \\  |  |   ________   ______    _______    ___   __       \n";
    std::cout << "                |    \\ |  | /  ______| /   ____\\  /   __   \\  |  \\  | |   \n";
    std::cout << "                |  \\  \\|  | \\_______   |   /     |   |__|   | | \\ \\ | |  \n";
    std::cout << "                |  |\\     |   ______ \\ |   \\____ |  |____|  | | |\\ \\| |  \n";
    std::cout << "                |__| \\____| \\________| \\_______/ |__|    |  | |_| \\___|   \n";
    std::cout << "-----------------------------------------------------------------------------------------------------\n";
    std::cout << "-------------------------------Network Scanning Tool By Vulcan Security------------------------------\n";
    std::cout << "-----------------------------------------------------------------------------------------------------\n";
}

void hostDescoveryMenu() {
    clear_screen();
    acs2_menu_art();

    int choice;

    std::cout << "\n=== Host Discovery ===\n";
    std::cout << "1. TCP Ping Sweep\n";
    std::cout << "2. Arp Scan\n";
    std::cout << "3. Back\n";
    std::cout << "Choose (1-4): ";

    std::cin >> choice;

    switch(choice) {
        case 1:
            tcp_scan();
            break;
        case 2:
            arpHostDescovery();
            break;
        case 3:
            main_menu();
            break;
        default:
            std::cout << "Invalid choice! Try again.\n";
            hostDescoveryMenu();
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
    std::cout << "3. Packet Sniffer\n";
    std::cout << "4. Exit\n";
    std::cout << "Choose option (1-4): ";

    std::cin >> choice;

    switch(choice) {
        case 1:
            hostDescoveryMenu();
            break;
        case 2:
            tcp_port_scanner(ip);
            break;
        case 3:
            std::cout << "Coming Soon...\n";
            main_menu();
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
