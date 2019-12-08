#include <vector>
#include <iostream>
#include <algorithm>
#include "public.h"

using namespace std;

#define STEP_LENGTH 10
TA_NODE *A_STAR::getAnode(int x, int y)
{
    return &_map[y * map_col + x];
}
int A_STAR::insertNeighbor2Openlist(TA_NODE *cur_node, TA_NODE *_end)
{
    TA_NODE *tmp = NULL;
    vector<TA_NODE*>::iterator is_contain;
    //取到当前节点的上下左右节点加入open列表 并设置这些节点的parent为当前节点
    //坐标系与数组下标有所不同
    //上
    if (cur_node->x > 0)
    {
        tmp = getAnode(cur_node->x-1, cur_node->y);
        is_contain = std::find(open_list.begin(), open_list.end(), tmp);
        if (tmp->attr != A_ATTR_FORBID && tmp->attr != A_ATTR_CLOSE && is_contain == open_list.end())
        {
            tmp->h = (abs(tmp->x - _end->x) + abs(tmp->y - _end->y)) * STEP_LENGTH;
            tmp->g = cur_node->g + STEP_LENGTH;
            tmp->parent = cur_node;
            open_list.push_back(tmp);
        }
    }

    //下
    if (cur_node->x < map_row - 1)
    {
        tmp = getAnode(cur_node->x+1, cur_node->y);
        is_contain = find(open_list.begin(), open_list.end(), tmp);
        if (tmp->attr != A_ATTR_FORBID && tmp->attr != A_ATTR_CLOSE && is_contain == open_list.end())
        {
            tmp->h = (abs(tmp->x - _end->x) + abs(tmp->y - _end->y)) * STEP_LENGTH;
            tmp->g = cur_node->g + STEP_LENGTH;
            tmp->parent = cur_node;
            open_list.push_back(tmp);
        }
    }

    //左
    if (cur_node->y > 0)
    {
        tmp = getAnode(cur_node->x, cur_node->y-1);
        is_contain = std::find(open_list.begin(), open_list.end(), tmp);
        if (tmp->attr != A_ATTR_FORBID && tmp->attr != A_ATTR_CLOSE && is_contain == open_list.end())
        {
            tmp->h = (abs(tmp->x - _end->x) + abs(tmp->y - _end->y)) * STEP_LENGTH;
            tmp->g = cur_node->g + STEP_LENGTH;
            tmp->parent = cur_node;
            open_list.push_back(tmp);
        }
    }

    //右
    if (cur_node->y < map_col - 1)
    {
        tmp = getAnode(cur_node->x, cur_node->y+1);
        is_contain = std::find(open_list.begin(), open_list.end(), tmp);
        if (tmp->attr != A_ATTR_FORBID && tmp->attr != A_ATTR_CLOSE && is_contain == open_list.end())
        {
            tmp->h = (abs(tmp->x - _end->x) + abs(tmp->y - _end->y))*STEP_LENGTH;
            tmp->g = cur_node->g + STEP_LENGTH;
            tmp->parent = cur_node;
            open_list.push_back(tmp);
        }
    }
    return 0;
}

bool A_STAR::compFunc(TA_NODE *a, TA_NODE *b)
{
    //比较f值
    int a_f = a->g + a->h;
    int b_f = b->g + b->h;
    return a_f < b_f;
}
void A_STAR::printPath(TA_NODE *node)
{
    cout << "print path backward order\n";
    while (node != NULL)
    {
        node->attr = A_ATTR_RIGHT;
        printf("%d,%d\n", node->x, node->y);
        node = node->parent;
    }
}

void A_STAR::printMap()
{
    cout << endl;
    TA_NODE *node = NULL;
    for (int i = 0; i < map_row; i++)
    {
        for (int j = 0; j < map_col; j++)
        {
            node = getAnode(j, i);
            if (node->attr == A_ATTR_RIGHT)
                cout << "* ";
            else if (node->attr == A_ATTR_FORBID)
                cout << "0 ";
            else
               cout << "  ";
        }
        cout << endl;
    }
}

int A_STAR::getpath(TA_NODE *_start, TA_NODE *_end) {
    TA_NODE *cur_node;
    open_list.push_back(_start);

    while(!open_list.empty())
    {
        cur_node = open_list.at(0);
        open_list.erase(open_list.begin());
        cur_node->attr = A_ATTR_CLOSE;
        //如果该节点等于终点 则循环结束
        if (cur_node->x < 0 || cur_node->x >= map_col || cur_node->y < 0 || cur_node->y >= map_row)
        {
            break;
        }
        //cout << "col:" << cur_node->x << "  row:" << cur_node->y << endl; 
        if (cur_node->x == _end->x && cur_node->y == _end->y)
        {
            printf("from %d,%d to %d,%d, find best way!\n",_start->x,_start->y,_end->x,_end->y);
            printPath(cur_node);
            printMap();
            break;
        }
        //根据这个节点寻找四周可以走的节点，并计算他们的g和h
        //将这些节点加入open列表
        insertNeighbor2Openlist(cur_node, _end);
        //排序open列表 让f最小的节点放在最前边
        sort(open_list.begin(), open_list.end(), A_STAR::compFunc);
    }
    return 0;
}