#include <iostream>
#include <fstream>
#include <ios>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "public.h"
using namespace std;

class TPlayerInfo 
{
    public:
    int id;
    int x;   /* col */
    int y;   /* row */
    int dia_cnt;  /* diamond count*/
    int gold_cnt;
    int weapon_cnt;
    int fly_cnt;
    char name;
    TPlayerInfo(int idx, char playName);
    friend ostream& operator<< (ostream& os, const TPlayerInfo& pI);
    TPlayerInfo& operator =(const TPlayerInfo& pI);
};

TPlayerInfo::TPlayerInfo(int idx, char playName)
{
    id = idx;
    name = playName;
    x = 0;
    y = 0;
    dia_cnt = 0;
    gold_cnt = 0;
    weapon_cnt = 0;
    fly_cnt = 0;
}
ostream& operator <<(ostream& os, const TPlayerInfo& pI)
{
    os << "\tPlayer\t" << pI.name << endl;
    os << "\tDiamond\t" << pI.dia_cnt << endl;
    os << "\tFly\t" << pI.fly_cnt << endl;
    os << "\tGlod\t" << pI.gold_cnt << endl;
    os << "\tWeapon\t" << pI.weapon_cnt << endl;
    return os;
}
TPlayerInfo& TPlayerInfo::operator =(const TPlayerInfo& pI)
{
    if (&pI != this ) {
        name = pI.name;
        dia_cnt = pI.dia_cnt;
        fly_cnt = pI.fly_cnt;
        gold_cnt = pI.gold_cnt;
        weapon_cnt =  pI.weapon_cnt;
        x = pI.x;
        y = pI.y;
        id = pI.id;
    }
    return *this;
}

class TGameMap
{
private:
    /* data */
    char *nodes;
    int row;
    int col;
public:
    TGameMap(int row, int col, char *data);
    TGameMap(const char* mapName) {nodes = NULL; loadMap(mapName);}
    //GameMap& operator = (const GameMap &) {};
    ~TGameMap();
    int loadMap(const char* mapName);
    char getPos(int x, int y);
    void setPos(char ch, int x, int y);
    void printMap();
    int getCol() { return col;}
    int getRow() { return row;}
    int getMapArray(char * data, int* pCol, int* pRow);
};
class TPlayer
{
private:
    /* data */
    int row;
    int col;
    int mapr;
    int mapc;
    TNode pos;
    TGameMap *map;
    TPlayerInfo *enemy;
    TPlayerInfo *my;
    void move(int dir);
    void move(int x, int y);
    int decisions();
    int sign();
public:
    TPlayer(int x = 0, int y = 0): row(x), col(y) { my = NULL;enemy = NULL; map = NULL;};
    TPlayer(): TPlayer(0,0) {};
    ~TPlayer(){};
    TNode go(const TPlayerInfo &myInfo, const TPlayerInfo& enemyInfo, const TGameMap& gameMap);
    
};

TGameMap::TGameMap(int row, int col, char *data)
{
    int len = row * col;
    nodes = new char(len);
    memcpy(nodes, data, len);
}

TGameMap::~TGameMap()
{
    delete nodes;
    nodes = NULL;
}

int TGameMap::loadMap(const char* mapName)
{
    ifstream f;

    f.open(mapName, ios::in);
    if (f.bad()) {
        return -1;
    }

    int i = 0;
    f >> row >> col;
    int s = row * col;
    
    if(nodes == NULL) {
        nodes = new char [s];
    }
    char buf[1000];
    while (!f.eof() && (f >> buf)) {
        memcpy(&nodes[i], buf, col);
        i += col;
    }
    cout << row << "   "  << col << "size: " << i << endl;
    f.close();
    
    return 0;
}

void TGameMap::printMap()
{
    for (int i = 0; i < row; i++) {
        cout << endl;
        for (int j = 0; j < col; j++) {
            cout << "  " << nodes[i * col + j] << "  ";
        }
        cout << endl;
    }
    cout << " row: " << row << "col: " << col << endl;
    return;
}

char TGameMap::getPos(int x, int y)
{
    if (x < col && y < row) {
        return nodes[y * col + x];
    }
    return 0;
}
void TGameMap::setPos(char ch, int x, int y)
{
    if (x < col && y < row) {
        nodes[y * col + x] = ch;
    }
}
int TGameMap::getMapArray(char* data, int* pCol, int* pRow)
{
    if (data == NULL || pCol == NULL || pRow == NULL)
    {
        return -1;
    }
    *pCol =col;
    *pRow = row;
    memcpy(data, nodes, col * row);
    return 0;
}
class TGame
{
private:
    TGameMap *map;
    vector<TPlayerInfo> info;
    int playid;
public:
    TGame(const char *mapName);
    ~TGame() {};
    void update(TPlayerInfo Player, int x, int y);
    int addPlayer(char name);
    void updatePlayerInfo(TPlayerInfo &Player, char ch, int x, int y);
    void show();
    void showPlayerInfo();
    int getMapArray(char * data, int* pCol, int* pRow);
    int play(TPlayer& player);
};

