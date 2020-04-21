
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <sstream>
#include <queue>
#include <vector>

using namespace std;

int rec(const vector<int> &c1, const vector<int> &c2, vector<vector<vector<int>>> &mem, int q, int i, int j)
{
        if ((i == c1.size()) || (j == c2.size()))
                return 0;
        if (q < 0)
                return 0;
        if (mem[q][i][j] == -1)
        {
                int t1 = rec(c1, c2, mem, q, i+1, j);
                int t2 = rec(c1, c2, mem, q, i, j+1);
                int t3 = 0;
                if (c1[i] + c2[j] <= q)
                {
                        t3 = c1[i] + c2[j] + rec(c1, c2, mem, q - (c1[i] + c2[j]), i+1, j+1);
                }
                mem[q][i][j] = max(t1, max(t2, t3));
        }
        return mem[q][i][j];
}

int solveOne(int Q, int N)
{
        vector<int> a1, a2;
        a1.reserve(N);
        a2.reserve(N);
        int a,b;
        for (int i = 0; i < 2*N; ++i)
        {
                cin >> a >> b;
                if (a == 1 && b <= Q)
                        a1.push_back(b);
                if (a == 2 && b <= Q)
                        a2.push_back(b);
        }

        vector<vector<vector<int>>> mem(Q+1, vector<vector<int>>(N, vector<int>(N, -1)));

        return rec(a1, a2, mem, Q, 0, 0);
}

int main()
{        
        ios::sync_with_stdio(false); // IO "optimization"
        cin.tie(nullptr);

        int q,n;
        cin >> q >> n;
        cout << solveOne(q, n/2) << "\n";
        return 0;
}

