#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include <chrono>
#include <thread>

#define DEBUG false
#define SHOW false
#define MINI_DISTANCE 1
#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_ESCAPE 27

#define CANDY_COLUMN 5
#define CANDY_ROW 3
using CANDY_ARRAY = char[CANDY_ROW][CANDY_COLUMN];

using namespace std;
using namespace std::chrono;
typedef struct
{
    unsigned int x = 0;
    unsigned int y = 0;
} Position;

int start = 0;
int totalSteps = 0;
string output;
bool checkApproachCandy(const CANDY_ARRAY candy, int distance, int maxApproach);

void printCandy(const CANDY_ARRAY candy)
{
    for (int i = 0; i < CANDY_ROW; i++)
    {
        for (int j = 0; j < CANDY_COLUMN; j++)
        {
            std::cout << candy[i][j] << "  ";
            if(DEBUG) if(candy[i][j] <97 || candy[i][j]>122) exit(0);
        }
        std::cout << std::endl;
    }
    cout << endl;
}
void initCandy(CANDY_ARRAY candy)
{
    for (int i = 0; i < CANDY_ROW; i++)
    {
        for (int j = 0; j < CANDY_COLUMN; j++)
        {
            candy[i][j] = 'e';
        }
    }
}
void string2Candy(string line, CANDY_ARRAY candy)
{
    if (line.length() > 2 * CANDY_ROW * CANDY_COLUMN)
        return;
    for (int i = 0; i < line.length(); i += 2)
    {
        char s = line.at(i);
        candy[i / (2 * CANDY_COLUMN)][(i / 2) % CANDY_COLUMN] = s;
    }
}
string candy2String(const CANDY_ARRAY candy)
{
    string candyStr = "";
    for (int i = 0; i < CANDY_ROW; i++)
    {
        for (int j = 0; j < CANDY_COLUMN; j++)
        {
            candyStr.push_back(candy[i][j]);
        }
    }
    return candyStr;
}
bool checkCandy(CANDY_ARRAY candy)
{
    for (int f = 0; f < CANDY_COLUMN; f++)
    {
        if (candy[0][f] != candy[CANDY_ROW - 1][f])
        {
            return false;
        }
    }
    return true;
}
bool checkMoveCandy(const CANDY_ARRAY candy, int xDistance, int yDistance)
{
    std::map<char, Position> counting;
    std::pair<std::map<char, Position>::iterator, bool> ret;
    int count = 0;
    for (int j = 0; j < CANDY_COLUMN; j++)
    {
        for (int i = 0; i < CANDY_ROW; i+=2)
        {
            if (candy[i][j] != 'e')
            {
                Position p;
                p.x = i;
                p.y = j;
                ret = counting.insert(std::pair<char, Position>(candy[i][j], p));
                if (ret.second == false)
                {
                    if (i - ret.first->second.x >= xDistance && j - ret.first->second.y <= yDistance)
                    {
                        count++;
                    }
                    counting.erase(candy[i][j]);
                }
            }
        }
    }
    return count >= 5;
}
Position getPosition(CANDY_ARRAY candy)
{
    Position p;
    for (int i = 0; i < CANDY_ROW; i++)
    {
        for (int j = 0; j < CANDY_COLUMN; j++)
        {
            if (candy[i][j] == 'e')
            {
                p.x = i;
                p.y = j;
                return p;
            };
        }
    }
}

void swapCandy(Position p1, Position p2, CANDY_ARRAY candy)
{
    if (p1.x == p2.x && p1.y == p2.y && p1.x >= CANDY_ROW && p1.y >= CANDY_COLUMN && p2.x >= CANDY_ROW && p2.y >= CANDY_COLUMN)
    {
        return;
    }
    char swap = candy[p1.x][p1.y];
    candy[p1.x][p1.y] = candy[p2.x][p2.y];
    candy[p2.x][p2.y] = swap;
}

char getStep(Position p)
{
    char steps[][5] = {{'A', 'B', 'C', 'D', 'E'}, {'F', 'G', 'H', 'I', 'J'}, {'K', 'L', 'M', 'N', 'O'}};
    return steps[p.x][p.y];
}

