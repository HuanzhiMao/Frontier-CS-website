#include <bits/stdc++.h>
using namespace std;

struct Item {
    string name;
    int q, v, m, l;
    double ratio;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    string line;
    string input = "";
    while (getline(cin, line)) {
        input += line;
    }
    
    vector<Item> items;
    map<string, int> result;
    
    // Parse JSON manually
    size_t pos = 0;
    while ((pos = input.find("\"", pos)) != string::npos) {
        pos++;
        size_t end = input.find("\"", pos);
        if (end == string::npos) break;
        
        string name = input.substr(pos, end - pos);
        pos = end + 1;
        
        // Find the array
        pos = input.find("[", pos);
        if (pos == string::npos) break;
        pos++;
        
        vector<long long> values;
        string num = "";
        bool inNumber = false;
        
        while (pos < input.length() && input[pos] != ']') {
            if (isdigit(input[pos])) {
                num += input[pos];
                inNumber = true;
            } else if (inNumber) {
                values.push_back(stoll(num));
                num = "";
                inNumber = false;
            }
            pos++;
        }
        if (inNumber) {
            values.push_back(stoll(num));
        }
        
        if (values.size() == 4) {
            Item item;
            item.name = name;
            item.q = values[0];
            item.v = values[1];
            item.m = values[2];
            item.l = values[3];
            item.ratio = (double)item.v / (item.m + item.l);
            items.push_back(item);
            result[name] = 0;
        }
        pos++;
    }
    
    // Sort by value/weight ratio
    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.ratio > b.ratio;
    });
    
    // Greedy approach with multiple strategies
    map<string, int> best_result = result;
    long long best_value = 0;
    
    // Strategy 1: By value/weight ratio
    map<string, int> current_result = result;
    long long current_mass = 0, current_volume = 0, current_value = 0;
    
    for (const Item& item : items) {
        int can_take = item.q;
        while (can_take > 0 && 
               current_mass + item.m <= 20000000 && 
               current_volume + item.l <= 25000000) {
            current_mass += item.m;
            current_volume += item.l;
            current_value += item.v;
            current_result[item.name]++;
            can_take--;
        }
    }
    
    if (current_value > best_value) {
        best_value = current_value;
        best_result = current_result;
    }
    
    // Strategy 2: By value only
    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.v > b.v;
    });
    
    current_result = result;
    current_mass = current_volume = current_value = 0;
    
    for (const Item& item : items) {
        int can_take = item.q;
        while (can_take > 0 && 
               current_mass + item.m <= 20000000 && 
               current_volume + item.l <= 25000000) {
            current_mass += item.m;
            current_volume += item.l;
            current_value += item.v;
            current_result[item.name]++;
            can_take--;
        }
    }
    
    if (current_value > best_value) {
        best_value = current_value;
        best_result = current_result;
    }
    
    // Strategy 3: By value/(mass+volume) ratio
    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return (double)a.v / (a.m + a.l) > (double)b.v / (b.m + b.l);
    });
    
    current_result = result;
    current_mass = current_volume = current_value = 0;
    
    for (const Item& item : items) {
        int can_take = item.q;
        while (can_take > 0 && 
               current_mass + item.m <= 20000000 && 
               current_volume + item.l <= 25000000) {
            current_mass += item.m;
            current_volume += item.l;
            current_value += item.v;
            current_result[item.name]++;
            can_take--;
        }
    }
    
    if (current_value > best_value) {
        best_value = current_value;
        best_result = current_result;
    }
    
    // Output JSON
    cout << "{\n";
    bool first = true;
    for (const auto& item : items) {
        if (!first) cout << ",\n";
        cout << " \"" << item.name << "\": " << best_result[item.name];
        first = false;
    }
    cout << "\n}\n";
    
    return 0;
}