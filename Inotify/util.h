#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <boost/filesystem.hpp>
using namespace std;
class Util
{
public:
    Util();
    static string get_relative_path(string abosolute_path);
    static string get_root_path();
    static size_t get_file_size(string abosolute_path);
    static size_t get_dir_size(string abosolute_path);
    static string get_parent_path(string abosolute_path);
    static string get_filename_from_path(string abosolute_path);
};

#endif // UTIL_H
