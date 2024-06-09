#include <iostream>
#include <vector>
#include <set>
#include <map>

using namespace std;

struct Rule{
    char from;
    vector<char> to;
    Rule(char from_,vector<char> to_)
        : from(from_),to(to_){
    }
};

bool operator==(const Rule& lhs, const Rule& rhs){
    if(lhs.from != rhs.from or lhs.to.size() != rhs.to.size()){
        return false;
    }
    for(size_t i=0;i<lhs.to.size();i++){
        if(lhs.to[i] != rhs.to[i]){
            return false;
        }
    }
    return true;
}

bool operator<(const Rule& lhs, const Rule& rhs){
    if(lhs.from != rhs.from){
        return lhs.from < rhs.from;
    }
    if(lhs.to.size() != rhs.to.size()){
        return lhs.to.size() < rhs.to.size();
    }
    for(size_t i=0;i<lhs.to.size();i++){
        if(lhs.to[i] != rhs.to[i]){
            return lhs.to[i] < rhs.to[i];
        }
    }
    return false;
}

bool operator>(const Rule& lhs, const Rule& rhs){
    return not(lhs == rhs) and not(lhs < rhs);
}

ostream& operator<<(ostream& os,Rule r){
    os << r.from << "->";
    for(char c : r.to){
        os << " " << c;
    }
    return os;
}

template<typename T>
ostream& operator<<(ostream& os,set<T> s){
    os << "{";
    for(T v : s){
        os << v << ",";
    }
    os << "}";
    return os;
}

// can be written in recursive descent parsing?
//  see tiger book(p.46).
bool can_be_written_in_recursive_descent_parsing(const set<char>& terminal,
                                                 const set<char>& non_terminal,
                                                 const vector<Rule>& rules){
    for(Rule r : rules){
        cerr << r << endl;
    }
    map<char,bool> nullable;
    map<char,set<char>> first,follow;
    for(char c : non_terminal){
        nullable[c] = false;
        first[c] = {};
        follow[c] = {};
    }
    for(char c : terminal){
        nullable[c] = false;
        first[c] = {c};
        follow = {};
    }

    {
        bool changed;
        do{
            changed = false;
            for(Rule r : rules){
                bool all_nullable = true;
                for(char t : r.to){
                    all_nullable = all_nullable and nullable[t];
                }
                if(all_nullable and not nullable[r.from]){
                    nullable[r.from] = true;
                    changed = true;
                }
                for(size_t i=0;i<r.to.size();i++){
                    bool nul = true;
                    for(size_t k=0;k<i;k++){
                        nul = nul and nullable[r.to[k]];
                    }
                    if(nul){
                        size_t bef = first[r.from].size();
                        first[r.from].insert(first[r.to[i]].begin(),
                                             first[r.to[i]].end());
                        changed = changed or (bef != first[r.from].size());
                    }
                }
                for(size_t i=0;i<r.to.size();i++){
                    // nullable[y[k]] for k <- [i+1,]
                    bool nul = true;
                    for(int k=i+1;k<(int)r.to.size();k++){
                        nul = nul and nullable[r.to[k]];
                    }
                    if(nul){
                        size_t bef = follow[r.to[i]].size();
                        follow[r.to[i]].insert(follow[r.from].begin(),
                                               follow[r.from].end());
                        changed = changed or (bef != follow[r.to[i]].size());
                    }
                }
                for(size_t i=0;i<r.to.size();i++){
                    for(size_t j=i+1;j<r.to.size();j++){
                        // nullable[y[k]] for k <- [i+1,j-1]
                        bool nul = true;
                        for(size_t k=i+1;k<j;k++){
                            nul = nul and nullable[r.to[k]];
                        }
                        if(nul){
                            size_t bef = follow[r.to[i]].size();
                            follow[r.to[i]].insert(first[r.to[j]].begin(),
                                                   first[r.to[j]].end());
                            changed = changed or (bef != follow[r.to[i]].size());
                        }
                    }
                }
            }
        }while(changed);
    }
    for(char c : non_terminal){
        cerr << c << endl
             << "nullable? " << nullable[c] << endl
             << "first " << first[c] << endl
             << "follow " << follow[c] << endl;
    }
    map<char,map<char,set<Rule>>> maps;
    for(Rule r : rules){
        // check r.to is nullable or not.
        //  and collect first.
        set<char> to_first;
        bool to_nullable = true;
        for(char t : r.to){
            to_first.insert(first[t].begin(),first[t].end());
            to_nullable = to_nullable and nullable[t];
            if(not to_nullable) break;
        }

        for(char tf : to_first){
            maps[r.from][tf].insert(r);
        }
        if(to_nullable){
            for(char t : follow[r.from]){
                maps[r.from][t].insert(r);
            }
        }
    }

    cerr << "Map" << endl;
    bool dup = false;
    for(const pair<char,map<char,set<Rule>>> m : maps){
        int from = m.first;
        for(const pair<char,set<Rule>> p : m.second){
            int to = p.first;
            cerr << char(from) << " " << char(to) << " " << p.second << endl;
            dup = dup or p.second.size() > 1;
        }
    }
    return not dup;
}


int main(){
    set<char> terminal;
    set<char> non_terminal;

    {
        // terminal;
        int n;cin >> n;
        for(int i=0;i<n;i++){
            char c;cin >> c;
            terminal.insert(c);
        }
    }

    {
        // non_terminal;
        int n;cin >> n;
        for(int i=0;i<n;i++){
            char c;cin >> c;
            non_terminal.insert(c);
        }
    }

    // number of rule.
    int n;
    cin >> n;
    vector<Rule> rules;
    for(int i=0;i<n;i++){
        int k;cin >> k;
        char from;cin >> from;
        vector<char> to(k);
        for(int j=0;j<k;j++){
            cin >> to[j];
        }
        rules.push_back(Rule(from,to));
    }
    cout << can_be_written_in_recursive_descent_parsing(terminal,non_terminal,rules) << endl;
    return 0;
}

              // 9                  // 9
// 3          // $ + - * / i n ( )  // $ + - * / i n ( )
// a c d      // 3                  // 6
// 3          // S E T              // S E T D U F
// X Y Z      // 10                 // 12
// 6          // 2 S E $            // 2 S E $
// 1 X Y      // 3 E E p T          // 2 E T D
// 1 X a      // 3 E E m T          // 3 D + T D
// 0 Y        // 1 E T              // 3 D - T D
// 1 Y c      // 3 T T * F          // 0 D
// 1 Z d      // 3 T T / F          // 2 T F U
// 3 Z X Y Z  // 1 T F              // 3 U * F U
//  -> 0      // 1 F i              // 3 U / F U
              // 1 F n              // 0 U
              // 3 F ( E )          // 1 F i
              //  -> 0              // 1 F n
                                    // 3 F ( E )
                                    //  -> 1
