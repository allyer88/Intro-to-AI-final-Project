// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Jian Li
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================


#ifndef MINE_SWEEPER_CPP_SHELL_MYAI_HPP
#define MINE_SWEEPER_CPP_SHELL_MYAI_HPP

#include "Agent.hpp"
#include <iostream> // temporary use
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <functional>
#include <utility>

using namespace std;
    
class MyAI : public Agent
{
public:
    MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY );
    Action getAction ( int number ) override;
    ~MyAI();
    class tile {
    public:
        //index start from 0
        int col, row;
        tile(){};
        tile(int x, int y) {
            col = x;
            row = y;
        }
        bool operator==(const tile& other) const{
        return col == other.col && row == other.row;
        }
        bool operator<(const tile& other) const {
            if (col != other.col) return col < other.col;
            return row < other.row;
        }

    };
    struct TileHasher {
        size_t operator()(const tile& t) const {
            size_t colHash = hash<int>{}(t.col);
            size_t rowHash = hash<int>{}(t.row);
            return colHash ^ rowHash;
        }
    };
    class MCvalues {
    public:
        int State;
        int EffectiveLabel;
        int AdjUMC;
        MCvalues() {
            //-1=covered/unmarked,-2=covered,but already in queue Q, 0-8 number of adj mine, 9 is mine
            State = -1;
            EffectiveLabel = 0;
            AdjUMC = 8;
        }
    };
    class PMine{
        public:
        int mine;
        float probability;
        
        PMine(){
            //-1 haven't visited, 1 safe, 2 mine
            mine=-1;
            probability=0.0;
        }
        PMine(int m, float p){
            mine=m;
            probability=p;
        }
        bool operator==(const PMine& other) const{
        return probability == other.probability && mine == other.mine;
        }
        bool operator<(const PMine& other) const {
        return probability < other.probability;
        }
    };
    struct PMineHasher {
    size_t operator()(const PMine& pmine) const {
        size_t probabilityHash = hash<float>{}(pmine.probability);
        size_t mineHash = hash<int>{}(pmine.mine);
        return probabilityHash ^ mineHash;
        }
    };
    tile pretile;//STORE the previous tile
    MCvalues** board;
    int coverTiles;//number of covered tile
    void neighbors(tile);//push neighbors of to queue
    void effectivelabel(tile);
    void UpdateTileInfo(tile);//Update tile's MCvalues
    void checkPattern(tile);// check Effectivelabels
    void ToCoverM(tile );
    //safeQ is the safe tile's that can be uncovered
    //nonzeroQ is the uncovered tiles which is not zero
    //tmpCoverQ store the tile's neighbors that are covered temperately
    queue<tile> safeQ, nonzeroQ, tmpCoverQ;
    
    //CoverM: use for backtracking
    //Cmap: covered tile connects to its uncovered tiles
    //Umap: Uncovered tile connects to its cover tiles
    map<tile, PMine> coverM;
    vector<pair<float, MyAI::tile>> orderedCoverM;//this is to order the probability
    map<tile, vector<tile>> Cmap, Umap;
    //check whether myMap has saved checkt in t
    bool checkExistInVector(map<tile,vector<tile>> myMap, tile t, tile checkt);
    //Use merge sort to sort the probability
    void merge(vector<pair<float, MyAI::tile>>& arr, int left, int mid, int right);
    void mergeSort(vector<pair<float, MyAI::tile>>& arr, int left, int right);
    //Backtracking
    bool checkVaild(int setmineorsafe, map<tile, PMine>& myMap, map<MyAI::tile, PMine>::iterator it);
    void UCBacktracking(float& solution, map<tile, PMine>& myMap, map<tile, PMine>::iterator it);
    //After Backtracking, find 0/1
    bool FindSafeandMine();
    //If no 0/1, we can only guess based on the probability
    bool Guess();

    //for testing
    void printboard();
};

#endif //MINE_SWEEPER_CPP_SHELL_MYAI_HPP
