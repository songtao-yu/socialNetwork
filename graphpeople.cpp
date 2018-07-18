#include <iostream>
#include <fstream>
#include <unistd.h>
#include "graphpeople.h"
using namespace std;

GraphPeople::GraphPeople(const string& fileName):m_fileName(fileName), m_isInitialized(false)
{
    // check if the file exists. If not, exit APP with error code
    if (access(m_fileName.c_str(), F_OK))
    {
        cout<<"file doesn't exit, please have a check"<<endl;
        exit(-1);
    }
}

GraphPeople::~GraphPeople()
{
}

void GraphPeople::initializeFirendList()
{
    string lastFirstPerson;
    string firstPerson;
    string secondPerson;
    set<int> friends;
    ifstream fileIn(m_fileName.c_str(), ios::in);
    string str;
    while(getline(fileIn, str))
    {
        size_t pos = str.find(',');
        if (pos != string::npos)
        {
            firstPerson = str.substr(0, pos);
            secondPerson = str.substr(pos+1);
        }
        else
        {
            continue;
        }

        // at the first beginning, we set the last one and the current one be same
        if (m_nameList.empty())
        {
            lastFirstPerson = firstPerson;
        }

        // construct name list
        if (m_nameList.find(firstPerson) == m_nameList.end())
        {
            int size = m_nameList.size();
            m_nameList[firstPerson] = size;
        }
        if (m_nameList.find(secondPerson) == m_nameList.end())
        {
            int size = m_nameList.size();
            m_nameList[secondPerson] = size;
        }

        // construct freind list
        if (firstPerson == lastFirstPerson)
        {
            friends.insert(m_nameList[secondPerson]);
        }
        else
        {
            m_friendList[m_nameList[lastFirstPerson]] = friends;
            friends.clear();
            friends.insert(m_nameList[secondPerson]);
            lastFirstPerson = firstPerson;
        }
    }

    // for the last person in the file, we need add it to the friend list
    m_friendList[m_nameList[firstPerson]] = friends;

    // check the content of input file
    if (m_nameList.empty())
    {
        cout<<"No person pairs were found, please check the input file"<<endl;
    }
    else
    {
        m_isInitialized = true;
    }

    //dump the whole graph
#if 0
    std::map< int,std::set<int> >::iterator iter = m_friendList.begin();
    while(iter != m_friendList.end())
    {
        cout<<iter->first<<endl;
        std::set<int>::iterator set_iter = iter->second.begin();
        while(set_iter != iter->second.end())
        {
            cout<<*set_iter<<" ";
            set_iter++;
        }
        iter++;
        cout<<endl<<endl;
    }
#endif
}

int GraphPeople::getPeopleNumber()
{
    if (!m_isInitialized)
    {
        cout<<"Initialization has NOT been run or initialization error"<<endl;
        return -1;
    }

    return m_nameList.size();
}


int GraphPeople::getFriendDistance(const std::string& startPerson, const std::string& endPerson)
{
    if (m_nameList.find(startPerson) == m_nameList.end() || m_nameList.find(endPerson) == m_nameList.end())
    {
        cout<< "User can't be found, please have a check"<<endl;
        return -1;
    }

    int startNum = m_nameList[startPerson];
    int endNum = m_nameList[endPerson];
    map<int,bool> includedSet;
    map<int, int> distanceSet;

    // distance of start person himself/herself is always 0
    includedSet[startNum] = false;
    distanceSet[startNum] = 0;

    // run untile we reach the end person
    while(includedSet.find(endNum) != includedSet.end() && includedSet.find(endNum)->second == true)
    {
        //firstly, find the person with the minimum distance in the included set, from the value which is NOT included
        int min = INT_MAX;
        int currentPersonNUm = 0;
        for (map<int,bool>::iterator iter= includedSet.begin(); iter!=includedSet.end(); ++iter)
        {
            if (iter->second == false && distanceSet[iter->first] <=min)
            {
                min= distanceSet[iter->first];
                currentPersonNUm = iter->first;
            }
        }
        // add it to the included set
        includedSet[currentPersonNUm] = true;
        for (set<int>::iterator iter=m_friendList[currentPersonNUm].begin(); iter!=m_friendList[currentPersonNUm].end(); ++iter)
        {
            if (includedSet.find(*iter)!=includedSet.end() && includedSet.find(*iter)->second == true)
            {
                continue;
            }
            includedSet[*iter] = false;
            distanceSet[*iter]=distanceSet[currentPersonNUm]+1;
        }
    }

    return distanceSet[endNum];
}