void writeSoltion(string solution, int time)
{
    ofstream file;
    file.open(output, std::ios_base::app);
    file << solution;
    file << "\n";
    file << time << "ms";
    file << "\n";
    file.close();
}
void moveCandy(CANDY_ARRAY candy)
{

    char ch;
    int steps = 0;
    string solution = "";
    system("stty -echo");  // supress echo
    system("stty cbreak"); // go to RAW mode
    do
    {
        printCandy(candy);
        Position p1 = getPosition(candy);
        Position p2 = p1;
        ch = getchar();
        if (ch == 'w')
        {
            if (p1.x == 0)
            {
                if(DEBUG)
                    cout << "**Cannot go up!" << endl;
            }
            else
            {
                p2.x--;
                steps++;
                solution.push_back(getStep(p2));
                if(DEBUG) cout << "UP!" << endl;
            }
        }
        else if (ch == 's')
        {
            if (p1.x == CANDY_ROW - 1)
            {
                if(DEBUG) cout << "**Cannot go down!" << endl;
            }
            else
            {
                p2.x++;
                steps++;
                solution.push_back(getStep(p2));
                if(DEBUG) cout << "DOWN" << endl;
            }
        }
        else if (ch == 'd')
        {
            if (p1.y == CANDY_COLUMN - 1)
            {
                if(DEBUG) cout << "**Cannot go right!" << endl;
            }
            else
            {
                p2.y++;
                steps++;
                solution.push_back(getStep(p2));
                if(DEBUG) cout << "RIGHT" << endl;
            }
        }
        else if (ch == 'a')
        {
            if (p1.y == 0)
            {
                if(DEBUG) cout << "**Cannot go left!" << endl;
            }
            else
            {
                p2.y--;
                steps++;
                solution.push_back(getStep(p2));
                if(DEBUG) cout << "LEFT!" << endl;
            }
        }
        else if (ch == 'q')
        {
            cout << "QUIT" << endl;
        }
        swapCandy(p1, p2, candy);
        if (checkCandy(candy))
        {
            cout << "Congratulations!! Spend:" << steps << " steps" << endl;
            cout << "Solution is " << solution << endl;
            totalSteps += steps;
            writeSoltion(solution, steps);
            break;
        }
    } while (ch != 'q');
    system("stty echo");    // Make echo work
    system("stty -cbreak"); // go to COOKED m
}
void loadFile(string file)
{
    string line;
    ifstream filestream(file);
    if (filestream.is_open())
    {
        while (getline(filestream, line))
        {
            cout << "Candy Problem:" << line << endl
                 << endl;
            CANDY_ARRAY candy;
            initCandy(candy);
            string2Candy(line, candy);
            moveCandy(candy);
        }
        filestream.close();
    }
    else
    {
        cout << "File opening is fail." << endl;
    }
}
int countUniqueCharacters(std::string str)
{
    std::sort(str.begin(), str.end());
    auto i = std::unique(str.begin(), str.end());
    return i - str.begin();
}
int heuristicFunction(const CANDY_ARRAY candy)
{
    int candyCount = countUniqueCharacters(candy2String(candy));
    if(candyCount < 10) {
        int heuristic = CANDY_COLUMN;
        for (int f = 0; f < CANDY_COLUMN; f++)
        {
            if (candy[0][f] == candy[CANDY_ROW - 1][f])
            {
                heuristic--;
            }
        }
        return heuristic;
    } else {
        std::map<char, Position> counting;
        std::pair<std::map<char, Position>::iterator, bool> ret;
        int heuristic = 0;
        int count = 0;
        for (int j = 0; j < CANDY_COLUMN; j++)
        {
            for (int i = 0; i < CANDY_ROW; i+=2)
            {
                if (candy[i][j] != 'e')
                {
                    Position p;
                    p.x = i;
                    p.y = j;
                    ret = counting.insert(std::pair<char, Position>(candy[i][j], p));
                    if (ret.second == false)
                    {
                        if(ret.first->second.y - j <=1) {
                            count++;
                            heuristic += (CANDY_ROW-1-(i-ret.first->second.x)+ret.first->second.y - j);
                        } else {
                            heuristic += (CANDY_ROW+ret.first->second.y - j);
                        }
                        counting.erase(candy[i][j]);
                        if(count == CANDY_COLUMN) {
                            return heuristic+CANDY_ROW*(CANDY_COLUMN-count);
                        }
                    }
                }
            }
            int i=1;
            if (candy[i][j] != 'e')
                {
                    Position p;
                    p.x = i;
                    p.y = j;
                    ret = counting.insert(std::pair<char, Position>(candy[i][j], p));
                    if (ret.second == false)
                    {
                        if(ret.first->second.y - j <=1) {
                            count++;
                            heuristic += (CANDY_ROW-1-(i-ret.first->second.x)+ret.first->second.y - j);
                        } else {
                            heuristic += (CANDY_ROW+ret.first->second.y - j);
                        }
                        counting.erase(candy[i][j]);
                        if(count == CANDY_COLUMN) {
                            heuristic+CANDY_ROW*(CANDY_COLUMN-count);
                        }
                    }
                }
        }
        return heuristic+CANDY_ROW*(CANDY_COLUMN-count);
    }
}

