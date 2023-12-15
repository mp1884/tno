#include <bits/stdc++.h>
using namespace std;

#define MAXN 1000

int n, xs[MAXN], hs[MAXN];
int ans = INT_MAX;

void fall_left(int i, list<int> &falls) {
    if (falls.size() == 0)
        return;

    int fall_until = xs[i] - hs[i];
    auto it = falls.end();
    --it;
    while (xs[*it] >= fall_until) {
        fall_until = min(fall_until, xs[*it] - hs[*it]);
        it = falls.erase(it);
        if (falls.empty())
            break;
        --it;
    }
}

void bf(int i, bool f, list<int> falls) {
    if (i >= n) {
        ans = min(ans, (int)falls.size());
        return;
    }

    if (f) {
        fall_left(i, falls);
        falls.push_back(i);
        bf(i + 1, true, falls);
        bf(i + 1, false, falls);
    } else {
        falls.push_back(i);
        int fall_until = xs[i] + hs[i];
        while (xs[i] <= fall_until && i < n) {
            fall_until = max(fall_until, xs[i] + hs[i]);
            i++;
        }
        bf(i, true, falls);
        bf(i, false, falls);
    }
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
        scanf("%d", xs + i);
    for (int i = 0; i < n; ++i)
        scanf("%d", hs + i);
    bf(0, true, {});
    bf(0, false, {});
    printf("%d\n", ans);
}
