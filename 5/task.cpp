#include <bits/stdc++.h>
using namespace std;

int n;
vector<list<int>> a;
unordered_map<int, int> col;

bool dfs(int v) {
    col[v] = 1;

    for (int u : a[v]) {
        if (col[u] == 0)
            if (!dfs(u))
                return false;

        if (col[u] == 1)
            return false;
    }

    col[v] = 2;
    return true;
}

void solve() {
    scanf("%d", &n);
    col.clear();
    a.clear();
    a.resize(n);
    for (int i = 0; i < n; ++i) {
        int k;
        scanf("%d", &k);
        for (int j = 0; j < k; ++j) {
            int c;
            scanf("%d", &c);
            a[i].push_back(c - 1);
        }
    }

    for (int v = 0; v < n; ++v) {
        if (col[v] != 2)
            if (!dfs(v)) {
                puts("NO");
                return;
            }
    }

    puts("YES");
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        solve();
    }
}

