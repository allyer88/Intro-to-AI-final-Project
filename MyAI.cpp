// ======================================================================
// FILE:        MyAI.cpp
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
//X is column
//Y is row
#include "MyAI.hpp"
//SET DEBUG TO FALSE BEFORE SUBMITTING !!!!!!
bool DEBUG = false;

MyAI::MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY ) : Agent()
{
    totalMines = _totalMines;
    rowDimension = _rowDimension;
    colDimension = _colDimension;
    agentX = _agentX;
    agentY = _agentY;
    board = new MCvalues*[colDimension];
    // Allocate memory for each row
    for (int i = 0; i < colDimension; i++) {
        board[i] = new MCvalues[rowDimension];
    }
    board[agentX][agentY].State=0;
    coverTiles = _rowDimension * _colDimension -1;
    pretile=tile(agentX,agentY);
    neighbors(pretile);
};
MyAI::~MyAI(){
    for (int i = 0; i < colDimension; i++) {
        delete[] board[i];
    }
    delete[] board;
}


void MyAI::neighbors(tile t) {//for "0" tile
    if (t.col < 0 || t.col >= colDimension || t.row < 0 || t.row >= rowDimension) return;
    if (t.col > 0) {
        if (t.row > 0 && board[t.col - 1][t.row - 1].State == -1) {
            safeQ.push({ t.col - 1, t.row - 1});
            board[t.col - 1][t.row - 1].State = -2;//avoid to uncover again
        }
        if (board[t.col - 1][t.row].State == -1) {
            safeQ.push({ t.col - 1, t.row});
            board[t.col - 1][t.row].State = -2;
        }
        if (t.col < colDimension - 1 && board[t.col - 1][t.row + 1].State == -1) {
            safeQ.push({ t.col - 1, t.row + 1 });
            board[t.col - 1][t.row + 1].State =-2;
        }
    }
    if(t.row>0&& board[t.col][t.row - 1].State == -1){
        safeQ.push({ t.col, t.row - 1});
        board[t.col][t.row - 1].State = -2;
    }
    if(t.row<rowDimension-1&& board[t.col][t.row + 1].State == -1){
        safeQ.push({ t.col, t.row + 1 });
        board[t.col][t.row + 1].State =-2;

    }
    if (t.col < colDimension-1) {
        if(t.row>0&& board[t.col + 1][t.row - 1].State == -1) {
            safeQ.push({ t.col + 1, t.row - 1});
            board[t.col + 1][t.row - 1].State = -2;
        }
        if(board[t.col + 1][t.row].State == -1){
            safeQ.push({ t.col + 1, t.row });
            board[t.col + 1][t.row].State = -2;
        }
        if (t.row < rowDimension - 1 && board[t.col + 1][t.row + 1].State == -1) {
            safeQ.push({ t.col + 1, t.row + 1});
            board[t.col + 1][t.row + 1].State =-2;
        }
    }
}
void MyAI::UpdateTileInfo(tile t) { //number of mine is marked flag
//if(board[t.col][t.row].EffectiveLabel==0 ) return;
    int num=0;
    int numofUMC=0;
    if (t.col > 0) {
        if (t.row > 0) {
            if(board[t.col - 1][t.row - 1].State == 9) {
                num++;
            }
            else if(board[t.col - 1][t.row - 1].State == -1) {
                tmpCoverQ.push({t.col - 1, t.row - 1});
                numofUMC++;
            }
        }
        if (board[t.col - 1][t.row].State == 9) num++;
        else if (board[t.col - 1][t.row].State == -1) {
            tmpCoverQ.push({ t.col - 1, t.row});
            numofUMC++;
        }
        if (t.row < rowDimension - 1){
            if(board[t.col - 1][t.row + 1].State == 9)num++;
            else if(board[t.col - 1][t.row + 1].State == -1) {
                tmpCoverQ.push({ t.col - 1, t.row + 1 });
                numofUMC++;
            }
        } 
    }
    if (t.row > 0 && t.row < rowDimension){
        if(board[t.col][t.row - 1].State == 9) num++;
        else if(board[t.col][t.row - 1].State == -1) {
            tmpCoverQ.push({ t.col, t.row - 1});
            numofUMC++;
        }

    }
    if (t.row >= 0 && t.row < rowDimension - 1){
        if(board[t.col][t.row + 1].State == 9)num++;
        else if (board[t.col][t.row + 1].State == -1) {
            tmpCoverQ.push({ t.col, t.row + 1 });
            numofUMC++;
        }
    }
    if (t.col < colDimension - 1) {
        if (t.row > 0){
            if(board[t.col + 1][t.row - 1].State == 9) num++;
            else if(board[t.col + 1][t.row - 1].State == -1) {
                tmpCoverQ.push({ t.col + 1, t.row - 1});
                numofUMC++;
            }
        } 
        if (board[t.col + 1][t.row].State == 9) num++;
        else if(board[t.col + 1][t.row].State == -1) {
            tmpCoverQ.push({ t.col + 1, t.row });
            numofUMC++;
        }
        if (t.row < colDimension - 1){
            if(board[t.col + 1][t.row + 1].State == 9) num++;
            else if (board[t.col + 1][t.row + 1].State == -1) {
                tmpCoverQ.push({ t.col + 1, t.row + 1 });
                numofUMC++;
            }
        } 
    }
    board[t.col][t.row].EffectiveLabel = board[t.col][t.row].State - num;
    board[t.col][t.row].AdjUMC=numofUMC;
}
void MyAI::merge(vector<pair<float, MyAI::tile>>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid; 
    vector<pair<float, MyAI::tile>> leftArr(n1);
    vector<pair<float, MyAI::tile>> rightArr(n2);
    for (int i = 0; i < n1; ++i) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        rightArr[j] = arr[mid + 1 + j];
    }
    // Merge the temporary arrays back into arr
    int i = 0; // Index of the left subarray
    int j = 0; // Index of the right subarray
    int k = left; // Index of the merged subarray
    while (i < n1 && j < n2) {
        if (leftArr[i].first <= rightArr[j].first) {
            arr[k] = leftArr[i];
            ++i;
        } else {
            arr[k] = rightArr[j];
            ++j;
        }
        ++k;
    }
    while (i < n1) {
        arr[k] = leftArr[i];
        ++i;
        ++k;
    }
    while (j < n2) {
        arr[k] = rightArr[j];
        ++j;
        ++k;
    }
}

