#include<iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>
#include <util.h>

#ifndef EVENTCHECKER_H
#define EVENTCHECKER_H
using namespace std;

class EventChecker
{
public:
    EventChecker();
    static void* DirCreatedChecker(std::string path);
    static void* FileCreatedChecker(std ::string path);

private:
    static size_t GetFileSize(std::string path);
    static size_t GetDirSize(std::string path);
    string get_relative_path(string abosolute_path);

};

#endif // EVENTCHECKER_H
