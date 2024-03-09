// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header!

#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;

//声明函数
void get_filename(string&);
void read_map(string ,Vector<char>&);
string get_tips();
void nomal_model(Grid<char>&);
void simulation(Grid<char>&);
void translate_map(int&,int &,Vector<char>&,Grid<char>&);
void get_mapsize(int &,int &,string&);
void print_map(Grid<char>);
void operator_map(int ,int ,Vector<char>&);
void game_start(Grid<char>&);
void animate_nomal(int,Grid<char>&);
int get_frames();
int find_neighbors_nomal(int,int,Grid<char>&);
void record_neighbors(Vector<int>&,Grid<char>&);
void nomal_algorithrns(Grid<char>&);
void modify_map (int,int, Grid<char>&,int);
void tick_nomal(Grid<char>&);
int find_neighbors_simulation (int,int,Grid<char>&);
void simulation_algorithrns(Grid<char>&);
void animate_simution(int,Grid<char> &);
//-----

int main() {
    // TODO: Finish the program!
    string filename;   		//获取文件名称

    int columns,rows;   	//创建存放地图大小的变量

    Vector<char> map_temp;  	//创将临时存放地图的向量

    Grid <char> map; 	 	//创建地图网格

    //得到文件名称，若用户输入错误则重新输入
    get_filename(filename);

    //读取文件中的内容，将其放入向量
    read_map(filename,map_temp);

    //获取地图大小
    get_mapsize(columns,rows,filename);

    //处理地图细节
    operator_map(columns,rows,map_temp);

    //将向量中的地图放入网格中
    translate_map(rows,columns,map_temp,map);

    //获取提示符号
    game_start(map);

    cout << "Have a nice Life!" << endl;
    return 0;
}

//获取文件名
void get_filename(string& filename) {
    do {
    getLine("Grid input file name?",filename);
    } while(!fileExists(filename));
}

//得到地图大小(利用向量)
void get_mapsize(int &columns,int &rows,string &filename) {
    int line = 0;
    fstream input;
    string temp;
    Vector<int> size_temp;
    input.open(filename);
    if (input.fail())
    {
        cout << "something worng" << endl;
    }
    else
    {
        while (line < 2 && getline(input,temp))
        {
            int n = stringToInteger(temp);
            size_temp.add(n);
            line++;
        }

    }
    rows = size_temp.get(0);
    columns = size_temp.get(1);
}

//将向量里的地图复制到网格中
void translate_map(int &rows,int &columns,Vector<char>&map_temp,Grid<char>&map) {
    map.resize(rows,columns);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            map[i][j] = map_temp[i * columns + j];
        }
    }
}

//处理地图细节
void operator_map(int columns,int rows,Vector<char> &map) {
    string t = integerToString(rows) + integerToString(columns);
    int n = t.size();
    for (int i = 0; i < n; i++)
    {
        map.remove(0);
    }
}

//在文件中获取地图
void read_map(string filename, Vector<char>& map)
{
    fstream input;
    char temp;
    string size;
    input.open(filename);
    if (input.fail())
    {
        cout <<"something was worng" << endl;
    }
    else
    {
        while (input >> temp)
        {
            map.add(temp);
        }
    }
    input.close();
}

//获取提示符
string get_tips() {
    string key;
    do {
        getLine("a)nimate, t)ick, q)uit?",key);
        key = toLowerCase(key);
    } while (key.compare("a") && key.compare("t") && key.compare("q"));
    return key;
}


//环绕网格
void simulation(Grid<char> &map) {
    string key;
    print_map(map);
    key = get_tips();
    if (!key.compare("a"))
    {
        animate_simution(get_frames(),map);
    }
    else if (!key.compare("t"))
    {
        tick_nomal(map);
    }
}

//获得帧数
int get_frames() {
    string frames;
    getLine("How Many frames? ",frames);
    return stringToInteger(frames);
}

