#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main() {
    cout << fixed << setprecision(7);
    
    // First, find approximate center using horizontal and vertical lines
    double response;
    
    // Find center x coordinate using vertical line
    cout << "query 50000 0 50000 100000" << endl;
    cin >> response;
    
    int cx = 50000;
    if (response > 0) {
        // Binary search for left edge
        int left = 0, right = 50000;
        while (right - left > 1) {
            int mid = (left + right) / 2;
            cout << "query " << mid << " 0 " << mid << " 100000" << endl;
            cin >> response;
            if (response > 0) {
                right = mid;
            } else {
                left = mid;
            }
        }
        int left_edge = right;
        
        // Binary search for right edge
        left = 50000;
        right = 100000;
        while (right - left > 1) {
            int mid = (left + right) / 2;
            cout << "query " << mid << " 0 " << mid << " 100000" << endl;
            cin >> response;
            if (response > 0) {
                left = mid;
            } else {
                right = mid;
            }
        }
        int right_edge = left;
        
        cx = (left_edge + right_edge) / 2;
    }
    
    // Find center y coordinate using horizontal line
    cout << "query 0 50000 100000 50000" << endl;
    cin >> response;
    
    int cy = 50000;
    if (response > 0) {
        // Binary search for bottom edge
        int bottom = 0, top = 50000;
        while (top - bottom > 1) {
            int mid = (bottom + top) / 2;
            cout << "query 0 " << mid << " 100000 " << mid << endl;
            cin >> response;
            if (response > 0) {
                top = mid;
            } else {
                bottom = mid;
            }
        }
        int bottom_edge = top;
        
        // Binary search for top edge
        bottom = 50000;
        top = 100000;
        while (top - bottom > 1) {
            int mid = (bottom + top) / 2;
            cout << "query 0 " << mid << " 100000 " << mid << endl;
            cin >> response;
            if (response > 0) {
                bottom = mid;
            } else {
                top = mid;
            }
        }
        int top_edge = bottom;
        
        cy = (bottom_edge + top_edge) / 2;
    }
    
    // Find radius by probing horizontal line through center
    cout << "query 0 " << cy << " 100000 " << cy << endl;
    cin >> response;
    int r = (int)(response / 2 + 0.5);
    
    cout << "answer " << cx << " " << cy << " " << r << endl;
    
    return 0;
}