#pragma once
#include <vector>
#include <list>
#include <unordered_map>

namespace graph
{

    template <typename T>
    class graph
    {
    public:
        virtual void add(T, T, int = 1) = 0;
        virtual void remove(T, T) = 0;
        virtual std::vector<T> neighbours(T) const = 0;
        virtual int edgeExist(T, T) const = 0;
        virtual void printGraph() const = 0;
        virtual bool empty() const = 0;
    };

    // Список рёбер - каждому ребру соответствуют две его вершины
    template <typename T>
    class simple_graph : public graph<T>
    {
        bool directed;
        struct edge
        {
            int weight;
            T u;
            T v;
            bool directed;
            edge(T u, T v, int w, bool d) : u(u), v(v), weight(w), directed(d) {}
            bool edgeExist(T t, T p)
            {
                if (directed)
                {
                    return u == t && v == p;
                }
                return (u == t && v == p) || (u == p && v == t);
            }
        };

        std::list<edge> edges;

    public:
        simple_graph(bool directed = false) : directed(directed) {};                       // пустой конструктор
        simple_graph(std::initializer_list<std::tuple<T, T, int>>, bool directed = false); // конструктор со списком
        simple_graph(std::initializer_list<std::pair<T, T>>, bool directed = false);       // конструктор со списком 2
        simple_graph(const simple_graph &);                                                // конструктор копирования
        simple_graph(simple_graph &&) noexcept;                                            // move-конструктор
        // операторы присваивания
        simple_graph &operator=(const simple_graph &);
        simple_graph &operator=(simple_graph &&) noexcept;

        void add(T, T, int = 1) override;
        void remove(T, T) override;
        std::vector<T> neighbours(T) const override;
        int edgeExist(T, T) const override;
        void printGraph() const override;

        bool empty() const override;
    };

    /*** Граф на списках рёбер ***/
    // Конструтор на списке
    template <typename T>
    simple_graph<T>::simple_graph(std::initializer_list<std::tuple<T, T, int>> glist, bool directed)
        : directed(directed)
    {
        for (const auto &[u, v, w] : glist)
        {
            this->add(u, v, w);
        }
    }

    // Конструтор на списке 2
    template <typename T>
    simple_graph<T>::simple_graph(std::initializer_list<std::pair<T, T>> glist, bool directed)
        : directed(directed)
    {
        for (const auto &[u, v] : glist)
        {
            this->add(u, v);
        }
    }

    // Конструктор копирования
    template <typename T>
    simple_graph<T>::simple_graph(const simple_graph &other)
    {
        this->directed = other.directed;
        this->edges = other.edges;
    }

    // Конструктор перемещения
    template <typename T>
    simple_graph<T>::simple_graph(simple_graph &&other) noexcept
    {
        this->directed = other.directed;
        this->edges = other.edges;
        other.edges.clear();
    }

    // Оператор присваивания-копирования
    template <typename T>
    simple_graph<T> &simple_graph<T>::operator=(const simple_graph &other)
    {
        this->directed = other.directed;
        this->edges = other.edges;
        return *this;
    }

    // Оператор присваивания-перемещения
    template <typename T>
    simple_graph<T> &simple_graph<T>::operator=(simple_graph &&other) noexcept
    {
        this->directed = other.directed;
        this->edges = other.edges;
        other.edges.clear();
        return *this;
    }

    // Проверить существует ли ребро (возвращает вес ребра)
    template <typename T>
    int simple_graph<T>::edgeExist(T f, T s) const
    {
        for (auto e : edges)
        {
            if (e.edgeExist(f, s))
            {
                return e.weight;
            }
        }
        return 0;
    }

    // Добавить ребро
    template <typename T>
    void simple_graph<T>::add(T u, T v, int w)
    {
        if (!edgeExist(u, v))
        {
            edges.push_back({u, v, w, this->directed});
        }
    }

    // Удалить ребро
    template <typename T>
    void simple_graph<T>::remove(T u, T v)
    {
        for (auto e = edges.begin(); e != edges.end(); e++)
        {
            if (e->edgeExist(u, v))
            {
                edges.erase(e);
                return;
            }
        }
    }

    // Получить соседей вершины
    template <typename T>
    std::vector<T> simple_graph<T>::neighbours(T v) const
    {
        std::vector<T> vertex;
        if (directed)
        {
            for (auto e : edges)
            {
                if (e.u == v)
                {
                    vertex.push_back(e.v);
                }
            }
        }
        else
        {
            for (auto e : edges)
            {
                if (e.u == v)
                {
                    vertex.push_back(e.v);
                }
                else if (e.v == v)
                {
                    vertex.push_back(e.u);
                }
            }
        }
        return vertex;
    }

