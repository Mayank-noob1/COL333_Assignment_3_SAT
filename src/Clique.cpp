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

void generate_degree_clause(int n, int offset, int k, vector<vector<int>>& edges, vector<string>& clauses){
    for (int i = 1; i<=n; i++){
        if (edges[i].size() < k-1){
            auto clause = "-"+to_string(offset+i)+" 0";
            clauses.push_back(clause);
        }
    }
}

void generate_clauses_for_not_connected_vertices(vector<string>& clauses, vector<vector<int> >& edges,bool two_clause=true){  // Generates clauses to ensure clique is subgraph
    int n = edges.size() - 1;
    for(int i=1; i<=n; i++){
        vector<bool> is_present(n+1, false);
        for(auto &vertices : edges[i]){
            is_present[vertices] = true;
        }
        for(int j=i+1; j<=n; j++){
            if (is_present[j]) continue;
            string clause1 = generate_edge_clause(i,j);
            clauses.push_back(clause1);
            if (two_clause){
            string clause2 = generate_edge_clause(i+n, j+n);
            clauses.push_back(clause2);}
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

void find_disjoint_cliques(int n, int m, int k1, int k2, vector<vector<int>>& edges, vector<string>& clauses){  // Generates all remaining clauses for part 1
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

bool find_fixed_size_cliques(int n, int m, int k1, vector<vector<int>>& edges, vector<string> &clauses,const string clause_file_address,const string minisat_file_name,const string output_file_address){  // Generates all remaining clauses for part 1
    int offset=n+1;
    generate_degree_clause(n, 0, k1, edges, clauses);
    generate_clause_with_offset(clauses, n, k1+1, offset, true);

    ofstream ClauseFile(clause_file_address);
    ClauseFile << "p cnf " << (n + (n+1)*(k1+1)) << ' ' << clauses.size() << '\n';
    for (int i = 0; i<clauses.size(); i++){
        ClauseFile << clauses[i];
        if (i != clauses.size()-1) ClauseFile << '\n';
    }
    ClauseFile.close();

    string command = "minisat "+clause_file_address+' '+minisat_file_name + " >> temp/minisat_log.txt";
    system(command.c_str());


    ifstream MiniSatFile(minisat_file_name);
    string satisfy;
    MiniSatFile >> satisfy;
    if (satisfy == "SAT"){
        ofstream OutputFile(output_file_address);
        OutputFile << "#1\n";
        int ct = 0;
        for (int i=1; i<=n; i++){
            int x;
            MiniSatFile >> x;
            if (x > 0){
                OutputFile << i;
                if (ct!=k1-1) OutputFile << ' ';
                ct++;
            }
        }
        MiniSatFile.close();
        OutputFile.close();
        return true;
    }
    else{
        MiniSatFile.close();
        return false;
    }
};

void find_maximal_clique(int n,int m, vector<vector<int> > &edges, vector<string>& clauses,const string clause_file_address,const string minsat_file_name,const string output_file_address){
    // Binary search on k
    ofstream OutputFile(output_file_address);
    OutputFile << "#1\n";
    OutputFile << "1";
    OutputFile.close();
    int low =0, high=n;
    string assignment;
    bool satisfiable = false;
    ofstream Minisatlog("temp/minisat_log.txt");
    Minisatlog.close();
    while (low < high){
        vector<string> newClause (clauses.begin(), clauses.end());
        int k = ((low+high)/2);
        satisfiable = find_fixed_size_cliques(n,m,k,edges,newClause,clause_file_address,minsat_file_name,output_file_address);
        if (!satisfiable){
            high = k;
        }else{
            low = k+1;
        }
    }
    return;
};

void disjoint_clique(const string input_file_address,const string clause_file_address){    // Main function for part1
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
    generate_degree_clause(n, 0, k1, edges, clauses);
    generate_degree_clause(n, n, k2, edges, clauses);
    find_disjoint_cliques(n, m, k1, k2, edges, clauses);

    ofstream ClauseFile(clause_file_address);
    ClauseFile << "p cnf " << (2*n + (n+1)*(k1+1) + (n+1)*(k2+1)) << ' ' << clauses.size() << '\n';
    for (int i = 0; i<clauses.size(); i++){
        ClauseFile << clauses[i];
        if (i != clauses.size()-1) ClauseFile << '\n';
    }
    ClauseFile.close();
}

void maximal_clique(const string input_file_address,const string output_file_address,const string clause_file_address,const string minsat_output_file_address){  // Main function for part2
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

    vector<string> clauses;
    generate_clauses_for_not_connected_vertices(clauses, edges, false);
    find_maximal_clique(n, m, edges, clauses, clause_file_address, minsat_output_file_address, output_file_address);
}

void format_changer_p1(int n, int k1, int k2, const string input_file_address,const string output_file_address){  // Changes format of output for part1
        ifstream MiniSatFile(input_file_address);
        ofstream OutputFile(output_file_address);
        string satisfy;
        MiniSatFile >> satisfy;
        if (satisfy == "SAT"){
            OutputFile << "#1\n";
            int ct = 0;
            for (int i=1; i<=n; i++){
                int x;
                MiniSatFile >> x;
                if (x > 0){
                    OutputFile << i;
                    if (ct!=k1-1) OutputFile << " ";
                    ct++;
                }
            }
            OutputFile << "\n#2\n"; 
            ct = 0;
            for (int i=1; i<=n; i++){
                int x;
                MiniSatFile >> x;
                if (x > 0){
                    OutputFile << i;
                    if (ct!=k2-1) OutputFile << " ";
                    ct++;
                }
            }
        }
        else{
            OutputFile << "0";
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
        int k1 = stoi(argv[5]);
        int k2 = stoi(argv[6]);
        format_changer_p1(n, k1, k2, input_file_address, output_file_address);
    }
    else if (mode == 2){        // run3.sh
        string input_file_address = argv[2];
        string output_file_address = argv[3];
        string clause_file_address = "temp/clause.txt";
        string minsat_output_file_address = "temp/minsat_output.txt";
        maximal_clique(input_file_address, output_file_address, clause_file_address, minsat_output_file_address);
    }
    else cout << "Invalid Mode";
}
