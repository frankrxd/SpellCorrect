#pragma once
#include <string>
#include <map>

using std::string;
using std::map;

class MyDict
{
public:
    MyDict(){
        _dictsrcPath = "./dictsource/english.txt";
        _dictobjPath = "./dictobject/endict.dat";
    }
    MyDict(const string &dictsrcpath,const string &dictobjpath);
    ~MyDict() {}

public:

    void initial_dict();
    void load_dict();
    void store_dict();

private:
    bool isValidword(string &word);
private:
    string _dictsrcPath;
    string _dictobjPath;
    map<string,int> _enDict;
};