//Merge sort to sort probability
void MyAI::mergeSort(vector<pair<float, MyAI::tile>>& arr, int left, int right) {
    if(arr.size()<=1) return;
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void MyAI::checkPattern(tile t){      
    UpdateTileInfo(t);
    if(board[t.col][t.row].AdjUMC==0) return;
    if(board[t.col][t.row].EffectiveLabel==tmpCoverQ.size()){
        //All covered tiles are mine
        board[t.col][t.row].AdjUMC=0;
        while(!tmpCoverQ.empty()){
            board[tmpCoverQ.front().col][tmpCoverQ.front().row].State=9;
            //recheck neighbor uncover tiles
            if(coverM.find(tmpCoverQ.front())!=coverM.end()){
                for(int i=0;i<Cmap[tmpCoverQ.front()].size();i++){
                    if(Cmap[tmpCoverQ.front()][i]==t) continue;
                    bool HasTile=false;
                    for(int j=0;j<nonzeroQ.size();j++){
                        if(nonzeroQ.front()==Cmap[tmpCoverQ.front()][i]) {
                            HasTile=true;
                            break;
                        }
                        else{
                            nonzeroQ.push(nonzeroQ.front());
                            nonzeroQ.pop();
                        }
                    }
                    if(!HasTile) nonzeroQ.push(Cmap[tmpCoverQ.front()][i]);
                }
            }
            
            //for debuging
            if (DEBUG)
            {
                std::cout << "Mine found when pattern checking " << tmpCoverQ.front().col + 1 << " " << tmpCoverQ.front().row + 1 << std::endl;
            }
            if(!coverM.empty() && coverM.find(tmpCoverQ.front())!=coverM.end()) {
                
                for(int i=0;i<Cmap[tmpCoverQ.front()].size();i++){
                    for(int j=0;j<Umap[Cmap[tmpCoverQ.front()][i]].size();j++){
                        if(Umap[Cmap[tmpCoverQ.front()][i]][j]==tmpCoverQ.front()){
                            Umap[Cmap[tmpCoverQ.front()][i]].erase(Umap[Cmap[tmpCoverQ.front()][i]].begin()+j);
                        }
                    }
                }
                Cmap.erase(Cmap.find(tmpCoverQ.front()));
                coverM.erase(coverM.find(tmpCoverQ.front()));
            }
            tmpCoverQ.pop();
        }
        return;
    }else if(board[t.col][t.row].EffectiveLabel==0){
        //all covered tiles are safe
        board[t.col][t.row].AdjUMC=0;
        if(!tmpCoverQ.empty()){
            while(!tmpCoverQ.empty()){
                tile notmine(tmpCoverQ.front().col, tmpCoverQ.front().row);
                safeQ.push(notmine);
                board[tmpCoverQ.front().col][tmpCoverQ.front().row].State=-2;
                tmpCoverQ.pop();
            }   
        }
        return;
    }
    else {
        while(!tmpCoverQ.empty()) tmpCoverQ.pop();
        nonzeroQ.push(t);
    }
    
}
bool MyAI::checkExistInVector(map<tile,vector<tile>> myMap, tile t, tile checkt){
    for(int i=0;i<myMap[t].size();i++){
        if(myMap[t][i]==checkt) return true;
    }
    return false;
}
void MyAI::ToCoverM(tile t){
    UpdateTileInfo(t);
    while(!tmpCoverQ.empty()){
            if(board[tmpCoverQ.front().col][tmpCoverQ.front().row].State==-1){
                if(coverM.find(tmpCoverQ.front())==coverM.end()){
                    MyAI::PMine pmine;
                    coverM[tmpCoverQ.front()]=pmine;
                }  
                if(!checkExistInVector(Umap,t,tmpCoverQ.front())) Umap[t].push_back(tmpCoverQ.front());
                if(!checkExistInVector(Cmap,tmpCoverQ.front(),t))Cmap[tmpCoverQ.front()].push_back(t);
                tmpCoverQ.pop();
            }
            
        }
}

bool MyAI::checkVaild(int setmineorsafe, map<tile,MyAI::PMine>& myMap, map<MyAI::tile, MyAI::PMine>::iterator it){
    MyAI::tile t = it->first;
    it->second.mine=setmineorsafe;
    int numofmine=0;
    int numofvisited=0;
    for(int i=0;i<Cmap[t].size();i++){
        for(int j=0;j<Umap[Cmap[t][i]].size();j++){
            if(myMap[Umap[Cmap[t][i]][j]].mine==2) numofmine++;
            if(myMap[Umap[Cmap[t][i]][j]].mine!=-1) numofvisited++;
        }
        if(numofmine>board[Cmap[t][i].col][Cmap[t][i].row].EffectiveLabel||(numofvisited==Umap[Cmap[t][i]].size()&&numofmine<board[Cmap[t][i].col][Cmap[t][i].row].EffectiveLabel)){
           it->second.mine=-1;
           return false;
        }else {
            numofmine=0;
            numofvisited=0;
        }
    } 
    it->second.mine=-1;
    return true;
}
    

void MyAI::UCBacktracking(float& solution, map<tile, MyAI::PMine>& myMap, map<MyAI::tile, MyAI::PMine>::iterator it) {
    // Base case: Reached the end of the map
    if (it == myMap.end()) {
        solution+=1.0;
        for(auto i=myMap.begin();i!=myMap.end();i++){
            if(i->second.mine==2){
                i->second.probability+=1.0;
            }
        }
        return;
    }
    MyAI::tile t = it->first;
    int setmineorsafe=2;
    while(setmineorsafe>0){
        if(checkVaild(setmineorsafe,myMap,it)) {
            it->second.mine=setmineorsafe;
            UCBacktracking(solution, myMap, std::next(it));
            it->second.mine=-1;
        }
       setmineorsafe--;
    } 
}
bool MyAI::FindSafeandMine(){
    if(coverM.empty()||orderedCoverM.empty()) return false;
    while(!orderedCoverM.empty() && orderedCoverM.front().first==0){
        //if probability is 0, then all safe
            safeQ.push(orderedCoverM.front().second);
            board[orderedCoverM.front().second.col][orderedCoverM.front().second.row].State=-2;
            for(int j=0;j<Cmap[orderedCoverM.front().second].size();j++){
                nonzeroQ.push(Cmap[orderedCoverM.front().second][j]);
            }
        //These just do some cleaning
            auto it=coverM.find(orderedCoverM.front().second);
            coverM.erase(it);
            for(int i=0;i<Cmap[orderedCoverM.front().second].size();i++){
                for(int j=0;j<Umap[Cmap[orderedCoverM.front().second][i]].size();j++){
                    if(Umap[Cmap[orderedCoverM.front().second][i]][j]==orderedCoverM.front().second){
                        Umap[Cmap[orderedCoverM.front().second][i]].erase(Umap[Cmap[orderedCoverM.front().second][i]].begin()+j);
                    }
                }
            }
            Cmap.erase(Cmap.find(orderedCoverM.front().second));
            orderedCoverM.erase(orderedCoverM.begin());
    }
    while(!orderedCoverM.empty() && orderedCoverM.back().first==1){
        //If probability is 1, then it is mine
        board[orderedCoverM.back().second.col][orderedCoverM.back().second.row].State=9;
        if (DEBUG)
        {
            std::cout << "Mine found when backtracking " << orderedCoverM.back().second.col + 1 << " " << orderedCoverM.back().second.row + 1 << std::endl;
        }
        for(int j=0;j<Cmap[orderedCoverM.back().second].size();j++){
            nonzeroQ.push(Cmap[orderedCoverM.back().second][j]);
        }
        //These just do some cleaning
        auto it=coverM.find(orderedCoverM.back().second);
        coverM.erase(it);
        for(int i=0;i<Cmap[orderedCoverM.back().second].size();i++){
                for(int j=0;j<Umap[Cmap[orderedCoverM.back().second][i]].size();j++){
                    if(Umap[Cmap[orderedCoverM.back().second][i]][j]==orderedCoverM.back().second){
                        Umap[Cmap[orderedCoverM.back().second][i]].erase(Umap[Cmap[orderedCoverM.back().second][i]].begin()+j);
                    }
                }
            }
        Cmap.erase(Cmap.find(orderedCoverM.back().second));
        orderedCoverM.pop_back();
    }
    if(!safeQ.empty()){
        pretile=tile(safeQ.front().col, safeQ.front().row);
        agentX = pretile.col;
        agentY = pretile.row;
        safeQ.pop();
        coverTiles--;
        return true;
    }else return false;
}
bool MyAI::Guess(){
    if(coverM.empty()||orderedCoverM.empty()) return false;
    MyAI::tile t= orderedCoverM[0].second;
    for(int j=0;j<Cmap[t].size();j++){
        nonzeroQ.push(Cmap[t][j]);
    }
    coverM.erase(coverM.find(t));
    orderedCoverM.erase(orderedCoverM.begin());
    for(int i=0;i<Cmap[t].size();i++){
        for(int j=0;j<Umap[Cmap[t][i]].size();j++){
            if(Umap[Cmap[t][i]][j]==t){
                Umap[Cmap[t][i]].erase(Umap[Cmap[t][i]].begin()+j);
            }
        }
    }
    Cmap.erase(Cmap.find(t));
    pretile=tile(t.col, t.row);
    agentX = pretile.col;
    agentY = pretile.row;
    coverTiles--;
    return true;
}
Agent::Action MyAI::getAction( int number )
{
    board[agentX][agentY].State = number;
    //push all non zero uncovered tile to nonzeroQ
    if(number!=0) nonzeroQ.push(pretile); 
    if (coverTiles == totalMines) {
        return { LEAVE,-1,-1 };
    }
    else {
        // push all safe tiles to safeQ
        if (number == 0) {
            board[agentX][agentY].AdjUMC=0;
            neighbors(pretile);
        }
        if(!safeQ.empty()){
            while(!safeQ.empty()){
                if(board[safeQ.front().col][safeQ.front().row].State==-2||board[safeQ.front().col][safeQ.front().row].State==-1){        
                    pretile=tile(safeQ.front().col, safeQ.front().row);
                    agentX = pretile.col;
                    agentY = pretile.row;
                    safeQ.pop();
                    coverTiles--;
                    //clean coverM, Cmap, Umap
                    if(!coverM.empty() && coverM.find(pretile)!=coverM.end()) {
                        for(int i=0;i<Cmap[pretile].size();i++){
                            for(int j=0;j<Umap[Cmap[pretile][i]].size();j++){
                                if(Umap[Cmap[pretile][i]][j]==pretile){
                                Umap[Cmap[pretile][i]].erase(Umap[Cmap[pretile][i]].begin()+j);
                                }
                            }
                        }
                        Cmap.erase(Cmap.find(pretile));
                        coverM.erase(coverM.find(pretile));
                    }
                    return { UNCOVER, agentX, agentY};
                }else{
                    //clean coverM, Cmap, Umap
                    if(!coverM.empty() && coverM.find(safeQ.front())!=coverM.end()) {
                        for(int i=0;i<Cmap[safeQ.front()].size();i++){
                            for(int j=0;j<Umap[Cmap[safeQ.front()][i]].size();j++){
                                if(Umap[Cmap[safeQ.front()][i]][j]==safeQ.front()){
                                Umap[Cmap[safeQ.front()][i]].erase(Umap[Cmap[safeQ.front()][i]].begin()+j);
                                }
                            }
                        }
                        Cmap.erase(Cmap.find(safeQ.front()));
                        coverM.erase(coverM.find(safeQ.front()));
                    }
                    safeQ.pop();
                }
            }
        }
        if(!nonzeroQ.empty()){
            while(!nonzeroQ.empty()){
                int size=nonzeroQ.size();
                for(int i=0;i<size;i++){
                    tile nzerotile(nonzeroQ.front().col, nonzeroQ.front().row);
                    nonzeroQ.pop();
                    checkPattern(nzerotile);
                    if(!safeQ.empty()){
                        pretile=tile(safeQ.front().col, safeQ.front().row);
                        agentX = pretile.col;
                        agentY = pretile.row;
                        safeQ.pop();
                        coverTiles--;
                        //clean coverM, Cmap, Umap
                        if(!coverM.empty() && coverM.find(pretile)!=coverM.end()) {
                            for(int i=0;i<Cmap[pretile].size();i++){
                                for(int j=0;j<Umap[Cmap[pretile][i]].size();j++){
                                    if(Umap[Cmap[pretile][i]][j]==pretile){
                                     Umap[Cmap[pretile][i]].erase(Umap[Cmap[pretile][i]].begin()+j);
                                    }
                                }
                            }
                            Cmap.erase(Cmap.find(pretile));
                            coverM.erase(coverM.find(pretile));
                        }
                        return { UNCOVER, agentX, agentY };
                    }
                }
                if(size==nonzeroQ.size()){
                    while(!nonzeroQ.empty()){
                        ToCoverM(nonzeroQ.front());
                        nonzeroQ.pop();
                    }
                }
            }
        }
        if(!coverM.empty()){
            float solution=0.0;
            UCBacktracking(solution, coverM, coverM.begin());
            for(auto i=coverM.begin();i!=coverM.end();i++){
                i->second.mine=-1;
                i->second.probability=i->second.probability/solution;
                orderedCoverM.push_back({i->second.probability, i->first});
            }
            mergeSort(orderedCoverM, 0, orderedCoverM.size()-1);
            if(FindSafeandMine()) {
                orderedCoverM.clear();
                for(auto i=coverM.begin();i!=coverM.end();i++){
                    i->second.probability=0.0; //CLEAR probibility
                }
                return { UNCOVER, agentX, agentY};
            }
            if(Guess()){
                orderedCoverM.clear();
                for(auto i=coverM.begin();i!=coverM.end();i++){
                    i->second.probability=0.0; //CLEAR probibility
                }
                return { UNCOVER, agentX, agentY };
            }
        }
            
    }    
    return {LEAVE, -1,-1}; 
}
//./Minesweeper -f /home/anteater/Desktop/Minesweeper_Student/WorldGenerator/Problems/B/Beginner_world_1.txt