//动画化输出
void animate_nomal(int frames,Grid<char> &map) {
    int count = 0;
    while (count < frames)
    {
        nomal_algorithrns(map);
        print_map(map);
        pause(50);
        clearConsole();
        count++;
    }
    nomal_model(map);
}

void animate_simution(int frames,Grid<char> &map) {
    int count = 0;
    while (count < frames)
    {
        simulation_algorithrns(map);
        print_map(map);
        pause(50);
        clearConsole();
        count++;
    }
    simulation(map);
}

//启动游戏
void game_start(Grid<char> &map) {
    string key;
    do {
        getLine("should the simuylation warp around the grid (y/n)",key);
        key =  toLowerCase(key);
    } while (key.compare("y") && key.compare("n"));

    if (!key.compare("y"))
    {
        simulation(map);
    }
    else
    {
        nomal_model(map);
    }
}

//游戏环绕算法实现
void nomal_algorithrns(Grid<char>&map) {
    Vector<int> neighbors_count;
    for (int i = 0; i < map.height(); i++)
    {
        for (int j = 0; j < map.width(); j++)
        {
            neighbors_count.add(find_neighbors_nomal(i,j,map));
        }
    }
    record_neighbors(neighbors_count,map);
}

void simulation_algorithrns(Grid<char>&map) {
    Vector<int> neighbors_count;
    for (int i = 0; i < map.height(); i++)
    {
        for (int j = 0; j < map.width(); j++)
        {
            neighbors_count.add(find_neighbors_simulation(i,j,map));
        }
    }
    record_neighbors(neighbors_count,map);
}


//记录细胞群
void record_neighbors(Vector<int>&neighbors_counts,Grid<char>&map) {
    int rows = map.height();
    int columns = map.width();
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            modify_map(i,j,map,neighbors_counts[j + i * columns]);
        }
    }
}

//修改地图细胞
void modify_map (int rows ,int columns, Grid<char> &map,int neighbors) {
    if (map[rows][columns] == 'X')
    {
        neighbors--;
    }
    if (neighbors == 0 || neighbors == 1)
    {
        map[rows][columns] = '-';
    }
    else if (neighbors == 3)
    {
        map[rows][columns] = 'X';
    }
    else if (neighbors >= 4)
    {
        map[rows][columns] = '-';
    }
}
//寻找邻居数目
int find_neighbors_nomal(int rows,int columns,Grid<char> &map) {
    int neighbors = 0;
    for (int i = rows - 1; i <= rows + 1; i++)
    {
        for (int j = columns - 1; j <= columns + 1; j++)
        {
            if (map.inBounds(i,j)&& map[i][j] == 'X')
            {
                neighbors++;
            }
        }
    }
    return neighbors;
}

int find_neighbors_simulation (int rows,int columns,Grid<char> &map)
{
    //
    int neighbors = 0;
    int R_columns = map.width();
    int R_rows = map.height();
    for (int i = rows - 1; i <= rows + 1; i++)
    {
        for (int j = columns - 1; j <= columns + 1; j++)
        {
            int temp1 = (i + R_rows) % R_rows;
            int temp2 = (j + R_columns) % R_columns;
            if (map[temp1][temp2] == 'X')
            {
                neighbors++;
            }
        }
    }
    return neighbors;
}

//不环绕网格
void nomal_model(Grid<char>&map) {
    string key;
    print_map(map);
    key = get_tips();
    if (!key.compare("a"))
    {
        animate_nomal(get_frames(),map);
    }
    else if (!key.compare("t"))
    {
        tick_nomal(map);
    }
}

//输出下一代(静态的)
void tick_nomal(Grid<char>& map)  {
    nomal_algorithrns(map);
    nomal_model(map);
}

//打印地图
void print_map(Grid<char> map) {
    for (int i = 0; i < map.height(); i++)
    {
        for (int j = 0; j < map.width(); j++)
        {
            cout << map[i][j];
        }
        cout << endl;
    }
}



