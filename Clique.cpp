#include<iostream>
#include<vector>
using namespace std;
typedef long long ll;



string generate_edge_clause(int i,int j) const{
    string clause = "-"+to_string(i)+" "+"-"+to_string(j)+" 0";
    return clause;
};

string generate_clause(int i,int k,int K,int offset) const{
    if (i == 0){
        return "-1"+to_string(offset+k+(i)*K) + " 0";
    }
    if (k == 0){
        return to_string(offset+k+(i)*K)+" 0";
    }
    string clause1 = to_string(i)+" "+to_string(offset+k+(i-1)*K)+" 0";
    string clause2 = to_string(offset+k-1+(i-1)*K)+" "+to_string(offset+k+(i-1)*K)+" 0";
    return clause;
}

void generate_clauses_for_not_connected_vertices(vector<string> &clauses,vector<vector<int> > &edges){
    int n=edges.size();
    for(int i=0;i<n;i++){
        bool is_present[n];
        for(auto &vertices : edges[i]){
            is_present[edges[i]-1] = true;
        }
        for(int j=0;j<n;j++){
            if (is_present[j]){continue;}
            string clause = generate_edge_clause(i+1,j+1);
            clauses.push_back(clause);
        }
    }
};

void sort_vertices_according_to_degrees(vector<vector<int> > &edges){
    sort(edges.begin(),edges.end(),
            [&](vector<int>left,vector<int> right){
                return left.size() < right.size();});
};

void find_disjoint_cliques(int n,int m, int k1, int k2,vector<vector<int> > &edges,vector<string>& clauses){
    for (int i=1;i<=n;i++){
        string clause = generate_edge_clause(i,i+n);
        clauses.push_back(clause);
    }

    // State : S[i,k] = S[i-1,k] | (S[i-1,k-1] & X[i])
    // Map : S[i,k] = K*i + k
    // Domain :  0 <= i <= n, 0 <= k <= K
    int offset=2*n+1;
    for(int i = 0;i <= n;i++){
        for(int k = 0;k<=k1;k++){
            string clause = generate_clause(i,k,k1,offset);
            clauses.push_back(clause);
        }
    }
    offset += n*k1;
    for(int i = 0;i <= n;i++){
        for(int k = 0;k<=k2;k++){
            string clause = generate_clause(i,k,k2,offset);
            clauses.push_back(clause);
        }
    }
    return;
};

void find_maximal_clique(int n,int m, vector<vector<int> > &edges, vector<string>& clauses){

};


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    // Need to think how to take input
    int n=0,m=0,k1=0,k2=0;cin>>n>>m>>k1>>k2;
    vector<vector<int> > edges;


    vector<string> clauses;
    generate_clauses_for_not_connected_vertices(clauses,edges);
    bool findMaximal = false;

    // sort_vertices_according_to_degrees(edges);
    if (findMaximal){
        find_maximal_clique(n,m,edges,clauses);
    }
    else{
        find_disjoint_cliques(n,m,k1,k2,edges,clauses);
    }

    return 0;
}