void betterSearchMove(const CANDY_ARRAY candy, vector<string> history, vector<char> &search, vector<char> &deepSearch, int deep, int *heuristic, Position pLT, Position pRB, char from)
{
    char better = 0;
    char move[] = {'w', 's', 'a', 'd'};
    if (deep == 0)
    {
        if(DEBUG) cout << "deep == 0" << endl;
        string candyStr = candy2String(candy);
        if(DEBUG) cout << "------------>" << endl;
        if(DEBUG) cout << candyStr << endl;
        if(DEBUG) printCandy(candy);
        if(DEBUG) cout << "<------------" << endl;
        if(DEBUG) cout << history.size() << endl;
        // for (std::vector<string>::iterator it = history.begin(); it != history.end(); ++it) {
        //     cout << *it << endl;
        // }
        if (find(history.begin(), history.end(), candyStr) == history.end())
        {
            int h = heuristicFunction(candy);
            if(DEBUG) cout << "get heuristic value:" << h << "    old is:" << *heuristic << endl;
            if (h < *heuristic)
            {
                if(DEBUG) cout << deepSearch.size() << "less than old heuristic:" << *heuristic << endl;
                *heuristic = h;
                search.clear();
                search.assign(deepSearch.begin(), deepSearch.end());
            }
        }
        return;
    }
    for (int approach = 0; approach < sizeof(move); approach++)
    {
        int d = deep;
        CANDY_ARRAY heuristicCandy;
        for (int i = 0; i < CANDY_ROW; i++)
        {
            for (int j = 0; j < CANDY_COLUMN; j++)
            {
                heuristicCandy[i][j] = candy[i][j];
            }
        }

        Position p1 = getPosition(heuristicCandy);
        Position p2 = p1;
        char ch = move[approach];
        if (ch == 'w')
        {
            if (p1.x == pLT.x || from == 's')
            {
                if(DEBUG) cout << "**Cannot go up!" << endl;
                continue;
            }
            else
            {
                p2.x--;
            }
        }
        else if (ch == 's')
        {
            if (p1.x == pRB.x || from == 'w')
            {
                if(DEBUG) cout << "**Cannot go down!" << endl;
                continue;
            }
            else
            {
                p2.x++;
            }
        }
        else if (ch == 'd')
        {
            if (p1.y == pRB.y || from == 'a')
            {
                if(DEBUG) cout << "**Cannot go right!" << endl;
                continue;
            }
            else
            {
                p2.y++;
            }
        }
        else if (ch == 'a')
        {
            if (p1.y == pLT.y || from == 'd')
            {
                if(DEBUG) cout << "**Cannot go left!" << endl;
                continue;
            }
            else
            {
                p2.y--;
            }
        }
        else if (ch == 'q')
        {
            cout << "QUIT" << endl;
        }
        deepSearch.push_back(ch);
        if(DEBUG) cout << "move " << ch << " and go deep" << d - 1 << endl;
        swapCandy(p1, p2, heuristicCandy);
        if(DEBUG) printCandy(heuristicCandy);
        betterSearchMove(heuristicCandy, history, search, deepSearch, --d, heuristic, pLT, pRB, ch);
        deepSearch.pop_back();
        if(*heuristic==0) break;
    }
}
void autoMoveCandy(CANDY_ARRAY candy, vector<string> &history)
{

    char ch;
    vector<char> search;
    vector<char> deepSearch;
    int steps = 0;
    string solution = "";
    // int heuristic = std::numeric_limits<int>::max();
    int heuristic = heuristicFunction(candy);
    int preHeuristic = heuristic;
    // if(heuristic > 3) {
    //     preHeuristic = 3;
    // } else 
    if(heuristic > 3) {
        preHeuristic = 2;
    } else {
        preHeuristic = 1;
    }
    int candyCount = countUniqueCharacters(candy2String(candy));
    do
    {
        // if(DEBUG || SHOW) printCandy(candy);
        int deep = 1;
        do
        {
            Position pLT;
            pLT.x = 0;
            pLT.y = 0;
            Position pRB;
            pRB.x = CANDY_ROW -1;
            pRB.y = CANDY_COLUMN-1;
            // Position pE = getPosition(candy);
            // if(pE.y > 0 && (candy[0][pE.y] != candy[CANDY_ROW-1][pE.y] || candy[0][pE.y-1] != candy[CANDY_ROW-1][pE.y-1])) {
            //     pLT.x = 0;
            //     pLT.y = pE.y-1;
            //     pRB.y = pE.y;
            //     pRB.x = CANDY_ROW-1;
            // } else if(pE.y < CANDY_COLUMN-1 && (candy[0][pE.y] != candy[CANDY_ROW-1][pE.y] || candy[0][pE.y+1]!= candy[CANDY_ROW-1][pE.y+1])) {
            //     pLT.x = 0;
            //     pLT.y = pE.y;
            //     pRB.y = pE.y+1;
            //     pRB.x = CANDY_ROW-1;
            //     printCandy(candy);
            //     cout << "------------------!!!!---------------------->" << heuristic << endl;
            // }
            if(DEBUG) cout << pLT.y << ":" << pRB.y << endl;
            if(DEBUG) cout << "search deep is:" << deep << "heuristic:" << heuristic << endl;
            search.clear();
            deepSearch.clear();
            betterSearchMove(candy, history, search, deepSearch, deep++, &heuristic, pLT, pRB, 0);
            if(candyCount <=4) {
                if(heuristic==0) break;
            } else {
                if(preHeuristic>heuristic) break;
            }
        // } while (preHeuristic<=heuristic);
        } while (true);//heuristic!=0);
        preHeuristic = heuristic;
        if (DEBUG) {
            cout << "get search path size:" << search.size() << endl;
            printCandy(candy);
        }
        for (std::vector<char>::iterator it = search.begin(); it != search.end(); ++it)
        {
            Position p1 = getPosition(candy);
            Position p2 = p1;
            char ch = *it;
            if(DEBUG) cout << "better move is: " << ch;
            if (ch == 'w')
            {
                if (p1.x == 0)
                {
                    if(DEBUG) cout << "**Cannot go up!" << endl;
                }
                else
                {
                    p2.x--;
                    steps++;
                    solution.push_back(getStep(p2));
                    if(DEBUG) cout << "UP!" << endl;
                }
            }
            else if (ch == 's')
            {
                if (p1.x == CANDY_ROW - 1)
                {
                    if(DEBUG) cout << "**Cannot go down!" << endl;
                }
                else
                {
                    p2.x++;
                    steps++;
                    solution.push_back(getStep(p2));
                    if(DEBUG) cout << "DOWN" << endl;
                }
            }
            else if (ch == 'd')
            {
                if (p1.y == CANDY_COLUMN - 1)
                {
                    if(DEBUG) cout << "**Cannot go right!" << endl;
                }
                else
                {
                    p2.y++;
                    steps++;
                    solution.push_back(getStep(p2));
                    if(DEBUG) cout << "RIGHT" << endl;
                }
            }
            else if (ch == 'a')
            {
                if (p1.y == 0)
                {
                    if(DEBUG) cout << "**Cannot go left!" << endl;
                }
                else
                {
                    p2.y--;
                    steps++;
                    solution.push_back(getStep(p2));
                    if(DEBUG) cout << "LEFT!" << endl;
                }
            }
            else if (ch == 'q')
            {
                if(DEBUG) cout << "QUIT" << endl;
            }
            totalSteps++;
            swapCandy(p1, p2, candy);
            history.push_back(candy2String(candy));
        }
        // if(checkMoveCandy(candy, 1, 0))
        if (checkCandy(candy))
        {
            if(DEBUG || SHOW) 
                printCandy(candy);
            if(DEBUG) cout << "Congratulations!! Spend:" << steps << " steps" << endl;
            if(DEBUG) cout << "Solution is " << solution << endl;
            milliseconds ms = duration_cast< milliseconds >(
                system_clock::now().time_since_epoch()
            );
            int finish = ms.count()-start;
            writeSoltion(solution, finish);
            break;
            
        }
    } while (ch != 'q');
}
/******APPROACH*******/
void writeApproach(string solution)
{
    ofstream file;
    file.open(output, std::ios_base::app);
    file << solution;
    file.close();
}
bool checkApproachCandy(const CANDY_ARRAY candy, int distance, int maxApproach)
{
    std::map<char, int> counting;
    std::pair<std::map<char, int>::iterator, bool> ret;
    int count = 0;
    if(DEBUG) printCandy(candy);
    for (int j = 0; j < CANDY_COLUMN; j++)
    {
        for (int i = 0; i < CANDY_ROW; i++)
        {
            if (candy[i][j] != 'e')
            {
                ret = counting.insert(std::pair<char, int>(candy[i][j], j));
                if (ret.second == false)
                {
                    if (j - ret.first->second <= distance)
                    {
                        count++;
                    }
                    counting.erase(candy[i][j]);
                }
            }
        }
    }
    return count >= maxApproach;
}
int approachHeuristicFunction(const CANDY_ARRAY candy, int distance, int maxApproach)
{
    std::map<char, int> counting;
    std::pair<std::map<char, int>::iterator, bool> ret;
    int heuristic = 0;
    int count = 0;
    for (int j = 0; j < CANDY_COLUMN; j++)
    {
        for (int i = 0; i < CANDY_ROW; i++)
        {
            if (candy[i][j] != 'e')
            {
                ret = counting.insert(std::pair<char, int>(candy[i][j], j));
                if (ret.second == false)
                {
                    int d = j - ret.first->second;
                    if(d <= distance) {
                        count++;
                    }
                    heuristic += (d);
                    counting.erase(candy[i][j]);
                    if(count == maxApproach) {
                        return heuristic+maxApproach-count;
                    }
                }
            }
        }
    }

    return heuristic+maxApproach-count;
}

