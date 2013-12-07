
#ifndef INOTIFYHANDLER_H
#define INOTIFYHANDLER_H

#include<iostream>
#include "inotify-cxx.h"
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <filehandler.h>
#include <cmd.h>
#include <util.h>
#include <set>
using namespace  std;

class InotifyHandler
{
public:
    InotifyHandler();
    void initialize_inotify();

private:
    Inotify notify;
    FileHandler fh;
    string root_dir;
    set<string> openedfile;
    set<string> createdfile;
    bool file_deleted;
    bool dir_deleted;
    string moved_from_dir;
    string moved_into_dir;
    string fname_before_move;
    string fname_after_move;

    string dmoved_from_dir="";
    string dmoved_into_dir="";
    string dname_before_move="";
    string dname_after_move="";

    void add_watch(string path);
    void remove_watch(string path);
    void recursively_add_watch(string dir);
    void wait_for_events();

    void dir_created_checker(string inode);// thread
    void file_created_checker(string path);
    void file_deleted_checker(string path,string name);
    void dir_deleted_checker(string path,string name);
    void dir_created_checker1(string path);
    void file_created_checker1(string path);

    bool is_temp_file(string filename);

};

#endif // INOTIFYHANDLER_H
