#include "Dict.h"
#include "../templateMylogger/Mylogger.h"
#include <fstream>
MyDict::MyDict(const string &dictsrcpath,const string &dictobjpath)
{
    _dictsrcPath = dictsrcpath;
    _dictobjPath = dictobjpath;
}

void MyDict::initial_dict()
{
    load_dict();
    LogInfo("loading dict complete!");
    store_dict();
    LogInfo("storing dict complete!");
}

void MyDict::load_dict()
{
    string word;
    std::ifstream ifs(_dictsrcPath);
    while(ifs >> word,!ifs.eof())
    {
        if(isValidword(word))
            _enDict[word]++;
    }
    ifs.close();
}

void MyDict::store_dict()
{
    std::ofstream fout(_dictobjPath);
    for(auto x:_enDict)
        fout<<x.first<<" "<<x.second<<std::endl;
}

bool MyDict::isValidword(string &word)
{
    for(auto &x:word)
    {
        if(!isalpha(x))
            return false;
        else if(x>64&&x<91)
            x=x+32;
    }
    if(word.size() < 3)
        return false;
    return true;
}
