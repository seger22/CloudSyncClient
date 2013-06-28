#include "eventchecker.h"

EventChecker::EventChecker()
{
}

/**
 * @brief TActionComplete::DirCreatedChecker
 *thread function
 *checck whether a directory is completely created(including files and sub dirs)
 * @param path
 *path of the directory
 * @return
 */
void *EventChecker::DirCreatedChecker(std::string path){
    boost::posix_time::seconds Time(4);
    size_t previous_size=-1;
    size_t current_size=-1;

    previous_size=GetDirSize(path);

    while(true){

        boost::this_thread::sleep(Time);
        current_size=GetDirSize(path);
        if(previous_size==current_size){// file completely created
            break;
        }else {
            previous_size=current_size;
        }
    }
    std::cout<< "dir completely created"<< std::endl;
    // call file handler

}

/**
 * @brief TActionComplete::FileCreatedChecker
 *thread function
 *check whether a file is completely created
 * @param path
 * path of the file
 * @return
 */
void *EventChecker::FileCreatedChecker(std::string path){

    boost::posix_time::seconds Time(4);
    size_t previous_size=-1;
    size_t current_size=-1;

    previous_size=GetFileSize(path);

    while(true){

        boost::this_thread::sleep(Time);
        current_size=GetFileSize(path);
        if(previous_size==current_size){// file completely created
            break;
        }else {
            previous_size=current_size;
        }
    }

    // call file handler
    std::cout<< "file completely created"<< std::endl;
}

size_t EventChecker::GetFileSize(std::string path){
    if(boost::filesystem3::exists(path)){
        return boost::filesystem3::file_size(path);
    }
    return  -1;

}


size_t EventChecker::GetDirSize(std::string path){


    if(boost::filesystem3::exists(path)){
        size_t size;
        boost::filesystem::recursive_directory_iterator itr(path);

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



