#include "cmd.h"

CMD::CMD()
{
}
/**
 * @brief CMD::get_inode_number
 * if success return the inode number as string else return  NULL
 * @param filepath
 * @return
 */
string CMD::get_inode_number(string filepath){
    string s="";
    struct stat file_stats;
    if(stat(filepath.c_str(),&file_stats)!=-1){
    s = boost::lexical_cast<string>( file_stats.st_ino );
    //cout<< s<< endl;
    }
    return s;
}


string CMD::get_path_from_inode(string inode){
        FILE *in;
        char buff[512];
        string command="find /home -inum ";
        command+=inode;
    //find path -inum 2635617
    //ls -il filename
//find /home -inum 1845319
        //chmod 551 /home/jena/sync
        if(!(in = popen(command.c_str(), "r"))){
            cout <<"error"<<endl;//return 1;
        }

        while(fgets(buff, sizeof(buff), in)!=NULL){
            //cout << buff;
        }
        pclose(in);
        std::string str=buff;
        str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
        return str;
}

int CMD::lock_folder(string abosolute_path){
    FILE *in;

    string command="chmod 551 ";
    command+=abosolute_path;
    if(!(in = popen(command.c_str(), "r"))){
        cout <<"error"<<endl;
        return -1;
    }
    return 1;
}

int CMD::release_folder(string abosolute_path){
    FILE *in;

    string command="chmod 771 ";
    command+=abosolute_path;
    if(!(in = popen(command.c_str(), "r"))){
        cout <<"error"<<endl;
        return -1;
    }
    return 1;

}
