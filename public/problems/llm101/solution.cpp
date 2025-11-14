#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include "testlib.h"
using namespace std;
enum {
    INVALID_INPUT = -1,        // 评测端输入非法（仅评测内部使用）
    INVALID_T_LENGTH = 1,      // t 长度 != N
    INVALID_T_CHAR   = 2,      // t 含非法字符
    WRONG_T          = 3,      // t 与真值不一致
    INVALID_S_LENGTH = 4,      // s 长度 != 2N+1
    INVALID_S_CHAR   = 5,      // s 含非法字符
    QUERY_LIMIT_EXCEEDED = 6,  // 询问次数 > 1000
    INVALID_OUTPUT   = 9,      // 选手输出行的首字符既不是 '?' 也不是 '!'
};

const int N_MAX = 8000;
const int QUERY_LIMIT = 5000;

int N, R;
std::vector<int> U, V;
std::string T;
int QUERY_COUNT = 0;

double score(int x){
    if(x<=900)return 1;
    if(x>5000)return 0;
    return (5000.0-x)/(5000.0-900.0);
}

[[noreturn]] void wrong(const int num) {
    // 向选手程序写一个标志（避免卡读），随后用 testlib 结束
    fprintf(stdout, "-1\n");
    fflush(stdout);
    quitf(_wa, "translate:wrong\nWrong Answer [%d]\n", num);
}

[[noreturn]] void ok() {
    quitp(score(QUERY_COUNT), "Ratio: %.4f , Queries: %d", score(QUERY_COUNT), QUERY_COUNT);
}

int query(std::string s) {
    const int M = 2 * N + 1;
    if ((int)s.size() != M) {
        wrong(INVALID_S_LENGTH);
    }
    for (char c : s) {
        if (c != '0' && c != '1') wrong(INVALID_S_CHAR);
    }
    if (QUERY_COUNT == QUERY_LIMIT) {
        wrong(QUERY_LIMIT_EXCEEDED);
    }
    QUERY_COUNT++;

    // 将 '0'/'1' 转为 0/1
    for (char &c : s) c -= '0';

    // 自高到低地计算槽输出并折叠到开关 i（XOR 技巧实现 OFF/ON 两种行为）
    for (int i = N - 1; i >= 0; --i) {
        const int u = U[i], v = V[i];
        if (T[i] == '&') {
            s[i] ^= (s[u] & s[v]);
        } else { // '|'
            s[i] ^= (s[u] | s[v]);
        }
    }
    // 返回最终开关 0 的输出
    return s[0];
}

void answer(std::string t) {
    if ((int)t.size() != N) {
        wrong(INVALID_T_LENGTH);
    }
    for (char c : t) {
        if (c != '&' && c != '|') wrong(INVALID_T_CHAR);
    }
    if (t != T) {
        wrong(WRONG_T);
    }
    ok();
}

int main(int argc, char* argv[]) {
    registerInteraction(argc, argv);

    // ---------- 读取评测输入（仅评测端使用） ----------
    N = inf.readInt();     // 1..8000
    R = inf.readInt();     // 1..min(N,120)
    if (N < 1 || N > N_MAX) {
        wrong(INVALID_INPUT);
    }
    cout<<N<<" "<<R<<endl;
    U.resize(N);
    V.resize(N);
    for (int i = 0; i < N; ++i) {
        U[i] = inf.readInt();
        V[i] = inf.readInt();
        /*if (!(i < U[i] && U[i] < V[i] && V[i] <= 2 * N)) {
            wrong(INVALID_INPUT);
        }*/
       cout<<U[i]<<" "<<V[i]<<endl;
    }

    // 隐藏真值（仅评测端使用，选手不可见）
    T = inf.readToken();
    if ((int)T.size() != N) {
        wrong(INVALID_INPUT);
    }
    int orCount = 0;
    for (char c : T) {
        if (c != '&' && c != '|') wrong(INVALID_INPUT);
        if (c == '|') ++orCount;
    }
    if (orCount > R) {
        // 评测数据与声明的 R 不一致
        wrong(INVALID_INPUT);
    }

    // ---------- 输出给选手的初始可见信息 ----------
    // 格式：N R，然后 N 行 U[i] V[i]
    /*fprintf(stdout, "%d %d\n", N, R);
    for (int i = 0; i < N; ++i) {
        fprintf(stdout, "%d %d\n", U[i], V[i]);
    }
    fflush(stdout);*/

    // ---------- 交互循环 ----------
    while (true) {
        // 读取操作符（'?', '!'）
        std::string op = ouf.readToken(); // 读一个 token
        if (op.empty()) wrong(INVALID_OUTPUT);
        const char type = op[0];
        if (type != '?' && type != '!') wrong(INVALID_OUTPUT);

        // 读取紧随其后的字符串（s 或 t）
        std::string payload = ouf.readToken();

        // 兼容性处理：若末尾带换行，去掉（通常 readToken 不会带）
        if (!payload.empty() && payload.back() == '\n') payload.pop_back();

        if (type == '?') {
            // 处理查询
            int res = query(payload);
            fprintf(stdout, "%d\n", res);
            fflush(stdout);
        } else {
            // 最终回答
            answer(payload);
            // answer() 内部会 quitf；正常不会走到这里
        }
    }
}
