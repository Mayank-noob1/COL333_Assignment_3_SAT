#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h> 

using namespace std;

string generate_edge_clause(int i,int j){
    return "-"+to_string(i)+" "+"-"+to_string(j)+" 0";
}

void generate_disjoint_clause(int i, int j, int k, int n, int offset, vector<string>& clauses, bool first_clique){ // Generates clauses to ensure disjoint Cliques
    if (j == 0){
        auto clause = to_string(offset+j+(i)*k)+" 0";
        clauses.push_back(clause);
        return;
    }
    if (i == 0){
        auto clause = "-"+to_string(offset+j+(i)*k) + " 0";
        clauses.push_back(clause);
        return;
    }
    clauses.push_back("-"+to_string(offset+j+(i)*k)+" "+to_string(offset+j+(i-1)*k)+" "+to_string(offset+(j-1)+(i-1)*k)+" 0");
    clauses.push_back("-"+to_string(offset+j+(i-1)*k)+" "+to_string(offset+j+(i)*k)+" 0");
    if (first_clique){
        clauses.push_back("-"+to_string(offset+j+(i)*k)+" "+to_string(offset+j+(i-1)*k)+" "+to_string(i)+" 0");
        clauses.push_back("-"+to_string(offset+(j-1)+(i-1)*k)+" "+to_string(offset+j+(i)*k)+" -"+to_string(i)+" 0");
    }
    else{
        clauses.push_back("-"+to_string(offset+j+(i)*k)+" "+to_string(offset+j+(i-1)*k)+" "+to_string(i+n)+" 0");
        clauses.push_back("-"+to_string(offset+(j-1)+(i-1)*k)+" "+to_string(offset+j+(i)*k)+" -"+to_string(i+n)+" 0");
    }
}

void generate_clauses_for_not_connected_vertices(vector<string>& clauses, vector<vector<int> >& edges){  // Generates clauses to ensure clique is subgraph
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

void generate_clause_with_offset(vector<string>& clauses, int n, int k, int offset, bool first_clique){ // Generates clauses to ensure disjoint Cliques
    for(int i=0; i<=n; i++){
        for(int j=0; j<k; j++){
            generate_disjoint_clause(i, j, k, n, offset, clauses, first_clique);
        }
    }
    clauses.push_back(to_string(offset+(k-1)+n*k)+" 0");
}

void find_disjoint_cliques(int n, int m, int k1, int k2, vector<vector<int> >& edges, vector<string>& clauses){  // Generates all remaining clauses for part 1
    // Generating Clauses to ensure labels for clique 1 do not interfere with labels for clique 2
    for (int i=1; i<=n; i++){
        string clause = generate_edge_clause(i,i+n);
        clauses.push_back(clause);
    }

    // Generating Clauses to ensure disjoint Cliques
    // State : S[i,k] = S[i-1,k] | (S[i-1,k-1] & X[i])
    // Map : S[i,k] = K1*i + k
    // Domain :  0 <= i <= n, 0 <= k <= K1
    int offset=2*n+1;
    generate_clause_with_offset(clauses, n, k1+1, offset, true);
    offset += (n+1)*(k1+1);
    generate_clause_with_offset(clauses, n, k2+1, offset, false);
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

void disjoint_clique(string input_file_address, string clause_file_address){    // Main function for part1
    ifstream InputFile(input_file_address);
    int n, m, k1, k2;
    InputFile >> n >> m >> k1 >> k2;

    // Input Graph Edges
    vector<vector<int> > edges (n+1,vector<int> (0));
    for(int i=0; i<m; i++){
        int x, y;
        InputFile >> x >> y;
        edges[x].push_back(y);
        edges[y].push_back(x);
    }
    InputFile.close();

    // Create and Insert Clauses
    vector<string> clauses;
    generate_clauses_for_not_connected_vertices(clauses, edges);
    find_disjoint_cliques(n, m, k1, k2, edges, clauses);

    ofstream ClauseFile(clause_file_address);
    ClauseFile << "p cnf " << (2*n + (n+1)*(k1+1) + (n+1)*(k2+1)) << ' ' << clauses.size() << '\n';
    for (auto &clause: clauses) ClauseFile << clause << '\n';
    ClauseFile.close();
}

void maximal_clique(string input_file_address, string output_file_address, string clause_file_address, string minsat_output_file_address){  // Main function for part2
    ifstream InputFile(input_file_address);
    int n, m;
    InputFile >> n >> m;

    // Input Graph Edges
    vector<vector<int> > edges (n+1,vector<int> (0));
    for(int i=0; i<m; i++){
        int x, y;
        InputFile >> x >> y;
        edges[x].push_back(y);
        edges[y].push_back(x);
    }
    InputFile.close();

    /*
    For every minsat, insert clauses in clause_file_address
    Run minsat and get the values in minsat_output_file_address
    Change the format of minsat_output_file_address and store it in output_file_address
    */
}

void format_changer_p1(int n, string input_file_address, string output_file_address){  // Changes format of output for part1
        ifstream MiniSatFile(input_file_address);
        ofstream OutputFile(output_file_address);
        string satisfy;
        MiniSatFile >> satisfy;
        if (satisfy == "SAT"){
            OutputFile << "#1\n";
            for (int i=1; i<=n; i++){
                int x;
                MiniSatFile >> x;
                if (x > 0) OutputFile << i << ' ';
            }
            OutputFile << "\n#2\n"; 
            for (int i=1; i<=n; i++){
                int x;
                MiniSatFile >> x;
                if (x > 0) OutputFile << i << ' ';
            }
            OutputFile << '\n';
        }
        else{
            OutputFile << "0\n";
        }
        MiniSatFile.close();
        OutputFile.close();
}

int main(int argc, char* argv[]){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int mode = stoi(argv[1]);
    if (mode == 0){     // run1.sh
        string input_file_address = argv[2];
        string clause_file_address = argv[3];
        disjoint_clique(input_file_address, clause_file_address);
    }
    else if (mode == 1){        // run2.sh
        string input_file_address = argv[2];
        string output_file_address = argv[3];
        int n = stoi(argv[4]);
        format_changer_p1(n, input_file_address, output_file_address);
    }
    else if (mode == 2){        // run3.sh
        string input_file_address = argv[2];
        string output_file_address = argv[3];
        string clause_file_address = "temp/clause.txt";
        string minsat_output_file_address = "temp/minsat_output.txt";
    }
    else cout << "Invalid Mode";
}
