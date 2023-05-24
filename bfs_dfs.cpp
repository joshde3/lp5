#include <bits/stdc++.h>
#include <omp.h>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

const int N = 10000; 

vector<int> adj[N]; 

void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void add_random_edges(int num_edges) {
    srand(time(NULL));

    for (int i = 0; i < num_edges; i++) {
        int u = rand() % N;
        int v = rand() % N;

        if (u != v) {
            add_edge(u, v);
        }
    }
}

void bfs_parallel(int start) {
    bool visited[N] = { false };
    queue<int> q;
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        #pragma omp parallel for
        for (int i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i];

            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
}

void bfs_sequential(int start) {
    bool visited[N] = { false };
    queue<int> q;
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i];

            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
}

void dfs_parallel(int start) {
    bool visited[N] = { false };
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int u = s.top();
        s.pop();

        if (!visited[u]) {
            visited[u] = true;

            #pragma omp parallel for
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];

                if (!visited[v]) {
                    s.push(v);
                }
            }
        }
    }
}

void dfs_sequential(int start) {
    bool visited[N] = { false };
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int u = s.top();
        s.pop();

        if (!visited[u]) {
            visited[u] = true;

            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];

                if (!visited[v]) {
                    s.push(v);
                }
            }
        }
    }
}

int main() {

    add_random_edges(10000);

    auto start_time_bfs_parallel = high_resolution_clock::now();
    bfs_parallel(0);
    auto end_time_bfs_parallel = high_resolution_clock::now();
    auto time_bfs_parallel = duration_cast<microseconds>(end_time_bfs_parallel - start_time_bfs_parallel).count();

    auto start_time_bfs_sequential = high_resolution_clock::now();
    bfs_sequential(0);
    auto end_time_bfs_sequential = high_resolution_clock::now();
    auto time_bfs_sequential = duration_cast<microseconds>(end_time_bfs_sequential - start_time_bfs_sequential).count();

    auto start_time_dfs_parallel = high_resolution_clock::now();
    dfs_parallel(0);
    auto end_time_dfs_parallel = high_resolution_clock::now();
    auto time_dfs_parallel = duration_cast<microseconds>(end_time_dfs_parallel - start_time_dfs_parallel).count();

    auto start_time_dfs_sequential = high_resolution_clock::now();
    dfs_sequential(0);
    auto end_time_dfs_sequential = high_resolution_clock::now();
    auto time_dfs_sequential = duration_cast<microseconds>(end_time_dfs_sequential - start_time_dfs_sequential).count();

    cout << "Time required for parallel BFS: " << time_bfs_parallel << " microseconds." << endl;
    cout << "Time required for sequential BFS: " << time_bfs_sequential << " microseconds." << endl;
    cout << "Time required for parallel DFS: " << time_dfs_parallel << " microseconds." << endl;
    cout << "Time required for sequential DFS: " << time_dfs_sequential << " microseconds." << endl;

    double speedup_bfs = (double) time_bfs_sequential / (double) time_bfs_parallel;
    cout << "Speedup for BFS: " << speedup_bfs << endl;

    double speedup_dfs = (double) time_dfs_sequential / (double) time_dfs_parallel;
    cout << "Speedup for DFS: " << speedup_dfs << endl;


    return 0;
}