    // Проверка на пустоту
    template <typename T>
    inline bool simple_graph<T>::empty() const
    {
        return edges.empty();
    }

    // Напечатать граф
    template <typename T>
    void simple_graph<T>::printGraph() const
    {
        for (auto e : edges)
        {
            std::cout << e.u << " ";
            if (directed)
            {
                if (e.weight)
                {
                    std::cout << "-(" << e.weight << ")-> ";
                }
                else
                {
                    std::cout << "--> ";
                }
            }
            else
            {
                if (e.weight)
                {
                    std::cout << "-(" << e.weight << ")- ";
                }
                else
                {
                    std::cout << "-- ";
                }
            }
            std::cout << e.v << std::endl;
        }
    }

    /*** Граф в виде матрицы смежности - строки и столбцы это вершины графа, а значения матрицы это вес рёбер ***/
    template <typename T>
    class matrix_graph : public graph<T>
    {
        bool directed;
        template <typename K>
        class matrix
        {

            using cell = std::pair<K, K>;

            struct Hashik
            {
                std::size_t operator()(const cell &key) const noexcept
                {
                    std::size_t prime = 16777619;
                    std::size_t hash = 2166136261;

                    hash ^= std::hash<K>{}(key.first);
                    hash *= prime;

                    hash ^= std::hash<K>{}(key.second);
                    hash *= prime;

                    return hash;
                }
            };

            struct row
            {
            private:
                K cur_row;
                matrix<K> &m;

            public:
                row(matrix<K> &m, K row) : m(m), cur_row(row) {}
                int &operator[](const K &col)
                {
                    return m(cur_row, col);
                }
            };

            struct const_row
            {
            private:
                K cur_row;
                const matrix<K> &m;

            public:
                const_row(const matrix<K> &m, K row) : m(m), cur_row(row) {}
                const int operator[](const K &col) const
                {
                    return m(cur_row, col);
                }
            };

            std::unordered_map<cell, int, Hashik> mat;

        public:
            matrix &operator=(const matrix &other)
            {
                this->mat = other.mat;
                return *this;
            }

            int &operator()(const K &row, const K &col)
            {
                return mat[{row, col}];
            }
            const int operator()(const K &row, const K &col) const
            {
                try
                {
                    return mat.at({row, col});
                }
                catch (std::out_of_range &e)
                {
                    return 0;
                }
            }
            row operator[](const K &row)
            {
                return matrix<K>::row(*this, row);
            }
            const const_row operator[](const K &row) const
            {
                return const_row(*this, row);
            }

            bool empty() const
            {
                return mat.empty();
            }
            void remove(const K &row, const K &col)
            {
                mat.erase({row, col});
            }
            void clear()
            {
                mat.clear();
            }

            // Итератор для внутренний map
            using Map = std::unordered_map<cell, int, Hashik>;
            using iterator = typename Map::iterator;
            using const_iterator = typename Map::const_iterator;

            iterator begin() noexcept { return mat.begin(); }
            iterator end() noexcept { return mat.end(); }

            const_iterator begin() const noexcept { return mat.begin(); }
            const_iterator end() const noexcept { return mat.end(); }

            const_iterator cbegin() const noexcept { return mat.cbegin(); }
            const_iterator cend() const noexcept { return mat.cend(); }
        };

        matrix<T> graph;

    public:
        matrix_graph(bool d = false) : directed(d) {}
        matrix_graph(std::initializer_list<std::pair<T, T>>, bool = false);
        matrix_graph(std::initializer_list<std::tuple<T, T, int>>, bool = false);
        matrix_graph(const matrix_graph &);
        matrix_graph(matrix_graph &&) noexcept;

        matrix_graph &operator=(const matrix_graph &);
        matrix_graph &operator=(matrix_graph &&) noexcept;

        void add(T, T, int = 1) override;
        void remove(T, T) override;
        std::vector<T> neighbours(T) const override;
        int edgeExist(T, T) const override;
        void printGraph() const override;

        bool empty() const override;
    };

    /*** Методы графа на матрице смежности ***/
    // Конструктор списКОК
    template <typename T>
    matrix_graph<T>::matrix_graph(std::initializer_list<std::pair<T, T>> list, bool d) : directed(d)
    {
        for (auto &[u, v] : list)
        {
            this->add(u, v);
        }
    }

