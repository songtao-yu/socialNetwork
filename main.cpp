#include "graphpeople.h"
#include <iostream>
using namespace std;

int main()
{
    GraphPeople gp("SocialNetwork.txt");
    gp.initializeFirendList();
    cout<<"Total number of people is: "<<gp.getPeopleNumber()<<endl;
    string startPerson="STACEY_STRIMPLE";
    string endPerson="RICH_OMLI" ;
    cout<<"The distance between "<<startPerson<<" and "<<endPerson<<" is: "<<gp.getFriendDistance(startPerson, endPerson);
   // cout<<gp.getFriendDistance("MYLES_JEFFCOAT", "CHAS_VANSCOIK");
    return 0;
}
