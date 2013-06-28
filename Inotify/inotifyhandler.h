
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
using namespace  std;

class InotifyHandler
{
public:
    InotifyHandler();
    void initialize_inotify();

private:
    Inotify notify;
    Filehandler fh;
    string root_dir;

  void add_watch(string path);
    void recursively_add_watch(string dir);
    void wait_for_events();

      void dir_created_checker(string inode);// thread
      void file_created_checker(string path);

      void dir_created_checker1(string path);
      void file_created_checker1(string path);

};

#endif // INOTIFYHANDLER_H
