#include <bits/stdc++.h>
using namespace std;

struct Item {
    string name;
    int quantity;
    long long value;
    long long mass;
    long long volume;
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
    
    // Parse JSON manually
    size_t pos = 0;
    while (pos < input.length()) {
        size_t start = input.find('"', pos);
        if (start == string::npos) break;
        
        size_t end = input.find('"', start + 1);
        string name = input.substr(start + 1, end - start - 1);
        
        size_t bracket_start = input.find('[', end);
        size_t bracket_end = input.find(']', bracket_start);
        
        string values_str = input.substr(bracket_start + 1, bracket_end - bracket_start - 1);
        
        vector<long long> values;
        stringstream ss(values_str);
        string token;
        while (getline(ss, token, ',')) {
            token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
            values.push_back(stoll(token));
        }
        
        Item item;
        item.name = name;
        item.quantity = values[0];
        item.value = values[1];
        item.mass = values[2];
        item.volume = values[3];
        item.ratio = (double)item.value / max((double)item.mass / 20000000.0, (double)item.volume / 25000000.0);
        
        items.push_back(item);
        
        pos = bracket_end + 1;
    }
    
    // Sort by value/constraint ratio
    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.ratio > b.ratio;
    });
    
    const long long MAX_MASS = 20000000;  // 20kg in mg
    const long long MAX_VOLUME = 25000000; // 25L in µl
    
    map<string, int> result;
    for (const auto& item : items) {
        result[item.name] = 0;
    }
    
    // Greedy approach with some optimization
    long long current_mass = 0;
    long long current_volume = 0;
    long long current_value = 0;
    
    // Try greedy first
    map<string, int> greedy_result = result;
    long long greedy_mass = 0, greedy_volume = 0, greedy_value = 0;
    
    for (const auto& item : items) {
        int max_by_mass = (MAX_MASS - greedy_mass) / item.mass;
        int max_by_volume = (MAX_VOLUME - greedy_volume) / item.volume;
        int can_take = min({item.quantity, max_by_mass, max_by_volume});
        
        if (can_take > 0) {
            greedy_result[item.name] = can_take;
            greedy_mass += can_take * item.mass;
            greedy_volume += can_take * item.volume;
            greedy_value += can_take * item.value;
        }
    }
    
    // Try different combinations focusing on high-value items
    long long best_value = greedy_value;
    map<string, int> best_result = greedy_result;
    
    // Try prioritizing different items
    for (int priority = 0; priority < min(12, (int)items.size()); priority++) {
        map<string, int> temp_result = result;
        long long temp_mass = 0, temp_volume = 0, temp_value = 0;
        
        // First, try to take as many of the priority item as possible
        const auto& priority_item = items[priority];
        int max_by_mass = MAX_MASS / priority_item.mass;
        int max_by_volume = MAX_VOLUME / priority_item.volume;
        int can_take = min({priority_item.quantity, max_by_mass, max_by_volume});
        
        temp_result[priority_item.name] = can_take;
        temp_mass += can_take * priority_item.mass;
        temp_volume += can_take * priority_item.volume;
        temp_value += can_take * priority_item.value;
        
        // Then fill with other items greedily
        for (const auto& item : items) {
            if (item.name == priority_item.name) continue;
            
            max_by_mass = (MAX_MASS - temp_mass) / item.mass;
            max_by_volume = (MAX_VOLUME - temp_volume) / item.volume;
            can_take = min({item.quantity, max_by_mass, max_by_volume});
            
            if (can_take > 0) {
                temp_result[item.name] = can_take;
                temp_mass += can_take * item.mass;
                temp_volume += can_take * item.volume;
                temp_value += can_take * item.value;
            }
        }
        
        if (temp_value > best_value) {
            best_value = temp_value;
            best_result = temp_result;
        }
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