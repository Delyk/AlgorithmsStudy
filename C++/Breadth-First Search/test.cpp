#include <gtest/gtest.h>
#include <vector>
#include <unordered_set>
#include "graph.hpp"
using namespace std;
using namespace graph;

template <class Graph>
class GraphTest : public ::testing::Test {
protected:
    Graph def;
    Graph gNoDir{
        {1, 2},
        {2, 3}
    };

    Graph gNoDirHard {
        {1, 3},
        {3, 4},
        {3, 1},
        {4, 2},
        {2, 3},
        {3, 5},
        {5, 4}
    };

    Graph gDir;
    Graph gDirHard;
    
    GraphTest() : gDir({
        {1, 2, 4},
        {2, 3, -1},
        {3, 1, 5}
    }, true), 
    gDirHard({
        {3, 1, 2},
        {3, 2, 8},
        {4, 2, 3},
        {4, 3, 4},
        {4, 5, -5},
        {5, 3, 1},
        {3, 1, -100}
    }, true)
    {
    }
};

using Graphs = ::testing::Types<simple_graph<int>>;

TYPED_TEST_SUITE(GraphTest, Graphs);

//Конструктор копирования
TYPED_TEST(GraphTest, CopyContructor) {
    TypeParam newG(this->gNoDir);
    vector<int> check{1, 3};
    vector<int> n = newG.neighbours(2);
    ASSERT_TRUE(check.size() == n.size());
    for(int i = 0; i < n.size(); i++) {
        EXPECT_EQ(check[i], n[i]);
    }
}

//Оператор присваивания-копирования
TYPED_TEST(GraphTest, AssCopy) {
    TypeParam newG;
    newG = this->gNoDir;
    vector<int> check{1, 3};
    vector<int> n = newG.neighbours(2);
    ASSERT_TRUE(check.size() == n.size());
    for(int i = 0; i < n.size(); i++) {
        EXPECT_EQ(check[i], n[i]);
    }
}

//Конструктор move-копирования
TYPED_TEST(GraphTest, MoveContructor) {
    TypeParam newG(move(this->gNoDir));
    ASSERT_TRUE(gNoDir.empty());
    vector<int> check{1, 3};
    vector<int> n = newG.neighbours(2);
    ASSERT_TRUE(check.size() == n.size());
    for(int i = 0; i < n.size(); i++) {
        EXPECT_EQ(check[i], n[i]);
    }
}

//Оператор присваивания-move
TYPED_TEST(GraphTest, AssMove) {
    TypeParam newG;
    newG = move(this->gNoDir);
    ASSERT_TRUE(gNoDir.empty());
    vector<int> check{1, 3};
    vector<int> n = newG.neighbours(2);
    ASSERT_TRUE(check.size() == n.size());
    for(int i = 0; i < n.size(); i++) {
        EXPECT_EQ(check[i], n[i]);
    }
}

TYPED_TEST(GraphTest, SimpleGraph) {
    this->def.add(1, 2);
    this->def.add(2, 3);
    vector<int> check{1, 3};
    vector<int> n = this->def.neighbours(2);
    ASSERT_TRUE(check.size() == n.size());
    for(int i = 0; i < n.size(); i++) {
        EXPECT_EQ(check[i], n[i]);
    }
}

//Проверка на существование ребра в неориентированном графе
TYPED_TEST(GraphTest, CheckEdgeNoDir) {
    EXPECT_TRUE(this->gNoDir.edgeExist(2, 3));
    EXPECT_FALSE(this->gNoDir.edgeExist(3, 1));
}

//Проверка соседей неориентированный граф
TYPED_TEST(GraphTest, CheckNeighboursNoDir) {
    unordered_set<int> check{1, 2, 4, 5};
    vector<int> n = this->gNoDirHard.neighbours(3);
    ASSERT_TRUE(n.size() == check.size());
    for(auto e : n) {
        EXPECT_TRUE(check.count(e));
    }
}

//Удаление рёбер неориентированный граф
TYPED_TEST(GraphTest, RemoveEdgesNoDir) {
    this->gNoDirHard.remove(3, 2);
    this->gNoDirHard.remove(4, 2);
    this->gNoDirHard.remove(3, 1);
    unordered_set<int> check{5, 4};
    vector<int> n = this->gNoDirHard.neighbours(3);
    ASSERT_TRUE(n.size() == check.size());
    for(auto e : n) {
        EXPECT_TRUE(check.count(e));
    }
    this->gNoDirHard.remove(3, 4);
    this->gNoDirHard.remove(3, 5);
    this->gNoDirHard.remove(5, 4);
    EXPECT_TRUE(this->gNoDirHard.empty());
}

//Проверка рёбер в ориентированном графе
TYPED_TEST(GraphTest, CheckEdgesDir) {
    EXPECT_EQ(this->gDirHard.edgeExist(3, 2), 8);
    EXPECT_EQ(this->gDirHard.edgeExist(3, 1), 2);
    EXPECT_EQ(this->gDirHard.edgeExist(4, 3), 4);
    EXPECT_EQ(this->gDirHard.edgeExist(5, 3), 1);
    EXPECT_EQ(this->gDirHard.edgeExist(4, 5), -5);
    EXPECT_EQ(this->gDirHard.edgeExist(4, 2), 3);
    EXPECT_FALSE(this->gDirHard.edgeExist(5, 4));
    EXPECT_FALSE(this->gDirHard.edgeExist(2, 3));
    EXPECT_FALSE(this->gDirHard.edgeExist(1, 5));
}

//Проверка соседей в ориентированном графе
TYPED_TEST(GraphTest, CheckNeighboursDir) {
    unordered_set<int> check3{1, 2};
    unordered_set<int> check4{3, 5, 2};
    vector<int> three = this->gDirHard.neighbours(3);
    vector<int> four = this->gDirHard.neighbours(4);
    ASSERT_TRUE(check3.size() == three.size());
    ASSERT_TRUE(check4.size() == four.size());
    for(int i = 0; i < three.size(); i++) {
        EXPECT_TRUE(check3.count(three[i]));
    }
    for(int i = 0; i < four.size(); i++) {
        EXPECT_TRUE(check4.count(four[i]));
    }
}

//Добавление, удаление в ориентированном графе
TYPED_TEST(GraphTest, RemoveEdgesDir) {
    this->gDirHard.remove(4, 2);
    this->gDirHard.remove(1, 3);
    this->gDirHard.remove(3, 2);
    this->gDirHard.remove(4, 5);
    this->gDirHard.remove(5, 3);
    unordered_set<int> check3{1};
    unordered_set<int> check4{3};
    vector<int> three = this->gDirHard.neighbours(3);
    vector<int> four = this->gDirHard.neighbours(4);
    ASSERT_TRUE(check3.size() == three.size());
    ASSERT_TRUE(check4.size() == four.size());
    for(int i = 0; i < three.size(); i++) {
        EXPECT_TRUE(check3.count(three[i]));
    }
    for(int i = 0; i < four.size(); i++) {
        EXPECT_TRUE(check4.count(four[i]));
    }
    this->gDirHard.remove(4, 3);
    this->gDirHard.remove(3, 1);
    EXPECT_FALSE(this->gDirHard.edgeExist(4, 3));
    EXPECT_TRUE(this->gDirHard.empty());
}