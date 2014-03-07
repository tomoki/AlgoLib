#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define all(c) c.begin(),c.end()
#define repeat(i,n) for(int i=0;i<static_cast<int>(n);i++)

struct Edge{
    int to,cost;
    Edge(int to_,int cost_)
        :to(to_),cost(cost_){
    }
};

int find_bridge_sub(int cur,int from,int number,
                    const vector<vector<Edge>>& graph,
                    vector<int> &pre_order,
                    vector<int> &low,
                    vector<vector<Edge> > &bridges){
    pre_order[cur] = low[cur] = number;
    number++;
    for(const Edge& e : graph[cur]){
        // not visited
        if(pre_order[e.to] == -1){
            number = find_bridge_sub(e.to,cur,number,graph,pre_order,low,bridges);
            if(pre_order[e.to] == low[e.to]){
                bridges[cur].push_back(e);
            }
        }
        if(from != e.to){ //ignore parent.
            low[cur] = min(low[cur],low[e.to]);
        }
    }
    return number;
}

// assume it is connected graph.
vector<vector<Edge>> find_bridge(const vector<vector<Edge>>& graph){
    int N = graph.size();
    vector<int> pre_order(N,-1),lowest_number_of_connected(N,N+1);
    vector<vector<Edge>> ret(N);
    find_bridge_sub(0,-1,0,graph,pre_order,lowest_number_of_connected,ret);

    // optional: make sure from < e.to
    //  You can use following if graph is bidirectional.
    // vector<vector<Edge>> swapped(N);
    // repeat(f,graph.size()){
    //     for(const Edge& e: ret[f]){
    //         if(f < e.to){
    //             swapped[f].push_back(e);
    //         }else{
    //             swapped[e.to].push_back(Edge(f,e.cost));
    //         }
    //     }
    // }
    // ret = swapped;
    return ret;
}


int main(){
    int V_SIZE,E_SIZE;
    cin >> V_SIZE >> E_SIZE;
    vector<vector<Edge>> graph(V_SIZE);
    repeat(i,E_SIZE){
        int s,t;
        cin >> s >> t;
        graph[s].push_back(Edge(t,1));
        graph[t].push_back(Edge(s,1));
    }

    vector<vector<Edge>> bridges = find_bridge(graph);
    repeat(f,graph.size()){
        vector<Edge> &es = bridges[f];
        sort(all(es),[](Edge lhs,Edge rhs){return lhs.to < rhs.to;});
        for(const Edge& e : es){
            cout << f << " " << e.to << endl;
        }
    }
    return 0;
}
