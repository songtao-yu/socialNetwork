#ifndef GRAPH_PEOPLE_H
#define GRAPH_PEOPLE_H
#include <map>
#include <string>
#include<set>

class GraphPeople
{
public:
    GraphPeople(const std::string& fileName);
    ~GraphPeople();
    void initializeFirendList();
    int getPeopleNumber();
    int getFriendDistance(const std::string& startPerson, const std::string& endPerson);
private:
    //disable copy constructor and assignment constructor
    GraphPeople(const GraphPeople& rhs);
    GraphPeople& operator=(const GraphPeople& rhs);
private:
    std::string m_fileName;
    std::map<std::string, int> m_nameDictionary; //key = person name, value = the associate number
    std::map< int,std::set<int> > m_friendMatrix; // key = associate number of person name, value = set of his/her direct friends
    bool m_isInitialized;
};
#endif
