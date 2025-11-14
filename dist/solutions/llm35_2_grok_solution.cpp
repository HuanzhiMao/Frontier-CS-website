#include <bits/stdc++.h>

int main() {
    int t;
    scanf("%d", &t);
    for (int test = 1; test <= t; test++) {
        int n;
        scanf("%d", &n);
        int m = 2 * n - 1;
        int check = 0;
        if (test > 1) {
            scanf("%d", &check);
            if (check == -1) return 0;
        }
        int y = -1;
        for (int x = 1; x <= n; x++) {
            int left = 1, right = m;
            while (left < right) {
                int mid = (left + right) / 2;
                int sz = mid - left + 1;
                printf("? %d %d", x, sz);
                for (int i = left; i <= mid; i++) {
                    printf(" %d", i);
                }
                printf("\n");
                fflush(stdout);
                int ans;
                scanf("%d", &ans);
                if (ans == -1) return 0;
                if (ans == 1) {
                    right = mid;
                } else {
                    left = mid + 1;
                }
            }
            int p = left;
            int sz = m - 1;
            printf("? %d %d", x, sz);
            for (int i = 1; i <= m; i++) {
                if (i != p) printf(" %d", i);
            }
            printf("\n");
            fflush(stdout);
            int ans;
            scanf("%d", &ans);
            if (ans == -1) return 0;
            if (ans == 0) {
                y = x;
            }
        }
        printf("! %d\n", y);
        fflush(stdout);
    }
    return 0;
}