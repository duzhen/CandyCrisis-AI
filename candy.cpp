#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>

#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_ESCAPE 27

#define CANDY_COLUMN 5
#define CANDY_ROW 3
using CANDY_ARRAY = char[CANDY_ROW][CANDY_COLUMN];

using namespace std;
typedef struct
{
    unsigned int x = 0;
    unsigned int y = 0;
} Position;

void printCandy(CANDY_ARRAY candy)
{
    for (int i = 0; i < CANDY_ROW; i++)
    {
        for (int j = 0; j < CANDY_COLUMN; j++)
        {
            std::cout << candy[i][j] << "  ";
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

char getStep(Position p) {
    char steps[][5] = {{'A','B','C','D','E'},{'F','G','H','I','J'},{'K','L','M','N','O'}};
    return steps[p.x][p.y];
}

void writeSoltion(string solution, int time) {
    ofstream file;
    file.open("output.txt", std::ios_base::app);
    file << solution;
    file << "\n";
    file << time;
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
                cout << "**Cannot go up!" << endl;
            }
            else
            {
                p2.x--;
                steps ++;
                solution.push_back(getStep(p2));
                cout << "UP!" << endl;
            }
        }
        else if (ch == 's')
        {
            if (p1.x == CANDY_ROW - 1)
            {
                cout << "**Cannot go down!" << endl;
            }
            else
            {
                p2.x++;
                steps ++;
                solution.push_back(getStep(p2));
                cout << "DOWN" << endl;
            }
        }
        else if (ch == 'd')
        {
            if (p1.y == CANDY_COLUMN - 1)
            {
                cout << "**Cannot go right!" << endl;
            }
            else
            {
                p2.y++;
                steps ++;
                solution.push_back(getStep(p2));
                cout << "RIGHT" << endl;
            }
        }
        else if (ch == 'a')
        {
            if (p1.y == 0)
            {
                cout << "**Cannot go left!" << endl;
            }
            else
            {
                p2.y--;
                steps ++;
                solution.push_back(getStep(p2));
                cout << "LEFT!" << endl;
            }
        }else if (ch == 'q') {
            cout << "QUIT" << endl;
        }
        swapCandy(p1, p2, candy);
        if (checkCandy(candy))
        {
            cout << "Congratulations!! Spend:" << steps << " steps" << endl;
            cout << "Solution is " << solution << endl;
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
            cout << "Candy Problem:" << line << endl <<endl;
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
int main(int argc, char **argv)
{
    std::cout << "Welcome to Candy Crisis!" << endl << "Type 'awsd' to move the empty tile" << std::endl;
    if(argc < 2) {
        std::cout << "Please input Candy file. e.g: candy Sample_Data.txt";
        return 0;
    }
    string input = argv[1];
    loadFile(input);
    return 0;
}
