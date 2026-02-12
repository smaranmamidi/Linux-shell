#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<cstring>
#include<vector>
#include<fcntl.h>
#include<glob.h>
#include "terminal_utils.h"
#include<map>
using namespace std;

// A hardcoded list of known malicious process names.
std::map<std::string, bool> malicious_processes = {
    {"malware.exe", true},
    {"trojan.sh", true},
    {"virus", true}
};

void detect_malware(std::vector<std::string> args) {
    if (args.size() < 2) {
        std::cout << "Usage: sb <process_name> -suggest" << std::endl;
        return;
    }
    
    std::string process_name = args[1];

    if (malicious_processes.count(process_name)) {
        std::cout << "WARNING: " << process_name << " is a known malicious process." << std::endl;
    } else {
        std::cout << "Process " << process_name << " is not on the known malware list." << std::endl;
    }

    if (args.size() > 2 && args[2] == "-suggest") {
        std::cout << "Suggestion: Consider terminating this process if you don't recognize it." << std::endl;
    }
}

void check_file_locks(std::vector<std::string> args) {
    if (args.size() < 2) {
        std::cout << "Usage: delep <file_path>" << std::endl;
        return;
    }

    std::string file_path = args[1];
    int fd = open(file_path.c_str(), O_RDWR);

    if (fd < 0) {
        perror("Error opening file");
        return;
    }

    struct flock fl;
    fl.l_type = F_WRLCK; // Check for a write lock
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0; // Lock the entire file

    if (fcntl(fd, F_GETLK, &fl) == -1) {
        perror("fcntl failed");
        close(fd);
        return;
    }

    if (fl.l_type == F_UNLCK) {
        std::cout << "File " << file_path << " is not locked." << std::endl;
    } else {
        std::cout << "File " << file_path << " is locked by process ID: " << fl.l_pid << std::endl;
    }

    close(fd);
}

void change_directory(vector<string> args){
    if(args.size()<2){cout<<"Missing Argument"<<endl;}
    else if(chdir(args[1].c_str()) !=0){
        cout<<"Failed Directory Change"<<endl;
    }
}

void handleWildCards(string& path, vector<string> &args){
    bool status = true;
    for(auto x:path) if(x=='*' || x=='?') status = false;
    if(status){
        args.push_back(path);
        return;
    }
    cout<<"Handling path"<<endl;
    glob_t glob_result;
    glob(path.c_str(),GLOB_NOCHECK,NULL,&glob_result);
    for(size_t i=0;i<glob_result.gl_pathc;i++){
        args.push_back(glob_result.gl_pathv[i]);
    }
    globfree(&glob_result);
}