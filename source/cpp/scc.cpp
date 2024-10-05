
template<typename T>
vector<vector<int>> strongly_connected_components(const vector<vector<Edge<T>>>& graph){
    const int n = graph.size();
    vector<int> back_number;

    {
        vector<bool> used(n);
        function<void(int)> check_back_number = [&](int v) {
            used[v] = true;
            for(const auto& e : graph[v]){
                if(!used[e.to]){
                    check_back_number(e.to);
                }
            }
            back_number.push_back(v);
        };
        for(int i = 0; i < n; i++){
            if(!used[i]){
                check_back_number(i);
            }
        }
    }

    vector<vector<int>> scc;

    {
        vector<vector<Edge<T>>> reversed_graph(n);
        for(int from = 0; from < n; from++){
            for(const auto& e : graph[from]){
                reversed_graph[e.to].push_back(Edge(from, e.value));
            }
        }
        vector<char> used(n);
        function<void(int, vector<int>&)> collect_nodes = [&](int v, vector<int>& s) {
            used[v] = true;
            s.push_back(v);
            for(const auto& e : reversed_graph[v]){
                if (!used[e.to]) {
                    collect_nodes(e.to,s);
                }
            }
        };

        reverse(back_number.begin(), back_number.end());
        for(int k : back_number){
            if(!used[k]){
                scc.emplace_back(0);
                collect_nodes(k, scc.back());
            }
        }
    }
    return scc;
}

namespace SAT{
    // variable is integer .
    struct Literal{
        bool is_not;
        int var;
        Literal(bool _is_not,int _var)
            : is_not(_is_not),var(_var){}
        Literal not_() const{
            return Literal(not is_not,var);
        }
    };
    Literal make_yes(int var){
        return Literal(false,var);
    }
    Literal make_no(int var){
        return Literal(true,var);
    }
    using Clause = std::vector<Literal>;
    bool solve_2SAT(const int number_of_variable,
                    const vector<Clause>& problem,
                    vector<char>& output){
        // variable is [0..N).
        // "not n" will be assigned to (n+N).
        auto index = [number_of_variable](const bool& is_not,const int& var){
            return number_of_variable*is_not+var;
        };
        auto to_int = [number_of_variable,&index](const Literal& lit){
            return index(lit.is_not,lit.var);
        };

        vector<vector<Edge>> graph(number_of_variable*2);
        for(const Clause& clause : problem){
            // clause = {Literal,Literal}
            // a \/ b -> ~a -> b /\ ~b -> a
            for(int i=0;i<2;i++){
                Literal c = clause[i].not_(),
                        d = clause[not i];
                graph[to_int(c)].push_back(Edge(to_int(d),1));
            }
        }
        vector<vector<int>> scc = strongly_connected_components(graph);
        // where[i] = which component holds i
        vector<int> where(number_of_variable*2);
        for(int i=0;i<scc.size();i++){
            for(int node : scc[i]){
                where[node] = i;
            }
        }
        for(int var=0;var<number_of_variable;var++){
            if(where[index(false,var)] == where[index(true,var)]){
                return false;
            }
        }
        for(int var=0;var<number_of_variable;var++){
            output[var] = where[index(false,var)] < where[index(true,var)];
        }
        return true;
    }
}


// Codeforces 228E
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n,m;
    cin >> n >> m;
    vector<SAT::Clause> problem;
    for(int i=0;i<m;i++){
        int a,b,c;
        cin >> a >> b >> c;
        a--;b--;
        if(c){
            // already
            // ~(A/\~B) /\ ~(~A/\B)
            //  = (~A \/ B) /\ (A \/ ~B)
            problem.push_back({SAT::make_no(a),SAT::make_yes(b)});
            problem.push_back({SAT::make_yes(a),SAT::make_no(b)});
        }else{
            // ~(A/\B) /\ ~(~A/\~B)
            //  = (~A \/ ~B) /\ (A \/ B)
            problem.push_back({SAT::make_no(a),SAT::make_no(b)});
            problem.push_back({SAT::make_yes(a),SAT::make_yes(b)});
        }
    }
    vector<char> output(n);
    bool is_solvable = SAT::solve_2SAT(n,problem,output);
    if(is_solvable){
        cout << count(all(output),true) << endl;
        for(int i=0;i<n;i++){
            if(output[i]){
                cout << i+1 << " ";
            }
        }
        cout << endl;
    }else{
        cout << "Impossible" << endl;
    }

    return 0;
}
