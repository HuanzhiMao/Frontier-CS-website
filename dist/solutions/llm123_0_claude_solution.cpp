#include <iostream>
#include <vector>
#include <set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    set<int> possible;
    for (int i = 1; i <= n; i++) {
        possible.insert(i);
    }
    
    string prev_answer = "";
    set<int> prev_set;
    
    while (possible.size() > 1) {
        // Split possible set roughly in half
        vector<int> query_set;
        int half = (possible.size() + 1) / 2;
        int count = 0;
        
        for (int x : possible) {
            if (count < half) {
                query_set.push_back(x);
                count++;
            } else {
                break;
            }
        }
        
        // Ask the question
        cout << "? " << query_set.size();
        for (int x : query_set) {
            cout << " " << x;
        }
        cout << endl;
        cout.flush();
        
        string answer;
        cin >> answer;
        
        if (prev_answer == "") {
            // First question - we don't know if it's reliable
            prev_answer = answer;
            prev_set.clear();
            for (int x : query_set) {
                prev_set.insert(x);
            }
        } else {
            // We have a previous answer, so at least one of the two consecutive answers is correct
            // We need to consider both possibilities
            
            set<int> new_possible1, new_possible2;
            
            // Case 1: previous answer was correct, current might be wrong
            if (prev_answer == "YES") {
                // x must be in prev_set
                for (int x : prev_set) {
                    if (possible.count(x)) {
                        new_possible1.insert(x);
                    }
                }
            } else {
                // x must not be in prev_set
                for (int x : possible) {
                    if (!prev_set.count(x)) {
                        new_possible1.insert(x);
                    }
                }
            }
            
            // Case 2: current answer is correct, previous might have been wrong
            set<int> current_set(query_set.begin(), query_set.end());
            if (answer == "YES") {
                // x must be in current_set
                for (int x : current_set) {
                    if (possible.count(x)) {
                        new_possible2.insert(x);
                    }
                }
            } else {
                // x must not be in current_set
                for (int x : possible) {
                    if (!current_set.count(x)) {
                        new_possible2.insert(x);
                    }
                }
            }
            
            // The actual possible set is the union of both cases
            possible.clear();
            for (int x : new_possible1) {
                possible.insert(x);
            }
            for (int x : new_possible2) {
                possible.insert(x);
            }
            
            prev_answer = answer;
            prev_set = current_set;
        }
        
        // If we're down to 2 or fewer possibilities, make a guess
        if (possible.size() <= 2) {
            int guess = *possible.begin();
            cout << "! " << guess << endl;
            cout.flush();
            
            string result;
            cin >> result;
            
            if (result == ":)") {
                return 0;
            } else {
                // Our guess was wrong, remove it from possibilities
                possible.erase(guess);
                if (!possible.empty()) {
                    cout << "! " << *possible.begin() << endl;
                    cout.flush();
                    cin >> result;
                }
                return 0;
            }
        }
    }
    
    if (possible.size() == 1) {
        cout << "! " << *possible.begin() << endl;
        cout.flush();
        string result;
        cin >> result;
    }
    
    return 0;
}