#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    registerTestlibCmd(argc, argv);

    // 从输入文件读取 n
    int n = inf.readInt();

    // 读取选手输出 f_user
    vector<int> f_user(n + 1);
    for (int i = 1; i <= n; ++i) {
        f_user[i] = ouf.readInt();
        if (abs(f_user[i]) != 1)
            quitf(_wa, "f_user(%d) = %d, |f(i)| must be 1", i, f_user[i]);
    }
    ouf.ensuref(ouf.seekEof(), "Extra output after %d numbers", n);

    // 读取标准答案 f_std
    vector<int> f_std(n + 1);
    for (int i = 1; i <= n; ++i) {
        f_std[i] = ans.readInt();
        if (abs(f_std[i]) != 1)
            quitf(_fail, "Invalid standard output: f_std(%d) = %d", i, f_std[i]);
    }

    // 验证完全积性：f(xy) = f(x)*f(y)
    // 这里采用 O(n log n) 检查（枚举倍数）
    for (int x = 1; x <= n; ++x) {
        for (int xy = x, y = 1; xy <= n; ++y, xy += x) {
            if (f_user[xy] != f_user[x] * f_user[y])
                quitf(_wa, "Not multiplicative: f(%d*%d)=%d, but f(%d)*f(%d)=%d",
                      x, y, f_user[xy], x, y, f_user[x]*f_user[y]);
        }
    }

    // 计算最大前缀和幅度（用户）
    long long s_user = 0, M_user = 0;
    for (int i = 1; i <= n; ++i) {
        s_user += f_user[i];
        M_user = max(M_user, llabs(s_user));
    }

    // 计算最大前缀和幅度（标准答案）
    long long s_std = 0, M_std = 0;
    for (int i = 1; i <= n; ++i) {
        s_std += f_std[i];
        M_std = max(M_std, llabs(s_std));
    }

    // 打分规则：越小越好
    if (M_user == 0)
        quitf(_fail, "User M=0 (impossible)");

    double ratio = (double)M_std / (double)M_user;
    double score = max(0.0, min(1.0, ratio));

    quitp(score, "User M=%lld, Std M=%lld, Ratio: %.4f", M_user, M_std, score);
}
