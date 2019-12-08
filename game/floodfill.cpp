#include "public.h"
#include <vector>
using namespace std;
int checkActive(NODE_T node)
{
    if (node.color != 0) {
        return 0;
    }
    
    if (node.ch == 'O' || node.ch == 'F' || node.ch == 'W' || node.ch == 'D') {
        return 1;
    }
    return 0;
}
int floodFill(NODE_T *nodes, int row, int col, int x, int y)
{
    vector <NODE_T*> s;
    NODE_T *node = nullptr;
    NODE_T *nextNode = nullptr;
    int pos = y * col + x;
    for (int i = 0; i < row * col; i++) {
        nodes[i].color = 0;
    }
    
    nodes[pos].color = 1;
    node = &nodes[pos];
    s.push_back(node);
    while (!s.empty()) {
        node = s.back();
        s.pop_back();
        pos = node->y * col + node->x;
        if (node->y < row -1 && checkActive(nodes[pos + col])) {
            node->idle++;
            nextNode = &nodes[pos + col];
            s.push_back(nextNode);
            nodes[pos + col].color = node->color;
        }
        if (node->y > 0 && checkActive(nodes[pos - col])) {
            node->idle++;
            s.push_back(&nodes[pos - col]);
            nodes[pos - col].color = node->color;
        }
        if (node->x > 0 && checkActive(nodes[pos - 1])) {
            node->idle++;
            s.push_back(&nodes[pos - 1]);
            nodes[pos - 1].color = node->color;
        }
        nextNode = &nodes[pos + 1];
        if (node->x < col - 1 && checkActive(nodes[pos + 1])) {
            node->idle++;
            s.push_back(&nodes[pos + 1]);
            nodes[pos + 1].color = node->color;
        }
    }
    
    return 0;
}