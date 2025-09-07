#include <iostream>
#include <ctime>
#include <iomanip>
#include <thread>    // For std::this_thread
#include <chrono>    // For std::chrono::seconds / milliseconds
#include <csignal>
#include <cstdlib>
#include <sstream>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// Global flag for graceful shutdown
volatile bool running = true;

// Signal handler for Ctrl+C
void signalHandler(int signal) {
    if (signal == SIGINT) {
        running = false;
        std::cout << "\n\nDigital Clock terminated gracefully.\n" << std::endl;
    }
}

// Cross-platform function to clear the console screen
void clearScreen() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) return;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;

    DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD coordScreen = {0, 0};
    DWORD charsWritten;

    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, coordScreen, &charsWritten);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, consoleSize, coordScreen, &charsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
#else
    std::cout << "\033[2J\033[1;1H";
    std::cout.flush();
#endif
}

// Get current time as string HH:MM:SS
std::string getCurrentTime() {
    std::time_t rawTime = std::time(nullptr);
    if (rawTime == -1) return "ERROR: Unable to get system time";

    std::tm* localTime = std::localtime(&rawTime);
    if (localTime == nullptr) return "ERROR: Unable to convert time";

    std::ostringstream timeStream;
    timeStream << std::setfill('0') 
               << std::setw(2) << localTime->tm_hour << ":"
               << std::setw(2) << localTime->tm_min << ":"
               << std::setw(2) << localTime->tm_sec;
    return timeStream.str();
}

// Get current date as string Day, Month DD, YYYY
std::string getCurrentDate() {
    std::time_t rawTime = std::time(nullptr);
    if (rawTime == -1) return "ERROR: Unable to get system date";

    std::tm* localTime = std::localtime(&rawTime);
    if (localTime == nullptr) return "ERROR: Unable to convert date";

    const char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    const char* months[] = {"January", "February", "March", "April", "May", "June",
                           "July", "August", "September", "October", "November", "December"};

    std::ostringstream dateStream;
    dateStream << days[localTime->tm_wday] << ", "
               << months[localTime->tm_mon] << " "
               << localTime->tm_mday << ", "
               << (localTime->tm_year + 1900);
    return dateStream.str();
}



int main() {
    // Setup Ctrl+C signal handler
    std::signal(SIGINT, signalHandler);

    std::cout << "Starting Digital Clock Application...\n";
    std::cout << "Press Ctrl+C to exit.\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Main loop
    while (running) {
        try {
            clearScreen();
            displayClock();

            // Sleep in small chunks for responsive exit
            for (int i = 0; i < 10 && running; ++i) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    clearScreen();
    std::cout << "Digital Clock Application terminated.\n";
    std::cout << "Thank you for using Digital Clock!\n" << std::endl;
    return 0;
}
