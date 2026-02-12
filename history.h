#ifndef HISTORY_H
#define HISTORY_H

#include <deque>
#include <string>

extern int current_index;
extern std::deque<std::string> his;

void save_to_history(std::string& command);
int getch();
void get_history(std::string& command);

#endif // HISTORY_H