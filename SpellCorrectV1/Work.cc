#include "Work.h"
#include "./templateMylogger/Mylogger.h"
#include <fstream>
#include <sstream>

using std::ifstream;
using std::istringstream;

int min_of_three(int a,int b,int c)
{
    int tem = a<b?a:b;
    return tem<c?tem:c;
}        
int minEditDistance(const string& word1,const string& word2)
{
    int n = word1.length();
    int m = word2.length();
    if(n * m == 0)
        return n+m;
    vector<vector<int>> dpArray(n+1,vector<int>(m+1));
    //init dpArray 
    for(int i=0;i<n+1;++i)
        dpArray[i][0] = i;
    for(int j=0;j<m+1;++j)
        dpArray[0][j] = j;
    //dp compute
    for(int i=1;i<n+1;++i)
        for(int j=1;j<m+1;++j)
        {
            if(word1[i-1] == word2[j-1])
                dpArray[i][j] = dpArray[i-1][j-1];
            else dpArray[i][j] = 1+min_of_three(dpArray[i-1][j],dpArray[i][j-1],dpArray[i-1][j-1]);
        }
    return dpArray[n][m];
}        

MyWork::MyWork()
{
    _index.resize(26);
}

void MyWork::load_dict()
{
    ifstream ifs(_dictPath);
    if(!ifs)
    {
        LogError("ifstream open file %s error!",_dictPath.c_str());
        return;
    }

    pair<string,int> wordpair;
    string line;
    while(getline(ifs,line),!ifs.eof())
    {
        istringstream iss(line);
        iss>>wordpair.first;
        iss>>wordpair.second;
        _enDict.push_back(wordpair);
        //wordpair.first.clear();
    }
}

void MyWork::create_index()
{
    for(size_t idx=0;idx<_enDict.size();++idx)
    {
        for(auto alpha:_enDict[idx].first)
        {
            _index[alpha-97].insert(idx);
        }
    }
}

vector<singleResult> MyWork::handle_request(const string & request)
{
    vector<bool> index_flags(_index.size(),false);
    vector<bool> endict_flags(_enDict.size(),false);
    priority_queue<singleResult> resultQue;
    for(auto &x:request)
    {
        if(index_flags[x-97]==false)
        {
            for(auto &y:_index[x-97])
            {
                if(endict_flags[y] == false)
                {
                    int ret = minEditDistance(request,_enDict[y].first);
                    resultQue.push(singleResult(_enDict[y].first,_enDict[y].second,ret));
                    if(resultQue.size()>3)//返回的结果个数
                        resultQue.pop();
                    endict_flags[y] = true;
                }
            }
            index_flags[x-97] = true;
        }
    }
    vector<singleResult> myresult;
    while(!resultQue.empty())
    {
        myresult.push_back(resultQue.top());
        resultQue.pop();
    }
    return myresult;
}
