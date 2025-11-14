#include <bits/stdc++.h>
using namespace std;

int N, M;
vector<pair<int, int>> edges;

int query(vector<int>& dirs) {
    cout << "0";
    for (int d : dirs) {
        cout << " " << d;
    }
    cout << endl;
    cout.flush();
    int result;
    cin >> result;
    return result;
}

void answer(int A, int B) {
    cout << "1 " << A << " " << B << endl;
    cout.flush();
}

int main() {
    cin >> N >> M;
    edges.resize(M);
    
    for (int i = 0; i < M; i++) {
        cin >> edges[i].first >> edges[i].second;
    }
    
    // Binary search for source A
    int left = 0, right = N - 1;
    while (left < right) {
        int mid = (left + right) / 2;
        
        // Create a configuration where nodes 0...mid can only go forward
        // and nodes mid+1...N-1 can only go backward
        vector<int> dirs(M);
        for (int i = 0; i < M; i++) {
            int u = edges[i].first;
            int v = edges[i].second;
            
            if (u <= mid && v <= mid) {
                dirs[i] = 0; // u -> v
            } else if (u > mid && v > mid) {
                dirs[i] = 1; // v -> u
            } else if (u <= mid && v > mid) {
                dirs[i] = 0; // u -> v (forward)
            } else {
                dirs[i] = 1; // v -> u (backward)
            }
        }
        
        int result = query(dirs);
        if (result == 1) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    
    int A = left;
    
    // Binary search for destination B
    left = 0;
    right = N - 1;
    while (left < right) {
        int mid = (left + right) / 2;
        
        // Create a configuration where we can reach nodes 0...mid from anywhere
        // but cannot reach nodes mid+1...N-1
        vector<int> dirs(M);
        for (int i = 0; i < M; i++) {
            int u = edges[i].first;
            int v = edges[i].second;
            
            if (u <= mid && v <= mid) {
                dirs[i] = 0; // u -> v
            } else if (u > mid && v > mid) {
                dirs[i] = 0; // u -> v
            } else if (u <= mid && v > mid) {
                dirs[i] = 1; // v -> u (prevent going to >mid)
            } else {
                dirs[i] = 0; // u -> v (allow going to <=mid)
            }
        }
        
        int result = query(dirs);
        if (result == 1) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    
    int B = left;
    
    answer(A, B);
    return 0;
}