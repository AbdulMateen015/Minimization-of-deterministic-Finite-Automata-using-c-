#include <iostream>
#include "partition.h"
#include "tableFilling.h"
using namespace std;

int main()
{
    int choice;
    cout<<"----------- DFA MINIMIZATION ALGORITHMS-----------------"<<endl;
    cout<<endl;
    cout<<"1. Table Filling Method."<<endl<<"2. Partition Method"<<endl;

   option:  
    cout<<"Enter your choice: ";
    cin >> choice;

    if(choice==1) {tableFilling();}
    else if(choice==2) {partitionMethod();}
    else
    {
        cout<<"Invalid choice!"<<endl;
        goto option;
    }
}
