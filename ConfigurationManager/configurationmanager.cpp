#include "configurationmanager.h"


ConfigurationManager::ConfigurationManager()
{
}

string ConfigurationManager::getLocation(){  
    char* pPath;
     pPath = getenv ("HOME");
     string path(pPath);
     if (pPath!=NULL){
         path = path +"/CloudSync";
     }
    return path;
}

int ConfigurationManager::getBlockSize(){

    return 700;
}




