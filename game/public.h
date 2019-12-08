#ifndef __PUBLIB_H__
#define __PUBLIB_H__
#include <vector>

enum DIR { MOVE_LEFT = 0, MOVE_UP = 1, MOVE_RIGHT = 2, MOVE_DOWN = 3 };
typedef struct TNode {
    int x;
    int y;
    char ch;
    char color;
    short idle;
} NODE_T;
int floodFill(TNode *nodes, int row, int col, int x, int y);
enum A_ATTR {
    A_ATTR_NULL,
    A_ATTR_OPEN,
    A_ATTR_CLOSE,
    A_ATTR_FORBID,
    A_ATTR_RIGHT
};
typedef struct A_NODE {
    int x;
    int y;
    int h;
    int g;
    int attr;
    struct A_NODE *parent;
} TA_NODE;
class A_STAR {
  private:
    int map_row;
    int map_col;
    TA_NODE *_map;
    std::vector<TA_NODE *> open_list;
    std::vector<TA_NODE *> close_list;
    TA_NODE *getAnode(int x, int y);
    int insertNeighbor2Openlist(TA_NODE *cur_node, TA_NODE *_end);
    static bool compFunc(TA_NODE *a, TA_NODE *b);
    void printPath(TA_NODE *node);
    void printMap();
  public:
    int getpath(TA_NODE *_start, TA_NODE *_end);
    A_STAR();
    ~A_STAR();
};
#endif