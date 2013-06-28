#include "util.h"

Util::Util()
{
}

string Util::get_parent_path(string abosolute_path){

    unsigned found = abosolute_path.find_last_of("/\\");
    string s=abosolute_path.substr(0,found);
    return s;
}

string Util::get_filename_from_path(string abosolute_path){

    unsigned found = abosolute_path.find_last_of("/\\");
    return abosolute_path.substr(found+1);
}
/**
 * @brief InotifyHandler::get_relative_path
 * get the relative path from root dir
 * @param abosolute_path
 * abosolute path
 * @return
 * relative path
 */
string Util::get_relative_path(string abosolute_path){
    string root_dir=get_root_path();
    unsigned pos = root_dir.length();
    std::string temp = abosolute_path.substr (pos);
    //  cout << "abs "+ abosolute_path<< endl;
    //  cout << "relat" + temp << endl;
    return temp+"/";
}
/**
 * @brief Util::get_root_path
 * @return
 */
string Util::get_root_path(){
// to do , read from external file
    string root_dir="/home/satthy/Client";
    return   root_dir;
}

size_t Util::get_file_size(std::string abosolute_path){
    if(boost::filesystem3::exists(abosolute_path)){
        return boost::filesystem3::file_size(abosolute_path);
    }
    return  -1;

}


size_t Util::get_dir_size(std::string abosolute_path){


    if(boost::filesystem3::exists(abosolute_path)){
        size_t size;
        boost::filesystem::recursive_directory_iterator itr(abosolute_path);

        while (itr != boost::filesystem::recursive_directory_iterator())
        {
            if(is_regular_file(itr->path())){

                size+=boost::filesystem3::file_size(itr->path());

            }
            ++itr;

        }
        return size;
    }
    return -1;
}

