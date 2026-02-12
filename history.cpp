#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <deque>
#include "history.h"

int current_index = 0;
std::deque<std::string> his; // For storing the history
#define MAXLEN 100 // Maximum length of deque

void save_to_history(std::string& command) {
    if (!command.empty()) {
        his.push_front(command);
        if (his.size() > MAXLEN) his.pop_back();
    }
}

int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void get_history(std::string &command) {
    int ch = getch();
    if (ch == '[') {
        ch = getch();
        if (ch == 'A') { // Up Arrow
            if (current_index < (int)his.size()) { // Corrected bounds check
                current_index++;
                std::cout << "\33[2K\rChinmai's Terminal:~ " << his[current_index - 1]; // Corrected index and added prompt
                std::cout.flush();
                command = his[current_index - 1]; // Corrected index
            }
        } else if (ch == 'B') { // Down Arrow
            if (current_index > 1) { // Corrected bounds check
                current_index--;
                std::cout << "\33[2K\rChinmai's Terminal:~ " << his[current_index - 1]; // Corrected index and added prompt
                std::cout.flush();
                command = his[current_index - 1]; // Corrected index
            } else if (current_index == 1) { // Case for going back to an empty command
                current_index--;
                std::cout << "\33[2K\rChinmai's Terminal:~ ";
                std::cout.flush();
                command = "";
            }
        }
    }
}