void betterApproachMove(const CANDY_ARRAY candy, vector<string> history, vector<char> &search, vector<char> &deepSearch, int deep, int *heuristic, int distance, int maxApproach, char from)
{
    char better = 0;
    char move[] = {'w', 's', 'a', 'd'};
    if (deep == 0)
    {
        if(DEBUG) cout << "deep == 0" << endl;
        string candyStr = candy2String(candy);
        if (find(history.begin(), history.end(), candyStr) == history.end())
        {
            int h = approachHeuristicFunction(candy, distance, maxApproach);
            if(DEBUG) cout << "get heuristic value:" << h << "    old is:" << *heuristic << endl;
            if (h < *heuristic)
            {
                if(DEBUG) cout << h << "less than old heuristic:" << *heuristic << endl;
                *heuristic = h;
                search.clear();
                search.assign(deepSearch.begin(), deepSearch.end());
            }
        }
        return;
    }
    for (int approach = 0; approach < sizeof(move); approach++)
    {
        int d = deep;
        CANDY_ARRAY heuristicCandy;
        for (int i = 0; i < CANDY_ROW; i++)
        {
            for (int j = 0; j < CANDY_COLUMN; j++)
            {
                heuristicCandy[i][j] = candy[i][j];
            }
        }

        Position p1 = getPosition(heuristicCandy);
        Position p2 = p1;
        char ch = move[approach];
        if (ch == 'w')
        {
            if (p1.x == 0 || from == 's')
            {
                if(DEBUG) cout << "**Cannot go up!" << endl;
                continue;
            }
            else
            {
                p2.x--;
            }
        }
        else if (ch == 's')
        {
            if (p1.x == CANDY_ROW - 1 || from == 'w')
            {
                if(DEBUG) cout << "**Cannot go down!" << endl;
                continue;
            }
            else
            {
                p2.x++;
            }
        }
        else if (ch == 'd')
        {
            if (p1.y == CANDY_COLUMN - 1 || from == 'a')
            {
                if(DEBUG) cout << "**Cannot go right!" << endl;
                continue;
            }
            else
            {
                p2.y++;
            }
        }
        else if (ch == 'a')
        {
            if (p1.y == 0 || from == 'd')
            {
                if(DEBUG) cout << "**Cannot go left!" << endl;
                continue;
            }
            else
            {
                p2.y--;
            }
        }
        else if (ch == 'q')
        {
            if(DEBUG) cout << "QUIT" << endl;
        }
        deepSearch.push_back(ch);
        if(DEBUG) cout << ch << "go deep" << d - 1 << endl;
        swapCandy(p1, p2, heuristicCandy);
        betterApproachMove(heuristicCandy, history, search, deepSearch, --d, heuristic, distance, maxApproach, ch);
        deepSearch.pop_back();
    }
}
void autoApproachCandy(CANDY_ARRAY candy, vector<string> &history)
{
    char ch;
    int distance = CANDY_COLUMN - 1;
    int steps = 0;
    int maxApproach = CANDY_COLUMN;
    int candyCount = countUniqueCharacters(candy2String(candy));
    if(candyCount <=4) {
        maxApproach = CANDY_COLUMN;
    } else {
        maxApproach = CANDY_COLUMN+1;
    }
    string solution = "";
    vector<char> search;
    vector<char> deepSearch;
    for(int i=1;i<CANDY_COLUMN;i++) {
        if (checkApproachCandy(candy, i, maxApproach)) {
            if(i<=MINI_DISTANCE) {
                if(SHOW) printCandy(candy);
                return;
            } else {
                distance = i;
                break;
            }
        }
    }
    do
    {
        // if(DEBUG || SHOW) printCandy(candy);
        int deep = 1;
        int heuristic = std::numeric_limits<int>::max();
        do
        {
            if(DEBUG) cout << "search deep is:" << deep << endl;
            search.clear();
            deepSearch.clear();
            betterApproachMove(candy, history, search, deepSearch, deep++, &heuristic, distance, maxApproach, 0);
        // } while(search.size()==0);CANDY_COLUMN+1
        // } while (!checkApproachCandy(heuristicCandy, distance));
        } while (heuristic>CANDY_COLUMN*distance);//search.size() == 0);
        if(DEBUG) {
            cout << "get search path size:" << search.size() <<",deep:" << deep << ",distance:" << distance << endl;
            printCandy(candy);
        }
        for (std::vector<char>::iterator it = search.begin(); it != search.end(); ++it)
        {
            char ch = *it;
            Position p1 = getPosition(candy);
            Position p2 = p1;
            if(DEBUG) cout << "better Approach move is: " << ch;
            if (ch == 'w')
            {
                if (p1.x == 0)
                {
                    if(DEBUG) cout << "**Cannot go up!" << endl;
                }
                else
                {
                    p2.x--;
                    steps++;
                    solution.push_back(getStep(p2));
                    if(DEBUG) cout << "UP!" << endl;
                }
            }
            else if (ch == 's')
            {
                if (p1.x == CANDY_ROW - 1)
                {
                    if(DEBUG) cout << "**Cannot go down!" << endl;
                }
                else
                {
                    p2.x++;
                    steps++;
                    solution.push_back(getStep(p2));
                    if(DEBUG) cout << "DOWN" << endl;
                }
            }
            else if (ch == 'd')
            {
                if (p1.y == CANDY_COLUMN - 1)
                {
                    if(DEBUG) cout << "**Cannot go right!" << endl;
                }
                else
                {
                    p2.y++;
                    steps++;
                    solution.push_back(getStep(p2));
                    if(DEBUG) cout << "RIGHT" << endl;
                }
            }
            else if (ch == 'a')
            {
                if (p1.y == 0)
                {
                    if(DEBUG) cout << "**Cannot go left!" << endl;
                }
                else
                {
                    p2.y--;
                    steps++;
                    solution.push_back(getStep(p2));
                    if(DEBUG) cout << "LEFT!" << endl;
                }
            }
            else if (ch == 'q')
            {
                if(DEBUG) cout << "QUIT" << endl;
            }
            totalSteps++;
            swapCandy(p1, p2, candy);
            history.push_back(candy2String(candy));
        }

        if (checkApproachCandy(candy, distance, maxApproach))
        {
            if(DEBUG) cout << "Approach good!! Spend:" << steps << " steps" << endl;
            if(DEBUG) cout << "Approach solution is " << solution << endl;
            if (distance == MINI_DISTANCE || checkApproachCandy(candy, MINI_DISTANCE, maxApproach))
            {
                if(SHOW) printCandy(candy);
                writeApproach(solution);
                break;
            }
            distance--;
        }
    } while (ch != 'q');
}