TGame::TGame(const char *mapName)
{
    playid = 0;
    map = new TGameMap(mapName);
}

void TGame::update(TPlayerInfo Player, int x, int y)
{
    char ch;
    char len;
    TPlayerInfo *playerInfo;
    if (Player.id >= playid || Player.name != info[Player.id].name) {
        return;
    }
    ch = map->getPos(x, y);
    if (ch == 0) {
        return;
    }
    playerInfo = &info[Player.id];
    len = abs(x - playerInfo->x) + abs(y - playerInfo->y);
    if (ch != 'O' && ch != playerInfo->name && playerInfo->weapon_cnt <1) {
        return;
    }
    if (len > 1 && playerInfo->fly_cnt < 1) {
        return;
    }
    if (ch != 'O' && ch != playerInfo->name && playerInfo->weapon_cnt > 0) {
        playerInfo->weapon_cnt--;
    }
    if (len > 1 && playerInfo->fly_cnt > 0) {
        playerInfo->fly_cnt--;
    }
    
    map->setPos(playerInfo->name, x, y);
    updatePlayerInfo(*playerInfo, ch, x, y);
    return;
}
int TGame::addPlayer(char name)
{
    TPlayerInfo *playerInfo;
    playerInfo = new TPlayerInfo(playid, name);
    info.push_back(*playerInfo);
    playid++;
    if (playerInfo->id == 0)
    {
        update(*playerInfo, 0, 0);
    }
    
    return playerInfo->id;
    
}
void TGame::updatePlayerInfo(TPlayerInfo &playerInfo, char ch, int x, int y)
{
    switch (ch) {
        case 'G':
        case 'g':
            playerInfo.gold_cnt++;
            break;
        case 'F':
        case 'f':
            playerInfo.fly_cnt++;
            break;
        case 'W':
        case 'w':
            playerInfo.weapon_cnt++;
            break;
        case 'D':
        case 'd':
            playerInfo.dia_cnt++;
            break;
    }
    playerInfo.x = x;
    playerInfo.y = y;
}
void TGame::show()
{
    map->printMap();
    showPlayerInfo();
    
}
void TGame::showPlayerInfo()
{
    cout << info[0] << endl;
}
int TGame::getMapArray(char * data, int* pCol, int* pRow)
{
    return map->getMapArray(data, pCol, pRow);
}
int TGame::play(TPlayer& player)
{
    TNode pos;
    pos = player.go(info[0], info[0], *map);
    cout << "(" << pos.x << "," << pos.y << ")" << endl;
    update(info[0], pos.x, pos.y);
    return 0;
}

void TPlayer::move(int dir)
{
    switch(dir) {
        case MOVE_LEFT: /* left*/
            col > 1 ? col-- : col;
            break;
        case MOVE_UP:
            row > 1 ? row-- : row;
            break;
        case MOVE_RIGHT:
            col < mapc - 1 ? col++ : col;
            break;
        case MOVE_DOWN:
            row < mapr - 1 ? row++ : row;
    }
}
void TPlayer::move(int x, int y)
{
    row = x;
    col = y;
}
TNode TPlayer::go(const TPlayerInfo &myInfo, const TPlayerInfo& enemyInfo, const TGameMap& gameMap)
{
    decisions();
    if (my == NULL)
    {
       my = new TPlayerInfo(myInfo);
    }
    if (map == NULL) {
        map = new TGameMap(gameMap);
        mapc = map->getCol();
        mapr = map->getRow();
        sign();
    }
    if (col < mapc - 1) {
        move(MOVE_RIGHT);
    } else if (row < mapr - 1) {
        move(MOVE_DOWN);
    } 

    pos.x = col;
    pos.y = row;
    
    return pos;
}
int TPlayer::decisions()
{
    return 0;
}
int TPlayer::sign()
{
    int len = mapc * mapr;
    NODE_T *nodes = new NODE_T[len];
    char cnode[len];
    int x, y;
    map->getMapArray(cnode, &x, &y);
    for (int i = 0; i < len; i++)
    {
        nodes[i].ch = cnode[i];
        nodes[i].color = 0; 
        nodes[i].idle = 0;
        nodes[i].x = i % mapc;
        nodes[i].y = i / mapc;
    }
    floodFill(nodes, mapr, mapc, 0, 0);
    for (int i = 0; i < mapr; i++)
    {
        for (int j = 0; j < mapc; j++)
        {
            cout << "  " << nodes[i * mapc +j].ch << int(nodes[i * mapc + j].color);
        }
        cout << endl;
    }
    return 0;
}
int main() 
{
    char ch;
    TPlayer A = TPlayer(0,0);
    TGame g = TGame("map/0.map");
    g.addPlayer('A');
    g.show();
    while (ch = cin.get()) {
        if (ch == 'q') {
            break;
        }
        cout << ch << endl;
        g.play(A);
        g.show();
    }
    return 0;
}