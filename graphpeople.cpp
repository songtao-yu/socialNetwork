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
        if (m_nameDictionary.empty())
        {
            lastFirstPerson = firstPerson;
        }

        // construct name list
        if (m_nameDictionary.find(firstPerson) == m_nameDictionary.end())
        {
            int size = m_nameDictionary.size();
            m_nameDictionary[firstPerson] = size;
        }
        if (m_nameDictionary.find(secondPerson) == m_nameDictionary.end())
        {
            int size = m_nameDictionary.size();
            m_nameDictionary[secondPerson] = size;
        }

        // construct freind list
        if (firstPerson == lastFirstPerson)
        {
            friends.insert(m_nameDictionary[secondPerson]);
        }
        else
        {
            m_friendMatrix[m_nameDictionary[lastFirstPerson]] = friends;
            friends.clear();
            friends.insert(m_nameDictionary[secondPerson]);
            lastFirstPerson = firstPerson;
        }
    }

    // for the last person in the file, we need add it to the friend list
    m_friendMatrix[m_nameDictionary[firstPerson]] = friends;

    // check the content of input file
    if (m_nameDictionary.empty())
    {
        cout<<"No person pairs were found, please check the input file"<<endl;
    }
    else
    {
        m_isInitialized = true;
    }

    //dump the whole graph
#if 0
    std::map< int,std::set<int> >::iterator iter = m_friendMatrix.begin();
    while(iter != m_friendMatrix.end())
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
    return m_nameDictionary.size();
}


int GraphPeople::getFriendDistance(const std::string& startPerson, const std::string& endPerson)
{
    if (m_nameDictionary.find(startPerson) == m_nameDictionary.end() || m_nameDictionary.find(endPerson) == m_nameDictionary.end())
    {
        cout<< "User can't be found, please have a check"<<endl;
        return -1;
    }

    int startNum = m_nameDictionary[startPerson];
    int endNum = m_nameDictionary[endPerson];
    map<int,bool> includedSet;
    map<int, int> distanceSet;

    // distance of start person himself/herself is always 0
    includedSet[startNum] = false;
    distanceSet[startNum] = 0;

    // traverse the whole graph from the start person
    size_t counter =0;
    while(counter<m_nameDictionary.size())
    {
        //firstly, find the person with the minimal distance in the included set, from the value which is NOT included
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
        // add the minimal distance vertex to the included set
        includedSet[currentPersonNUm] = true;
        counter++;
#if 0
        cout<<"current is: "<<counter<<endl;
#endif

        // break out if we have included the end person into the set
        if (includedSet.find(endNum)!=includedSet.end() && includedSet[endNum]==true)
        {
            break;
        }

        // try to add it adjacents vertices to the include set
        for (set<int>::iterator iter=m_friendMatrix[currentPersonNUm].begin(); iter!=m_friendMatrix[currentPersonNUm].end(); ++iter)
        {
            // if the adjacent vertex has been in included set, bypass it
            if (includedSet.find(*iter)!=includedSet.end() && includedSet.find(*iter)->second == true)
            {
                continue;
            }

            // if distance of the adjacent vertex is less than distance of current vertex plus 1, bypass it
            if (distanceSet.find(*iter)!=distanceSet.end() && (distanceSet.find(*iter)->second<=distanceSet[currentPersonNUm]+1))
            {
                continue;
            }
            includedSet[*iter] = false;
            distanceSet[*iter]=distanceSet[currentPersonNUm]+1;
        }
    }

    return distanceSet[endNum];
}
