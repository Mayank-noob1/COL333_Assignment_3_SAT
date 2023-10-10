#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

string generate_edge_clause(int i,int j){
    return "-"+to_string(i)+" "+"-"+to_string(j)+" 0";
}

string generate_clause(int i, int j, int k, int offset){
    if (j == 0){
        return to_string(offset+j+(i)*k)+" 0\n";
    }
    if (i == 0){
        return "-"+to_string(offset+j+(i)*k) + " 0\n";
    }
    string clause1 = to_string(i)+" "+to_string(offset+j+(i-1)*k)+" 0\n";
    string clause2 = to_string(offset+j-1+(i-1)*k)+" "+to_string(offset+j+(i-1)*k)+" 0\n";
    return clause1+ clause2;
}

void generate_clauses_for_not_connected_vertices(vector<string>& clauses, vector<vector<int>>& edges){  // Generates clauses to ensure clique is subgraph
    int n = edges.size() - 1;
    for(int i=1; i<=n; i++){
        vector<bool> is_present(n+1, false);
        for(auto &vertices : edges[i]){
            is_present[vertices] = true;
        }
        for(int j=i+1; j<=n; j++){
            if (is_present[j]) continue;
            string clause1 = generate_edge_clause(i,j);
            string clause2 = generate_edge_clause(i+n, j+n);
            clauses.push_back(clause1);
            clauses.push_back(clause2);
        }
    }
};

void generate_clause_with_offset(vector<string> &clauses, int n, int k1, int offset){
    for(int i=0; i<=n; i++){
        for(int j=0; j<=k1; j++){
            string clause = generate_clause(i, j, k1+1, offset);
            clauses.push_back(clause);
        }
    }
}

void find_disjoint_cliques(int n, int m, int k1, int k2, vector<vector<int>>& edges, vector<string>& clauses){  // Generates all remaining clauses for part 1
    // Generating Clauses to ensure labels for clique 1 do not interfere with labels for clique 2
    for (int i=1; i<=n; i++){
        string clause = generate_edge_clause(i,i+n);
        clauses.push_back(clause);
    }

    // Generating Clauses to ensure disjoint Cliques
    // State : S[i,k] = S[i-1,k] | (S[i-1,k-1] & X[i])
    // Map : S[i,k] = K*i + k
    // Domain :  0 <= i <= n, 0 <= k <= K
    int offset=2*n+1;
    generate_clause_with_offset(clauses, n, k1, offset);
    offset += (n+1)*(k1+1);
    generate_clause_with_offset(clauses, n, k2, offset);
    return;
};

void find_maximal_clique(int n,int m, vector<vector<int> > &edges, vector<string>& clauses){
    // Binary search on k
    int low =0,high=n;
    string assignment;
    bool satisfiable=false;
    while (low < high){
        int mid = (low+high/2)>>1;
        if (satisfiable){
            // Store the assignment
            high = mid;
        }else{
            low = mid+1;
        }
    }
    return;
};

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    // Need to think how to take input
    int n, m, k1 = 0, k2 = 0;
    cin >> n >> m >> k1 >> k2;

    // Input Graph Edges
    vector<vector<int>> edges (n+1,vector<int> (0));
    for(int i=0; i<m; i++){
        int x, y;
        cin >> x >> y;
        edges[x].push_back(y);
        edges[y].push_back(x);
    }

    // Print Graph
    for(int i=0; i<edges.size(); i++){
        cout << i << "-> ";
        for(int j=0; j<edges[i].size(); j++) cout << edges[i][j] << ' ';
        cout<<'\n';
    }

    // Create and Insert Clauses
    vector<string> clauses;
    generate_clauses_for_not_connected_vertices(clauses, edges);
    bool findMaximal = false;
    if (findMaximal) find_maximal_clique(n, m, edges, clauses);
    else find_disjoint_cliques(n, m, k1, k2, edges, clauses);

    ofstream ClauseFile("Clauses.txt");
    ClauseFile << "p cnf " << (2*n + (n+1)*(k1+1) + (n+1)*(k2+1)) << ' ' << clauses.size() << '\n';
    for (auto &clause: clauses) ClauseFile << clause << '\n';
    return 0;
}
