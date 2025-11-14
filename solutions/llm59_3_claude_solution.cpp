#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> result(n + 1);
    vector<vector<int>> comp(n + 1, vector<int>(n + 1, 0)); // 0: unknown, 1: i < j, -1: i > j
    
    auto query = [&](int i, int j) -> int {
        if (comp[i][j] != 0) return comp[i][j];
        cout << "? " << i << " " << j << endl;
        cout.flush();
        char c;
        cin >> c;
        if (c == '<') {
            comp[i][j] = 1;
            comp[j][i] = -1;
            return 1;
        } else {
            comp[i][j] = -1;
            comp[j][i] = 1;
            return -1;
        }
    };
    
    // Sort using comparison queries
    vector<int> indices(n);
    for (int i = 0; i < n; i++) {
        indices[i] = i + 1;
    }
    
    // Use merge sort to minimize queries
    function<void(int, int)> mergeSort = [&](int l, int r) {
        if (l >= r) return;
        int mid = (l + r) / 2;
        mergeSort(l, mid);
        mergeSort(mid + 1, r);
        
        vector<int> temp;
        int i = l, j = mid + 1;
        
        while (i <= mid && j <= r) {
            if (query(indices[i], indices[j]) < 0) {
                temp.push_back(indices[i]);
                i++;
            } else {
                temp.push_back(indices[j]);
                j++;
            }
        }
        
        while (i <= mid) {
            temp.push_back(indices[i]);
            i++;
        }
        
        while (j <= r) {
            temp.push_back(indices[j]);
            j++;
        }
        
        for (int k = 0; k < temp.size(); k++) {
            indices[l + k] = temp[k];
        }
    };
    
    mergeSort(0, n - 1);
    
    // indices now contains positions sorted by their values
    for (int i = 0; i < n; i++) {
        result[indices[i]] = i + 1;
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << result[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}