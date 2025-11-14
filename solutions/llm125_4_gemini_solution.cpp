#include <iostream>
#include <vector>
#include <numeric>

// Function to send a query for slice x
void query(int x) {
    std::cout << "? " << x << std::endl;
}

// Function to output a found pair (a, b)
void answer(int a, int b) {
    std::cout << "! " << a << " " << b << std::endl;
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    // A stack-like vector to keep track of slices in the device that are not yet paired.
    std::vector<int> unpaired_in_device;
    // A boolean array to mark slices that have been paired.
    std::vector<bool> is_paired(2 * n + 1, false);
    // The current number of distinct mineral kinds in the device.
    int current_distinct_kinds = 0;

    // Process each slice from 1 to 2N
    for (int i = 1; i <= 2 * n; ++i) {
        if (is_paired[i]) {
            continue;
        }

        // Toggle slice i's presence in the device and read the new number of distinct kinds.
        query(i);
        int new_distinct_kinds;
        std::cin >> new_distinct_kinds;

        if (new_distinct_kinds > current_distinct_kinds) {
            // If the number of distinct kinds increased, slice i is of a new kind.
            // Add it to our list of unpaired slices in the device.
            unpaired_in_device.push_back(i);
            current_distinct_kinds = new_distinct_kinds;
        } else {
            // If the number of distinct kinds did not increase, slice i must form a pair
            // with one of the slices already in the device.
            int kinds_before_search = current_distinct_kinds;
            std::vector<int> temp_removed;
            
            // Find the partner by removing slices from the unpaired list one by one.
            while (!unpaired_in_device.empty()) {
                int j = unpaired_in_device.back();
                unpaired_in_device.pop_back();

                // Remove slice j and check the new count of distinct kinds.
                query(j);
                int count_after_removal;
                std::cin >> count_after_removal;
                
                // Logic to identify the pair:
                // Let the device contain {unpaired_1, ..., unpaired_k, i}. The distinct kind count is k.
                // If we remove a slice 'j' that IS the pair of 'i', the kind of 'i' is no longer
                // duplicated, so it becomes a "new" distinct kind relative to the other k-1 slices.
                // The total distinct kinds remain k.
                // If 'j' is NOT the pair, its kind is removed entirely, and the total distinct kinds drop to k-1.
                if (count_after_removal == kinds_before_search) {
                    // j is the pair of i.
                    answer(i, j);
                    is_paired[i] = true;
                    is_paired[j] = true;
                    
                    // Remove i from the device as its pair is found.
                    query(i);
                    std::cin >> current_distinct_kinds;
                    
                    break; // Exit search for this i
                } else {
                    // j is not the pair. Keep it aside to add back later.
                    temp_removed.push_back(j);
                    // The baseline number of distinct kinds for the next check decreases.
                    kinds_before_search--;
                }
            }
            
            // Put back the slices that were removed during the search but were not the pair.
            for (int k = temp_removed.size() - 1; k >= 0; --k) {
                int val = temp_removed[k];
                query(val);
                std::cin >> current_distinct_kinds;
                unpaired_in_device.push_back(val);
            }
        }
    }

    return 0;
}