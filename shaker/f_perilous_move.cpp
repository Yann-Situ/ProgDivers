
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <sstream>
#include <queue>
#include <vector>
using namespace std;

#define forn(i, n) for (int i = 0; i < (n); ++i)
#define foreach(it, a) for (__typeof((a).begin()) it = (a).begin(); it != (a).end(); ++it)
typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<VVI> VVVI;
typedef vector<VVVI> VVVVI;
typedef double T;
typedef vector<T> VT;
typedef long long LL;
//Struct edge pour les algos pas flots
struct edge {
        int x, y, w;
        edge(void): x(),y(),w() {};
        edge(int a, int b, int c): x(a), y(b), w(c) {}
        bool operator< (const edge &e) const {
return w > e.w; // Extract min-cost edges first
//Change with < for maximum spanning tree
}
};
//Struct edge pour les flots/matchings
struct Edge {
        int from, to, cap, flow, index;
        Edge(int from, int to, int cap, int flow, int index) :
        from(from), to(to), cap(cap), flow(flow), index(index) {}
};

const int INF = 2000000000;
struct Dinic {
        int N;
        vector<vector<Edge>> G;
        vector<Edge *> dad;
        vector<int> Q;
        Dinic(int N) : N(N), G(N), dad(N), Q(N) {}
        void AddEdge(int from, int to, int cap) {
                G[from].push_back(Edge(from, to, cap, 0, G[to].size()));
                if (from == to) G[from].back().index++;
                G[to].push_back(Edge(to, from, 0, 0, G[from].size() - 1));
        }
        long long BlockingFlow(int s, int t) {
                fill(dad.begin(), dad.end(), (Edge *) NULL);
                dad[s] = &G[0][0] - 1;
                int head = 0, tail = 0;
                Q[tail++] = s;
                while (head < tail) {
                        int x = Q[head++];
                        for (int i = 0; i < G[x].size(); i++) {
                                Edge &e = G[x][i];
                                if (!dad[e.to] && e.cap - e.flow > 0) {
                                        dad[e.to] = &G[x][i];
                                        Q[tail++] = e.to;
                                }
                        }
                }
                if (!dad[t]) return 0;
                long long totflow = 0;
                for (int i = 0; i < G[t].size(); i++) {
                        Edge *start = &G[G[t][i].to][G[t][i].index];
                        int amt = INF;

                        for (Edge *e = start; amt && e != dad[s]; e = dad[e->from]) {
                                if (!e) { amt = 0; break; }
                                amt = min(amt, e->cap - e->flow);
                        }
                        if (amt == 0) continue;
                        for (Edge *e = start; amt && e != dad[s]; e = dad[e->from]) {
                                e->flow += amt;
                                G[e->to][e->index].flow -= amt;
                        }
                        totflow += amt;
                }
                return totflow;
        }
        long long GetMaxFlow(int s, int t) {
                long long totflow = 0;
                while (long long flow = BlockingFlow(s, t))
                        totflow += flow;
                return totflow;
        }
};

int solveOne(int B, int M, int Z)
{
        int n_vertex = 2 + B + 2*M + Z;
        Dinic d(n_vertex);
        // 0 is s, n_vertex -1 is t
        // bzorg are from 1 to b
        // mediators are from b+1 to b + 2*M
        // mstart + 2i is in vertex, mstart + 2i+1 is out vertex
        int mstart = B + 1;
        // zorgs are from b + 2*M+1 to b + 2*M + Z
        int zstart = B + 2*M + 1;
        // b + 2*M + Z + 1 is t
        vector<vector<int>> graph(n_vertex, vector<int>(n_vertex, 0)); 

        // OK links
        for (int i = 1; i <= B; ++i)
                d.AddEdge(0,i,1);
        for (int i = zstart; i < n_vertex - 1; ++i)
                d.AddEdge(i, n_vertex-1, 1);


        int p, sb, sz, bb, zz;
        for (int i = 0; i < M; ++i)
        {
                cin >> p >> sb >> sz;
                d.AddEdge(mstart + 2*i, mstart + 2*i + 1, (p-1)/2);

                for (int j = 0; j < sb; ++j)
                {
                        cin >> bb;
                        // numbers start at 1 !
                        d.AddEdge(bb, mstart + 2*i, 1);
                }
                for (int j = 0; j < sz; ++j)
                {
                        cin >> zz;
                        // numbers start at 1 !
                        d.AddEdge(mstart + 2*i + 1, zstart + zz - 1, 1);
                }
        }

        /*for (int i = 0; i < graph.size(); ++i)
        {
                cout << i << " : ";
                for (int j : graph[i])
                {
                        cout << j << " ";
                }
                cout << endl;
        }*/
        return 2*d.GetMaxFlow(0, n_vertex-1);//*Ford_Fulkerson(graph, 0, n_vertex-1);
}

int main()
{        
        ios::sync_with_stdio(false); // IO "optimization"
        cin.tie(nullptr);

        int b,m,z;
        cin >> b >> m >> z;
        cout << solveOne(b, m, z) << "\n";
        return 0;
}


