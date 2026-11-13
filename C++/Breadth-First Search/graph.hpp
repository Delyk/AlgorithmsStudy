#pragma once
#include <vector>
#include <list>

namespace graph {
//Список рёбер - каждому ребру соответствуют две его вершины
template<typename T>
class simple_graph {
    bool directed;
    struct edge {
        int weight;
        T u;
        T v;
        bool directed;
        edge(T u, T v, int w, bool d) : u(u), v(v), weight(w), directed(d) {}
        bool edgeExist(T t, T p) {
            if(directed) {
                return u == t && v == p;
            }
            return (u == t && v == p) || (u == p && v == t);
        }
    };

    std::list<edge> edges;
    
public:
    simple_graph(bool directed = false) : directed(directed) {}; //пустой конструктор
    simple_graph(std::initializer_list<std::tuple<T,T,int>>, bool directed = false) ;//конструктор со списком
    simple_graph(std::initializer_list<std::pair<T, T>>, bool directed = false) ;//конструктор со списком 2
    simple_graph(const simple_graph&); //конструктор копирования
    simple_graph(simple_graph&&) noexcept; //move-конструктор
    //операторы присваивания
    simple_graph& operator=(const simple_graph&);
    simple_graph& operator=(simple_graph&&) noexcept;
    
    void add(T, T, int = 1);
    void remove(T, T);
    std::vector<T> neighbours(T) const;
    int edgeExist(T, T) const;
    void printGraph() const;

    bool empty() const;
};

/*** Граф на списках рёбер ***/
//Конструтор на списке
template<typename T>
simple_graph<T>::simple_graph(std::initializer_list<std::tuple<T,T,int>> glist, bool directed) 
: directed(directed) {
    for(const auto& [u, v, w] : glist) {
        this->add(u, v, w);
    }
}

//Конструтор на списке 2
template<typename T>
simple_graph<T>::simple_graph(std::initializer_list<std::pair<T, T>> glist, bool directed) 
: directed(directed) {
    for(const auto& [u, v] : glist) {
        this->add(u, v);
    }
}

//Конструктор копирования
template<typename T>
simple_graph<T>::simple_graph(const simple_graph& other) {
    this->directed = other.directed;
    this->edges = other.edges;
}

//Конструктор перемещения
template<typename T>
simple_graph<T>::simple_graph(simple_graph&& other) noexcept{
    this->directed = other.directed;
    this->edges = other.edges;
    other.edges.clear();
}

//Оператор присваивания-копирования
template<typename T>
simple_graph<T>& simple_graph<T>::operator=(const simple_graph& other) {
    this->directed = other.directed;
    this->edges = other.edges;
    return *this;
}

//Оператор присваивания-перемещения
template<typename T>
simple_graph<T>& simple_graph<T>::operator=(simple_graph&& other) noexcept{
    this->directed = other.directed;
    this->edges = other.edges;
    other.edges.clear();
    return *this;
}

//Проверить существует ли ребро (возвращает вес ребра)
template<typename T>
int simple_graph<T>::edgeExist(T f, T s) const {
    for(auto e : edges) {
        if(e.edgeExist(f, s)) {
            return e.weight;
        }
    }
    return 0;
}



// Добавить ребро
template<typename T>
void simple_graph<T>::add(T u, T v, int w) {
    if(!edgeExist(u, v)) {
        edges.push_back({u, v, w, this->directed});
    }
}

// Удалить ребро
template<typename T>
void simple_graph<T>::remove(T u, T v) {
    for(auto e = edges.begin(); e != edges.end(); e++) {
        if(e->edgeExist(u, v)) {
            edges.erase(e);
            return;
        }
    }
}

// Получить соседей вершины
template<typename T>
std::vector<T> simple_graph<T>::neighbours(T v) const {
    std::vector<T> vertex;
    if(directed) {
        for(auto e : edges) {
            if(e.u == v) {
                vertex.push_back(e.v);
            }
        }
    } else {
        for(auto e : edges) {
            if(e.u == v) {
                vertex.push_back(e.v);
            } else if(e.v == v) {
                vertex.push_back(e.u);
            }
        }
    }
    return vertex;
}

//Проверка на пустоту
template <typename T>
inline bool simple_graph<T>::empty() const
{
    return edges.empty();
}

//Напечатать граф
template <typename T>
void simple_graph<T>::printGraph() const {
    for(auto e : edges) {
        std::cout << e.u << " ";
        if(directed) {
            if(e.weight) {
                std::cout << "-(" << e.weight << ")-> ";
            }
            else {
            std::cout << "--> ";
            }
        } else {
            if(e.weight) {
                std::cout << "-(" << e.weight << ")- ";
            }
            else {
            std::cout << "-- ";
            }
        }
        std::cout << e.v << std::endl;
    }
}

}