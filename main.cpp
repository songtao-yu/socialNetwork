#include "graphpeople.h"
#include <iostream>
using namespace std;

int main()
{
    GraphPeople gp("SocialNetwork.txt");
    gp.initializeFirendList();
    cout<<gp.getPeopleNumber()<<endl;
    return 0;
}
