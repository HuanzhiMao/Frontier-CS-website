#include <iostream>
#include <vector>
#include <string>
using namespace std;

int N, R;
vector<pair<int, int>> connections;

int query(const string& s) {
    cout << "? " << s << endl;
    int result;
    cin >> result;
    return result;
}

void answer(const string& t) {
    cout << "! " << t << endl;
}

int main() {
    cin >> N >> R;
    connections.resize(N);
    
    for (int i = 0; i < N; i++) {
        cin >> connections[i].first >> connections[i].second;
    }
    
    string result(N, '&');
    
    for (int i = 0; i < N; i++) {
        int u = connections[i].first;
        int v = connections[i].second;
        
        // Test case 1: both inputs are 0
        string s1(2*N + 1, '0');
        int out1 = query(s1);
        
        // Test case 2: first input is 1, second is 0
        string s2(2*N + 1, '0');
        s2[u] = '1';
        int out2 = query(s2);
        
        // Test case 3: first input is 0, second is 1
        string s3(2*N + 1, '0');
        s3[v] = '1';
        int out3 = query(s3);
        
        // Test case 4: both inputs are 1
        string s4(2*N + 1, '0');
        s4[u] = '1';
        s4[v] = '1';
        int out4 = query(s4);
        
        // Determine gate type based on outputs
        // For AND gate: 00->0, 01->0, 10->0, 11->1
        // For OR gate:  00->0, 01->1, 10->1, 11->1
        
        if (out2 == 1 || out3 == 1) {
            result[i] = '|';
        } else {
            result[i] = '&';
        }
    }
    
    answer(result);
    return 0;
}