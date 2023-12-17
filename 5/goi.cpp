#include <iostream>

using namespace std;

int **a;
int *col;
int t, n, k;
int *k_mass;

bool deep(int ind)
{
    if((col[ind] == 2) || (ind == -1))
    {
        return false;
    }
    else if(col[ind] == 1)
    {
        return true;
    }
    else
    {
        col[ind] = 1;
        for(int i = 0; i < k_mass[ind]; ++i)
        {
            if(deep(a[ind][i] - 1))
            {
                return true;
            }
        }
        col[ind] = 2;
        return false;
    }
}

int main() {
    cin >> t;
    for(int m = 0; m < t; ++m)
    {
        cin >> n;
        col = new int[n];
        a = new int*[n];
        k_mass = new int[n];
        for (int i = 0; i < n; ++i) {
            cin >> k;
            a[i] = new int[k];
            k_mass[i] = k;
            col[i] = 0;
            for (int p = 0; p < k; ++p) {
                cin >> a[i][p];
            }
        }
        bool ans = false;
        for(int i = 0; i < n; ++i)
        {
            if(deep(i))
            {
                ans = true;
            }
        }
        if(ans)
        {
            cout << "NO" << endl;
        }
        else
        {
            cout << "YES" << endl;
        }
    }
    return 0;
}