    // Конструктор список 2
    template <typename T>
    matrix_graph<T>::matrix_graph(std::initializer_list<std::tuple<T, T, int>> list, bool d) : directed(d)
    {
        for (auto &[u, v, w] : list)
        {
            this->add(u, v, w);
        }
    }

    // Конструктор копирования
    template <typename T>
    matrix_graph<T>::matrix_graph(const matrix_graph &other)
    {
        this->directed = other.directed;
        this->graph = other.graph;
    }

    // Конструктор перемещения
    template <typename T>
    matrix_graph<T>::matrix_graph(matrix_graph &&other) noexcept
    {
        this->directed = other.directed;
        this->graph = other.graph;
        other.graph.clear();
    }

    // Оператор присваивания
    template <typename T>
    matrix_graph<T> &matrix_graph<T>::operator=(const matrix_graph &other)
    {
        this->directed = other.directed;
        this->graph = other.graph;
        return *this;
    }

    // Оператор присваивания-перемещения
    template <typename T>
    matrix_graph<T> &matrix_graph<T>::operator=(matrix_graph &&other) noexcept
    {
        this->directed = other.directed;
        this->graph = other.graph;
        other.graph.clear();
        return *this;
    }

    // Добавить ребро
    template <typename T>
    void matrix_graph<T>::add(T u, T v, int w)
    {
        if (!this->edgeExist(u, v))
            graph[u][v] = w;
    }

    // Удалить ребро
    template <typename T>
    void matrix_graph<T>::remove(T u, T v)
    {
        graph.remove(u, v);
        if (!directed)
            graph.remove(v, u);
    }

    // Проверка на существование ребра
    template <typename T>
    int matrix_graph<T>::edgeExist(T u, T v) const
    {
        if (!directed)
        {
            int w = graph[u][v];
            if (!w)
                return graph[v][u];
            return w;
        }
        return graph[u][v];
    }

    // Соседи ребра
    template <typename T>
    std::vector<T> matrix_graph<T>::neighbours(T v) const
    {
        std::vector<T> n;
        if (directed)
        {
            for (auto &[cell, weight] : graph)
            {
                if (weight)
                {
                    if (cell.first == v)
                    {
                        n.push_back(cell.second);
                    }
                }
            }
        }
        else
        {
            for (auto &[cell, weight] : graph)
            {
                if (weight)
                {
                    if (cell.first == v)
                    {
                        n.push_back(cell.second);
                    }
                    else if (cell.second == v)
                    {
                        n.push_back(cell.first);
                    }
                }
            }
        }
        return n;
    }

    // Вывести граф
    template <typename T>
    void matrix_graph<T>::printGraph() const
    {
        for (auto &[cell, w] : graph)
        {
            if (w)
            {
                std::cout << cell.first << " ";
                if (directed)
                {
                    std::cout << "-(" << w << ")-> ";
                }
                else
                {
                    std::cout << "-(" << w << ")- ";
                }
                std::cout << cell.second << std::endl;
            }
        }
    }

    // Проверка на пустоту
    template <typename T>
    bool matrix_graph<T>::empty() const
    {
        return graph.empty();
    }

    /*** Граф на списке смежности - каждой вершине соответствует коллекция её соседей ***/
    template <typename T>
    class list_graph : public graph<T>
    {
        bool directed;
        std::unordered_map<T, std::list<std::pair<T, int>>> graph;

    public:
        list_graph(bool d = false) : directed(d) {}
        list_graph(std::initializer_list<std::pair<T, T>>, bool = false);
        list_graph(std::initializer_list<std::tuple<T, T, int>>, bool = false);
        list_graph(const list_graph &);
        list_graph(list_graph &&) noexcept;

        list_graph &operator=(const list_graph &);
        list_graph &operator=(list_graph &&) noexcept;

        void add(T, T, int = 1) override;
        void remove(T, T) override;
        std::vector<T> neighbours(T) const override;
        int edgeExist(T, T) const override;
        void printGraph() const override;

        bool empty() const override;
    };

    /*** Методы графа на основе списка смежности ***/
    // Конструктор списка
    template <class T>
    list_graph<T>::list_graph(std::initializer_list<std::pair<T, T>> list, bool d) : directed(d)
    {
        for (const auto &[u, v] : list)
        {
            this->add(u, v);
        }
    }

