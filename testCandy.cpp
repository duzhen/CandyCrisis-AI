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
#include <cmath>

using namespace std;
using namespace std::chrono;

#define CANDY_COLUMN 5
#define CANDY_ROW 3
using CANDY_ARRAY = char[CANDY_ROW][CANDY_COLUMN];
typedef struct
{
    int x = 0;
    int y = 0;
} Position;
void printCandy(const CANDY_ARRAY candy)
{
    for (int i = 0; i < CANDY_ROW; i++)
    {
        for (int j = 0; j < CANDY_COLUMN; j++)
        {
            std::cout << candy[i][j] << "  ";
            if(candy[i][j] <97 || candy[i][j]>122) exit(0);
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
Position getStepPosition(char step) {
    Position p;
    char steps[][5] = {{'A', 'B', 'C', 'D', 'E'}, {'F', 'G', 'H', 'I', 'J'}, {'K', 'L', 'M', 'N', 'O'}};
    for (int i = 0; i < CANDY_ROW; i++)
    {
        for (int j = 0; j < CANDY_COLUMN; j++)
        {
            if(step == steps[i][j]) {
                p.x = i;
                p.y = j;
                return p;
            }
        }
    }
}
void testCandy(string input, vector<string> solution) {
    string line;
    int lineNumber = 0;
    ifstream filestream(input);
    if (filestream.is_open())
    {
        while (getline(filestream, line))
        {
            CANDY_ARRAY candy;
            vector<string> history;
            initCandy(candy);
            string2Candy(line, candy);
            string s = solution.at(lineNumber);
            cout << line << endl << s << endl;
            for ( std::string::iterator it=s.begin(); it!=s.end(); ++it) {
                // cout << *it;
                Position p = getStepPosition(*it);
                Position pe = getPosition(candy);
                // cout << p.x << p.y << pe.x << pe.y << endl;
                if(std::abs(p.x-pe.x) > 1 || std::abs(p.y-pe.y) > 1) {
                    cout << std::abs(p.x-pe.x) << " Failed!!" << endl;
                    exit(-1);
                }
                swapCandy(pe, p, candy);
            }
            ++lineNumber;
            printCandy(candy);
            if(checkCandy(candy)) {
                cout << lineNumber << " Passed!" << endl;
            }
            
        }
        filestream.close();
    }
    else
    {
        cout << "File opening is fail." << endl;
    }
}
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "Please input Candy file. e.g: testCandy input.txt solution.txt";
        return 0;
    }
    string line;
    string input = argv[1];
    string solution = argv[2];
    vector<string> solutions;
    ifstream filestream(solution);
    if (filestream.is_open())
    {
        while (getline(filestream, line))
        {
            solutions.push_back(line);
            getline(filestream, line);
        }
        testCandy(input, solutions);
    }
    return 0;
}