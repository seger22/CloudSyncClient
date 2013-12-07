#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H
#include<iostream>
#include<string>
#include <stdlib.h> // to get path of home dir

using namespace std;

class ConfigurationManager
{
    
public:
    ConfigurationManager();
    static string getValue();
    static string getLocation();
    static int getBlockSize();   
};

#endif // CONFIGURATIONMANAGER_H
