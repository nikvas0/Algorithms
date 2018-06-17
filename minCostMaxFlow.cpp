#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <functional>
#include <numeric>
#include <vector>
#include <queue>


const int MAXN = 210;
const int MAXM = 2010;
const int MAXK = 110;

struct Edge {
    int id;
    int from;
    int to;
    int t;
    int c;
    int f;
    Edge* pair;

    Edge() {}

    Edge(int id, int from, int to, int t=0, int c=0, int f=0) : 
        id(id), from(from), to(to), t(t), c(c), f(f), pair(nullptr) {}
};

struct Graph {
    std::vector<Edge> edges;
    std::vector<std::vector<Edge*>> vert;
    int n;

    Graph(int n=0, int m=0) : n(n) {
        vert.resize(n, std::vector<Edge*>(0,nullptr));
        edges.reserve(m);
    }

    void pushEdge(const Edge& e) {
        edges.push_back(e);
        vert[e.from].push_back(&edges.back());
    }
};

void addEdge(Graph& g, int id, int from, int to, int time, int c) {
    Edge e(id, from, to, time, c);
    Edge eBack(id, to, from, -time, 0);
    g.pushEdge(e);
    g.pushEdge(eBack);

    g.edges[g.edges.size() - 1].pair = &g.edges[g.edges.size() - 2];
    g.edges[g.edges.size() - 2].pair = &g.edges[g.edges.size() - 1];
}

std::vector<int> findPotentials(const Graph& g, int start) {
    std::vector<int> dist(g.n, std::numeric_limits<int>::max());
    dist[start] = 0;
    for (int i = 0; i < g.n; ++i) {
        for (const Edge& e : g.edges) {
            if (e.f < e.c && dist[e.to] > static_cast<long long>(dist[e.from]) + e.t) {
                dist[e.to] = static_cast<long long>(dist[e.from]) + e.t;
            }
        }
    }
    return dist;
}

std::vector<Edge*> findMinPath(Graph& g, std::vector<int>& pot, int start, int finish) {
    std::vector<int> dist(g.n, std::numeric_limits<int>::max());
    std::vector<Edge*> pr(g.n, nullptr);

    dist[start] = 0;

    std::priority_queue<std::pair<int, int>> pq;
    pq.push({-dist[start], start});

    while (!pq.empty()) {
        auto cur = pq.top();
        int curDist = -cur.first;
        int v = cur.second;

        pq.pop();

        if (curDist > dist[v]) {
            continue;
        }

        for (Edge* e : g.vert[v]) {
            if (e->f < e->c && dist[e->to] > static_cast<long long>(dist[e->from]) + e->t + pot[e->from] - pot[e->to]) {
                dist[e->to] = static_cast<long long>(dist[e->from]) + e->t + pot[e->from] - pot[e->to];
                pr[e->to] = e;
                pq.push({-dist[e->to], e->to});
            }
        }
    }

    for (int i = 0; i < g.n; ++i) {
        if (dist[i] == std::numeric_limits<int>::max()) {
            continue;
        }
        pot[i] += dist[i];
    }

    if (pr[finish] == nullptr) {
        return {};
    }

    std::vector<Edge*> path;
    int v = finish;
    while (v != start) {
        path.push_back(pr[v]);
        v = pr[v]->from;
    }
    reverse(path.begin(), path.end());
    return path;
}

void findFlow(Graph& g, int start, int finish) {
    std::vector<int> potentials = findPotentials(g, start);

    while (true) {
        std::vector<Edge*> path = findMinPath(g, potentials, start, finish);

        if (path.empty()) {
            break;
        }

        int minUp = std::numeric_limits<int>::max();
        for (Edge* e : path) {
            minUp = std::min(minUp, e->c - e->f);
        }

        for (Edge* e : path) {
            e->f += minUp;
            e->pair->f -= minUp;
        }
    }
}

std::vector<Edge*> nextPath(Graph& g, int start, int finish) {
    std::vector<Edge*> pr(g.n, nullptr);
    std::vector<bool> used(g.n, false);
    
    std::function<void(int)> dfs = [&](int v) {
        used[v] = true;

        for (Edge* edge : g.vert[v]) {
            if (!used[edge->to] && edge->f > 0) {
                pr[edge->to] = edge;
                dfs(edge->to);
            }
        }
    };

    dfs(start);

    int mnFlow = std::numeric_limits<int>::max();
    std::vector<Edge*> path;

    if (pr[finish] == nullptr) {
        return {};
    }
    int v = finish;
    while (v != start) {
        path.push_back(pr[v]);
        mnFlow = std::min(mnFlow, path.back()->f);
        v = path.back()->from;
    }

    for (Edge* e : path) {
        e->f -= mnFlow;
        e->pair->f += mnFlow;
    }

    reverse(path.begin(), path.end());

    return path;
}

long long pathTime(const std::vector<Edge*> path) {
    return std::accumulate(
        path.begin(), 
        path.end(), 
        static_cast<long long>(0), 
        [](long long& cur, Edge* const e) { 
            return cur + e->t; 
        }
    );
}

int main() {
    std::cout.precision(50);

    int n, m, k;
    std::cin >> n >> m >> k;
    Graph g(MAXN, 4 * MAXM);


    for (int i = 0; i < m; ++i) {
        int from, to, time;
        std::cin >> from >> to >> time;
        --from;
        --to;
        addEdge(g, i, from, to, time, 1);
        addEdge(g, i, to, from, time, 1);
    }

    int start = n;
    addEdge(g, -1, start, 0, 0, k);
    int finish = n + 1;
    addEdge(g, -1, n - 1, finish, 0, k);



    findFlow(g, start, finish);
    if (g.vert[start].front()->f < g.vert[start].front()->c) {
        std::cout << "-1\n";
        return 0;
    }

    long double time = 0;
    std::vector<std::vector<Edge*>> pathes;
    for (int i = 0; i < k; ++i) {
        pathes.push_back(nextPath(g, start, finish));
        time += pathTime(pathes.back());
    }


    std::cout << time / k << "\n";

    for (const std::vector<Edge*> path : pathes) {
        std::cout << path.size() - 2 << " ";
        for (int i = 1; i + 1 < path.size(); ++i) {
            std::cout << path[i]->id + 1 << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