void loadFileAutomaticlyMove(string input)
{
    string line;
    int lineNumber = 0;
    ifstream filestream(input);
    if (filestream.is_open())
    {
        while (getline(filestream, line))
        {
            cout << ++lineNumber << " Candy Problem:" << endl
                 << line << endl
                 << endl;
            CANDY_ARRAY candy;
            vector<string> history;
            initCandy(candy);
            string2Candy(line, candy);
            milliseconds ms = duration_cast< milliseconds >(
                system_clock::now().time_since_epoch()
            );
            start = ms.count();
            autoApproachCandy(candy, history);
            autoMoveCandy(candy, history);
        }
        writeApproach(std::to_string(totalSteps));
        filestream.close();
    }
    else
    {
        cout << "File opening is fail." << endl;
    }
}
// void candyCrisis(vector<string> lines)
// {
//     for (std::vector<string>::iterator it = lines.begin(); it != lines.end(); ++it)
//     {
//         string line = *it;
//         CANDY_ARRAY candy;
//         vector<string> history;
//         initCandy(candy);
//         string2Candy(line, candy);
//         milliseconds ms = duration_cast<milliseconds>(
//             system_clock::now().time_since_epoch());
//         start = ms.count();
//         autoApproachCandy(candy, history);
//         autoMoveCandy(candy, history);
//     }
// }
// void multiThreadLoadFileAutomaticlyMove(string input, int threadNumber) {
//     string line;
//     int lineNumber = 0;
//     ifstream filestream(input);
//     vector<string> line1;
//     vector<string> line2;
//     if (filestream.is_open())
//     {
//         while (getline(filestream, line))
//         {
//             cout << ++lineNumber << " Candy Problem:" << endl
//                  << line << endl
//                  << endl;
//             if(lineNumber%1==0) {
//                 line1.push_back(line);
//             } else {
//                 line2.push_back(line);
//             }
//         }
//         std::thread t1(candyCrisis, line1);
//         std::thread t2(candyCrisis, line2);
//         t1.join();
//         t2.join();
//         writeApproach(std::to_string(totalSteps));
//         filestream.close();
//     }
//     else
//     {
//         cout << "File opening is fail." << endl;
//     }
// }
int main(int argc, char **argv)
{
    std::cout << "Welcome to Candy Crisis!" << endl
              << "Type 'awsd' to move the empty tile" << std::endl;
    if (argc < 3)
    {
        std::cout << "Please input Candy file. e.g: candy Sample_Data.txt";
        return 0;
    }
    string input = argv[1];
    output = argv[2];
    if (argc == 4)
    {
        string automatic = argv[3];
        if (automatic == "auto")
        {
            loadFileAutomaticlyMove(input);
        }
    } else if(argc == 5) {
        int threadNumber = std::stoi(argv[4]);
        string automatic = argv[3];
        if (automatic == "auto")
        {
            // multiThreadLoadFileAutomaticlyMove(input, threadNumber);
        }
    } else
    {
        loadFile(input);
    }
    return 0;
}
