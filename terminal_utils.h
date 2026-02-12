#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
#include <fcntl.h>

using namespace std;

void detect_malware(std::vector<std::string> args);
void check_file_locks(std::vector<std::string> args);
void change_directory(vector<string> args);
void handleWildCards(string& path,vector<string>&args);

#endif // KERNEL_UTILS_H