#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

int n_states; 
int tgroups;  
int n_inputs; 

class State {
public:
    int state_no; 
    int group_no; 
    int* transitedState; 
    char status; 
    void set(int); 
};


int Minimize(State s[]) {
    int g_reached[n_states]; 
    for(int I=0; I<n_states-2; I++) { 
        for(int j=0; j<n_inputs; j++) { 
            for(int i=0; i<n_states; i++) {
                int temp = s[i].transitedState[j];
                g_reached[i] = s[temp].group_no;
            }
            for(int i=0; i<n_states-1; i++) {
                int group_record[n_states]; 
                memset(group_record, -1, sizeof(group_record));
                for(int k=i+1; k<n_states; k++) {
                    if(s[i].group_no == s[k].group_no && g_reached[i] != g_reached[k]) {
                        int f = -1;
                        for(int l=0; l<n_states; l++) { 
                            if(g_reached[k] == group_record[l]) {
                                f = l;
                                break;
                            }
                        }
                        if(f == -1) { 
                            s[k].group_no = tgroups;
                            tgroups++;
                            group_record[k] = g_reached[k];
                        } else { 
                            s[k].group_no = s[f].group_no;
                        }
                    }
                }
            }
        }
        int equivalence = I;
        for(int i=0; i<n_states; i++) {
            if(s[i].group_no != g_reached[i]) {
                equivalence = -1;
                break;
            }
        }
        if(equivalence != -1) {
            return equivalence;
        }
    }
}

void represent(State s[]) {
    for(int i=0; i<n_states-1; i++) {
        if(s[i].state_no != -1) {
            for(int j=i+1; j<n_states; j++) {
                if(s[i].group_no == s[j].group_no) {
                    for(int k=0; k<n_states; k++) {
                        for(int l=0; l<n_inputs; l++) {
                            if(s[k].transitedState[l] == s[j].state_no)
                                s[k].transitedState[l] = i;
                        }
                    }
                    s[j].state_no = -1; 
                    if(s[j].status == 'F' || s[j].status == 'f')
                        s[i].status = 'F';
                }
            }
        }
    }
}



// Function to get the minimum equivalence partition
void GetEquivalencePartition(State s[]) {
    cout << "*Minimum Equivalence Partition\n";
    for(int i=0; i<tgroups; i++) {
        cout << "{ ";
        for(int j=0; j<n_states; j++) {
            if(s[j].group_no == i)
                cout << "q" << s[j].state_no << " ";
        }
        cout << "}\n";
    }
}


void generateDOTFile(State s[]) {
    ofstream outputFile;
    outputFile.open("minimized_dfa.dot");

    // Write DOT file header
    outputFile << "digraph MinimizedDFA {\n";
    outputFile << "rankdir=LR;\n";

    // Write states and transitions
    for(int i=0; i<n_states; i++) {
        if(s[i].state_no != -1) {
            if (s[i].status == 'f' || s[i].status == 'F')
                outputFile << "q" << s[i].state_no << " [shape=doublecircle];\n";
            else
                outputFile << "q" << s[i].state_no << " [shape=circle];\n";

            for(int j=0; j<n_inputs; j++) {
                if(s[i].transitedState[j] != -1) {
                    outputFile << "q" << s[i].state_no << " -> q" << s[i].transitedState[j] << " [label=\"" << j << "\"];\n";
                }
            }
        }
    }
    outputFile << "}\n";
    outputFile.close();

    cout << "\nDOT file 'minimized_dfa.dot' generated successfully.\n";
}

void State::set(int n_inputs) {
    transitedState = new int[n_inputs];
    cout << "State: q";
    cin >> state_no;
    cout << "\nFinal or non-final state(f/n): ";
    cin >> status;
    if(status == 'n' || status == 'N')
        group_no = 0; 
    else if(status == 'f' || status == 'F')
        group_no = 1;
    cout << "\nEnter the transition states for the following inputs: \n";
    for(int i=0; i<n_inputs; i++) {
        cout << "Input " << i << ": q";
        cin >> transitedState[i];
        cout << "\n";
    }
}

int partitionMethod() {
    cout << "\n";
    cout << "Enter number of states: ";
    cin >> n_states;
    cout << "Enter number of input symbols: ";
    cin >> n_inputs;
    State s[n_states];
    for(int i=0; i<n_states; i++) {
        s[i].set(n_inputs);
    }
    tgroups = 2; // Initially there are two partitions, final and non-final states
    int equivalence = Minimize(s);
    GetEquivalencePartition(s);
    represent(s);
    
    generateDOTFile(s);

    return 0;
}
