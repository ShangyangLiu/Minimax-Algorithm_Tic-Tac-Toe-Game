/*
 * File: main.cpp
 * --------------
 * Blank C++ project configured to use Stanford cslib and Qt
 */

#include "console.h"
#include "simpio.h"
#include "grid.h"
#include "set.h"
#include "map.h"
using namespace std;

Map<char,char> nextPlayer={{'X','O'},{'O','X'}};

bool isGameEnd(Grid<char> grid, GridLocation loc)
{
    if((grid[0][0] == grid[1][1] && grid[0][0] == grid[2][2])
            || (grid[0][2] == grid[1][1] && grid[2][0] == grid[1][1])
            || (grid[0][loc.col] == grid[1][loc.col] && grid[2][loc.col] == grid[1][loc.col])
            || (grid[loc.row][0] == grid[loc.row][1] && grid[loc.row][0] == grid[loc.row][2]))
    {
        return true;
    }
    return false;
}

Set<GridLocation> feasibleMove(Grid<char> &grid)
{
    Set<GridLocation> moves;
    for(GridLocation x: grid.locations())
    {
        if(isdigit(grid[x]))
        {
            moves += x;
        }
    }
    return moves;
}

int optimalMove(Grid<char> &grid,GridLocation &move,char player)
{
    if(feasibleMove(grid).isEmpty())
    {
        return 0;
    }
    else
    {
        GridLocation tieMove(-1,-1);
        for(GridLocation loc: feasibleMove(grid))
        {
            char backup=grid[loc];
            grid[loc]=player;
            if(isGameEnd(grid,loc))
            {
                move = loc;
                grid[loc]=backup;
                return 1;
            }
            else
            {
                GridLocation oppoentMove;
                int oppoentOptimalMove = optimalMove(grid,oppoentMove,nextPlayer[player]);
                if(oppoentOptimalMove == -1)
                {
                    move = loc;
                    grid[loc]=backup;
                    return 1;
                }
                else if(oppoentOptimalMove == 0)
                {
                    tieMove = loc;
                }
            }
            grid[loc]=backup;
        }
        if(tieMove != GridLocation(-1,-1))
        {
            move = tieMove;
            return 0;
        }
        else
        {
            move = feasibleMove(grid).first();
            return -1;
        }
    }
}

int main()
{
    Grid<char> grid = {{'1','2','3'},{'4','5','6'},{'7','8','9'}};
    cout<<"Welcom to TicTacToe"<<endl;
    cout<<"I will be X, and you will be O"<<endl;
    cout<<"The squares are numbered like this:"<<endl;
    cout<<grid.toString2D()<<endl<<endl;
    bool yes = getYesOrNo("DO you want to be the first?");
    char player = yes? 'O':'X';
    GridLocation move;
    while(!isGameEnd(grid,move) && !feasibleMove(grid).isEmpty())
    {
        if(player == 'O')
        {
            int temp = getInteger("You move,what square:") - 1;
            move = {temp / 3,temp % 3};
            while(!feasibleMove(grid).contains(move))
            {
                temp = getInteger("Invalid! Imput again:") - 1;
                move = {temp / 3,temp % 3};
            }
            grid[move] = 'O';
        }
        else
        {
            optimalMove(grid,move,player);
            cout<<"I will move to:"<<move.row * 3 + move.col + 1<<endl;
            grid[move] = 'X';
        }
        cout<<"The game now looks like this"<<endl;
        cout<<grid.toString2D()<<endl<<endl;
        player = nextPlayer[player];
        pause(1000);
    }
    if(isGameEnd(grid,move))
    {
        cout<<(player == 'O'? "I win": "you win")<<endl;
    }
    else
    {
        cout<<"We tie"<<endl;
    }

    return 0;
}








