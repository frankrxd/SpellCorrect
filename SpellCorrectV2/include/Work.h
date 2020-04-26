#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <iostream>
#include <queue>

using std::vector;
using std::string;
using std::unordered_set;
using std::pair;
using std::priority_queue;

struct singleResult
{
public:
    singleResult(const string &word,int freq,int editDistance)
        :_word(word)
         ,_freq(freq)
         ,_editDistance(editDistance)
    {}
    string _word;
    int _freq;
    int _editDistance;

    bool operator<(const singleResult &b) const
    {
        if(_editDistance == b._editDistance)
            return _freq>b._freq;
        else return _editDistance<b._editDistance;
    }
};

class MyWork
{
public:
    MyWork();
    ~MyWork() {}

    void init()
    {
        load_dict();
        create_index();
    }
    void show()
    {
        for(auto x:_enDict)
        std::cout<<x.first<<" : "<<x.second<<std::endl;
    }
    
    vector<singleResult> handle_request(const string & request);
private:
    void load_dict();
    void create_index();
private:
    string _dictPath = "../dict/dictobject/endict.dat";
    vector<pair<string,int>> _enDict;
    vector<unordered_set<size_t>> _index;
};