    // Конструктор списка 2
    template <class T>
    list_graph<T>::list_graph(std::initializer_list<std::tuple<T, T, int>> list, bool d) : directed(d)
    {
        for (const auto &[u, v, w] : list)
        {
            this->add(u, v, w);
        }
    }

    // Конструктор копирования
    template <class T>
    list_graph<T>::list_graph(const list_graph &other)
    {
        this->graph = other.graph;
    }

    // Конструктор move-копирования
    template <class T>
    list_graph<T>::list_graph(list_graph &&other) noexcept
    {
        this->graph = other.graph;
        other.graph.clear();
    }

    // Оператор присваивания-копирования
    template <typename T>
    list_graph<T> &list_graph<T>::operator=(const list_graph &other)
    {
        this->graph = other.graph;
        return *this;
    }

    // Оператор присваивания-move
    template <typename T>
    list_graph<T> &list_graph<T>::operator=(list_graph &&other) noexcept
    {
        this->graph = other.graph;
        other.graph.clear();
        return *this;
    }

    // Проверить существование связи
    template <class T>
    int list_graph<T>::edgeExist(T u, T v) const
    {
        try
        {
            const std::list<std::pair<T, int>> &vs = graph.at(u);
            for (const auto &[p, t] : vs)
            {
                if (p == v)
                {
                    return t;
                }
            }
            if (!directed)
            {
                const auto &vs1 = graph.at(v);
                for (const auto &[p, t] : vs)
                {
                    if (p == u)
                    {
                        return t;
                    }
                }
            }
        }
        catch (std::exception &e)
        {
            return 0;
        }
        return 0;
    }

    // Добавить связь
    template <class T>
    void list_graph<T>::add(T u, T v, int w)
    {
        if (!this->edgeExist(u, v))
        {
            graph[u].push_back({v, w});
            if (!directed)
            {
                for (auto &[cur_vertex, list] : graph)
                {
                    for (auto &[check_vertex, weight] : list)
                    { // p - это list, d это v, k это weight
                        if (check_vertex == v)
                        {
                            bool already_have = false;
                            for (auto &[double_v, w] : graph[v])
                            { // Проверить не добавляли ли мы эту вершину уже
                                if (double_v == cur_vertex)
                                {
                                    already_have = true;
                                    break;
                                }
                            }
                            if (!already_have)
                            {
                                graph[v].push_back({cur_vertex, weight});
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    // Получить соседей вершины
    template <class T>
    std::vector<T> list_graph<T>::neighbours(T u) const
    {
        std::vector<T> n;
        try
        {
            const std::list<std::pair<T, int>> &v = graph.at(u);
            for (const auto &[p, t] : v)
            {
                if (t)
                {
                    n.push_back(p);
                }
            }
        }
        catch (std::exception &e)
        {
            return {};
        }
        return n;
    }

    // Удалить связь
    template <class T>
    void list_graph<T>::remove(T u, T v)
    {
        if (this->edgeExist(u, v))
        {
            std::list<std::pair<T, int>> &vs = graph[u];
            for (auto it = vs.begin(); it != vs.end(); ++it)
            {
                if (it->first == v)
                {
                    vs.erase(it);
                    break;
                }
            }
            if (!directed)
            {
                std::list<std::pair<T, int>> &nvs = graph[v];
                for (auto it = nvs.begin(); it != nvs.end(); ++it)
                {
                    if (it->first == u)
                    {
                        nvs.erase(it);
                        break;
                    }
                }
                if (nvs.empty())
                {
                    graph.erase(v);
                }
            }
            if (vs.empty())
            {
                graph.erase(u);
            }
        }
    }

    // Напечатать граф
    template <class T>
    void list_graph<T>::printGraph() const
    {
        for (const auto &[v, vect] : graph)
        {
            for (const auto &[u, w] : vect)
            {
                std::cout << v;
                std::cout << " -(" << w << ")-";
                if (directed)
                {
                    std::cout << '>';
                }
                std::cout << ' ' << u << std::endl;
            }
        }
    }

    // Проверка на пустоту
    template <class T>
    bool list_graph<T>::empty() const
    {
        return graph.empty();
    }

    /*** Граф матрицей инцендетности - столбцы рёбра, а строки вершины***/
    template <typename T>
    class strange_graph : public graph<T>
    {

    public:
        void add(T, T, int = 1) = 0;
        void remove(T, T) = 0;
        std::vector<T> neighbours(T) const = 0;
        int edgeExist(T, T) const = 0;
        void printGraph() const = 0;
        bool empty() const = 0;
    };

    /*** Методы графа на матрице инцендентности ***/

}