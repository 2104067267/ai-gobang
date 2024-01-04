//
// Created by 21040 on 2024/1/4.
//
#ifndef UNTITLED3_五子棋_H
#define UNTITLED3_五子棋_H
#include<iostream>
#include<map>
#include<vector>
#include<array>
#include<utility>
using namespace std;
#include"raylib.h"
#define WIDTH 15
#define LENTH 15
#define EMPTY 0
#define  black 1
#define white  -1
#define rectangle_size 80

//棋盘
typedef struct chess_map{
    int map[WIDTH][LENTH];
}chess_map;

//当前棋盘的子节点
typedef struct children{
    vector<chess_map> vec;
}children;

////当前棋盘状态，模拟的子棋盘状态
map<chess_map,children> mp;

////存储的是相应节点的父节点，用于反向传播
map<chess_map,chess_map> father;

bool operator < (chess_map x,chess_map y){//用于搜索棋盘状态
    for(int i=0;i<WIDTH;i++)
    {
        for(int j=0;j<LENTH;j++)//比较最先不同的位置，列先行后
        {
            if(x.map[i][j]<y.map[i][j])return true;
            else if(x.map[i][j]>y.map[i][j])return false;
        }
    }
    return false;
}

bool operator == (chess_map x,chess_map y){//用于判断棋盘状态是否相等
    for(int i=0; i <= WIDTH; i++)
    {
        for(int j=0; j < LENTH; j++)
        {
            if(x.map[i][j]!=y.map[i][j])return false;
        }
    }
    return true;
}

int check_win(chess_map x);////检查游戏是否胜利

int is_terminal(chess_map &x);////检查是否为叶子节点，有无剩余空位置

void init_map(chess_map x);////初始地图

int  play(chess_map &map,int play);////玩家走子

pair<int ,pair<int,int>> key_record(chess_map x,int player);////寻找关键棋形

pair<int,int> cal_centre(chess_map x,int player);////棋子、棋型计算走子中心

#define search_range 3 ////中心搜索范围

#define _depth 3 ////搜索深度

int ab_search( int depth, chess_map x, int player, int a, int b);////剪枝搜索

chess_map UCT_search(chess_map x,pair<int,int> center,int player);////蒙特卡罗树搜索

pair<int,pair<int,int>> predict_win(chess_map x , int player);////预言胜利

int get_score(int &num);////棋形赋分函数

int  calculate_score(chess_map x,int player);////估值函数

int link_evaluate(chess_map x,int player);////连通行估分

int __centerlise(int size,int**board,chess_map x,int player,pair<int,int> location);////目数估分

chess_map copy(chess_map x);////复制函数

#endif //UNTITLED3_五子棋_H
