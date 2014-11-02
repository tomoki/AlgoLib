#include <vector>
#include <queue>
#include <iostream>


template<class T,
         class Container=std::vector<T>,
         class Compare=std::less<typename Container::value_type>>
struct lens_queue : public std::priority_queue<T,Container,Compare>{
    typedef std::priority_queue<T,Container,Compare> super;
    lens_queue(int _cap)
        : cap(_cap){
    }
    inline std::size_t capacity() const {return cap;}
    void push(const T& x){
        if(should_push(x)){
            super::push(x);erase_overflow();
        }
    }
    void push(T&& x){
        if(should_push(x)){
            super::push(std::move(x));erase_overflow();
        }
    }
    template <class... Args>
    void emplace(Args&&... args){
        this->c.emplace_back(std::forward<Args>(args)...);
        if(should_push(this->c.back())){
            std::push_heap(this->c.begin(),this->c.end(),this->comp);
            erase_overflow();
        }else{
            this->c.pop_back();
        }
    }
    // this crash lens_queue.
    Container&& take(){
        return std::move(this->c);
    }
    template<class T2,class Container2,class Compare2>
    friend std::ostream& operator<<(std::ostream&,
                                    const lens_queue<T2,Container2,Compare2>&);
private:
    std::size_t cap;
    void erase_overflow(){
        while(this->size() > capacity()){
            this->pop();
        }
    }
    inline bool should_push(const T& x){
        return this->size()<capacity() || !this->comp(this->top(),x);
    }
};

template<class T,class Container,class Compare>
std::ostream& operator<<(std::ostream& os,const lens_queue<T,Container,Compare>& que){
    os << "{";
    for(const auto& v : que.c){
        os << v << ",";
    }
    os << "}";
    return os;
}



signed main() {
    int n,k;char c;
    std::cin >> n >> k >> c;
    if(c == 'g'){
        // k = 3
        // 4 2 3 1 5 -> 3 4 5
        lens_queue<int,std::vector<int>,std::greater<int>> que(k);
        for(int i=0;i<n;i++){
            int v;std::cin >> v;
            que.push(v);
            // que.emplace(v);
        }
        vector<int> taken = que.take();
        // while(not que.empty()){
        //     std::cout << que.top() << std::endl;
        //     que.pop();
        // }
    }else if(c == 'l'){
        // k = 3
        // 4 2 3 1 5 -> 3 2 1
        lens_queue<int> que(k);
        for(int i=0;i<n;i++){
            int v;std::cin >> v;
            que.push(v);
            // que.emplace(v);
        }
        while(not que.empty()){
            std::cout << que.top() << std::endl;
            que.pop();
        }
    }
    return 0;
}
