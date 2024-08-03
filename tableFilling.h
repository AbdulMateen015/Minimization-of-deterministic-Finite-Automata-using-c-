#include<bits/stdc++.h>
using namespace std;

#define MARKED 1
#define UNMARKED -1

int total_state, total_symbol, final_states_count, a;
vector<int> final_states;
int transition[10][10], m[10][10], newstate[10][10], arr[10], new_transition[10][10];

void store_dfa() {
    int i, j, final_state;
    cout << "Enter total no. of states neglecting unreachable states: ";
    cin >> total_state;
    cout << "Enter total no. of allowed symbols: ";
    cin >> total_symbol;
    cout << "Enter the number of final states: ";
    cin >> final_states_count;
    cout << "Enter the final states: ";
    for (i = 0; i < final_states_count; i++) {
        cin >> final_state;
        final_states.push_back(final_state);
    }
    cout << "Enter the transitions: " << endl;
    for (i = 0; i < total_state; i++) {
        for (j = 0; j < total_symbol; j++) {
            cout << i << " ---> " << char(j + 97) << ": ";
            cin >> transition[i][j];
        }
    }
}

void show_dfa() {
    int i, j;
    cout << endl << endl << endl;
    cout << endl << "   TRANSITION TABLE OF ENTERED DFA IS AS FOLLOWS ";
    cout << endl << "====================================================" << endl;
    for (i = 0; i < total_symbol; i++)
        cout << "\t" << char(i + 97);
    cout << endl;
    for (i = 0; i < (total_symbol + 1); i++)
        cout << "-\t";
    cout << "\n";
    for (i = 0; i < total_state; i++) {
        cout << "\n" << i << "\t";
        for (j = 0; j < total_symbol; j++) {
            cout << transition[i][j] << "\t";
        }
    }
    cout << endl << endl << "Total no. is states: " << total_state;
    cout << endl << "Final states are: ";
   for (size_t i = 0; i < final_states.size(); ++i)
    cout << " " << final_states[i];

}

void mark_dfa() {
    int i, j, k, x, y;
    for (i = 0; i < total_state; i++) {
        for (j = 0; j < total_state; j++) {
            if (j > i) {
                m[i][j] = MARKED;
            } else {
                bool is_final_i = find(final_states.begin(), final_states.end(), i) != final_states.end();
                bool is_final_j = find(final_states.begin(), final_states.end(), j) != final_states.end();
                if ((is_final_i && !is_final_j) || (!is_final_i && is_final_j) && (i != j)) {
                    m[i][j] = MARKED;
                } else {
                    m[i][j] = UNMARKED;
                }
            }
        }
    }
    for (i = 0; i < total_state; i++) {
        for (j = i + 1; j < total_state; j++) {
            if (m[j][i] == UNMARKED) {
                for (k = 0; k < total_symbol; k++) {
                    x = transition[i][k];
                    y = transition[j][k];
                    if (m[x][y] == MARKED || m[y][x] == MARKED) {
                        m[j][i] = MARKED;
                        break;
                    }
                }
            }
        }
    }
}

int check(int x) {
    for (int c = 0; c < total_state; c++) {
        if (x == arr[c]) {
            return 0;
        }
    }
    return 1;
}

void newstate_table() {
    int i, j, k = 0, b = 0;
    for (i = 0; i < total_state; i++) {
        for (j = 0; j < total_state; j++) {
            newstate[i][j] = total_state;
        }
    }
    for (i = 0; i < total_state; i++) {
        arr[i] = total_state;
    }
    for (i = 0; i < total_state; i++) {
        if (check(i) != 0) {
            b = 0;
            newstate[a][b] = i;
            for (j = i + 1; j < total_state; j++) {
                if (m[j][i] == UNMARKED) {
                    arr[k] = j;
                    k++;
                    newstate[a][b + 1] = j;
                }
            }
            a++;
        }
    }
}

int find_s(int x) {
    for (int i = 0; i <= a; i++) {
        for (int j = 0; j < total_state; j++) {
            if (x == newstate[i][j])
                return i;
        }
    }
}

void trans_table() {
    int i, x, s1, k;
    for (i = 0; i <= a; i++) {
        x = newstate[i][0];
        for (k = 0; k < total_symbol; k++) {
            s1 = transition[x][k];
            int fin = find_s(s1);
            new_transition[i][k] = fin;
        }
    }
   for (int i = 0; i < final_states.size(); i++)
    cout << " " << final_states[i];

}

void show_minimized_dfa() {
    int i, j;
    cout << "\n\n\n";
    cout << "\n   MINIMIZED DFA TABLE";
    cout << "\n===========================\n\t";
    for (i = 0; i < total_symbol; i++)
        cout << char(i + 97) << "\t";
    cout << "\n";
    for (i = 0; i < (total_symbol + 1); i++)
        cout << "-\t";
    cout << "\n";
    for (i = 0; i < a; i++) {
        cout << "\n" << i << "\t";
        for (j = 0; j < total_symbol; j++) {
            cout << new_transition[i][j] << "\t";
        }
    }
    cout << endl << endl << "Total no. of new state is :" << a;
    cout << endl << "Final states are :";
   for (int i = 0; i < final_states.size(); i++)
    cout << " " << final_states[i];

}

void generate_dot_file() {
    ofstream dotfile("minimized_dfa.dot");
    dotfile << "digraph DFA {" << endl;

    // Nodes
    for (int i = 0; i < a; i++) {
        dotfile << "\t" << i << " [shape=circle";
        if (i == final_states_count) dotfile << ", peripheries=2"; // Final state
        dotfile << "]" << endl;
    }

    // Transitions
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < total_symbol; j++) {
            dotfile << "\t" << i << " -> " << new_transition[i][j] << " [label=\"" << char(j + 97) << "\"];" << endl;
        }
    }

    dotfile << "}" << endl;
    dotfile.close();
}

void tableFilling() {
    store_dfa();
    show_dfa();
    mark_dfa();
    newstate_table();
    trans_table();
    show_minimized_dfa();
    generate_dot_file(); // Call the function to generate DOT file
}
