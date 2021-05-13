
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <random>

typedef struct GridType
{
    int col;
    int row;
    int* data;
};

GridType* MakeGrid(int col, int row)
{
    GridType* grid = (GridType*)malloc(sizeof(GridType));
    grid->data = (int*)malloc (sizeof(int)*col*row);
    grid->col = col;
    grid->row = row;
    for (int i =0; i < grid->col * grid->row ; i++)
        grid->data[i] = 0;
    return grid;
}

void FillGrid(GridType* grid, int j)
{
    for (int i =0; i < (grid->col * grid->row) ; i++)
        grid->data[i] = j;
}

int ReadGrid(GridType* grid, int col, int row)
{
    if (grid == NULL)
        return NULL;

    if (col >= grid->col || row >= grid->row)
        return NULL;

    return grid->data[(row * (grid->row-1))+col];
}

void WriteGrid(GridType*grid, int col, int row, int i)
{
    if (grid == NULL)
        return;

    if (col >= grid->col || row >= grid->row)
        return;

    grid->data[(row * (grid->row-1))+col] = i;

}

void PrintGrid(GridType* grid)
{
    for (int i =0 ; i < grid->col ; i++)
    {
        for (int j = 0 ; j < grid->row; j++)
        {
            std::cout << i <<":" << j << "=" << ReadGrid(grid,i,j)<<std::endl;
        }

    }
}

int CountNeighbour(GridType* grid, int x, int y)
{
    int n=0;

    for (int i = -1 ; i < 2 ; i++)
    {
        for (int j = -1 ; j < 2 ; j++)
        {
            n += ReadGrid(grid,(x+i+grid->col)%grid->col,(y+j+grid->row)%grid->row);
        }
    }
    n -= ReadGrid(grid,x,y);
    return n;
}

void CopyGrid(GridType* dst, GridType* src)
{

    for (int i =0 ; i < src->col ; i++)
    {
        for (int j = 0  ; j < src->row ; j++)
        {
            WriteGrid(dst,i,j,ReadGrid(src,i,j));
        }
    }
}
int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 700), "Game of Life");


    //int resolution = 10;
    int col = 50;//window.getSize().x/resolution;
    int row = 50;//window.getSize().y/resolution;

    const int CellWidth = 10;
    const int CellSpacing = 2;

    sf::Vector2f CellSize (10.0f,10.0f);
    sf::Vector2f position (0.0f,0.0f);


    sf::RectangleShape Cell(CellSize);


    Cell.setPosition(sf::Vector2f(10.0f,10.0f));
    Cell.setFillColor(sf::Color::White);


    GridType* grid = MakeGrid(col,row);
    GridType* NextGrid = MakeGrid(col,row);
    FillGrid(NextGrid,0);
    FillGrid(grid,0);

    time_t seed;
    srand(time(&seed));

    for (int i = 0 ; i < grid->col * grid->row ; i++)
    {
        grid->data[i]=rand()%2;
    }

    int neighbour =0;
    float x = 0.0f; float y =0.0f;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Q)
                    window.close();

                if (event.key.code == sf::Keyboard::R)
                 {
                    srand(time(&seed));
                    for (int i = 0 ; i < grid->col * grid->row ; i++)
                    {
                        grid->data[i]=rand()%2;
                    }
                 }0

            }

        }

        for (int i =0; i < col; i++)
        {
            for (int j =0; j < row ;j++ )
            {
                neighbour =  CountNeighbour(grid,i,j);

                int state = ReadGrid(grid,i,j);

                /*if (i == 0 || i == col-1 || j == 0 || j == row-1)
                {
                    WriteGrid(NextGrid,i,j,state);
                }
                else*/
                //{
                    if (state == 0 && neighbour ==3)
                    {
                        WriteGrid(NextGrid,i,j,1);
                    }
                    else if (state == 1 && (neighbour < 2 || neighbour > 3))
                    {
                        WriteGrid(NextGrid,i,j,0);
                    }
                    else
                    {
                        WriteGrid(NextGrid,i,j,state);
                    }
                //}
            }
        }


        window.clear();

        for (int i =0; i < col; i++)
        {
            for (int j =0; j < row ;j++ )
            {
                x = i * (CellWidth + CellSpacing);
                y = j * (CellWidth + CellSpacing);


                if (ReadGrid(NextGrid,i,j)==1)
                    Cell.setFillColor(sf::Color::Red);
                else if (ReadGrid(NextGrid,i,j)==0)
                    Cell.setFillColor(sf::Color::White);

                Cell.setPosition(x,y);

                window.draw(Cell);
            }
        }

        CopyGrid(grid,NextGrid);
        window.display();
        sf::sleep(sf::milliseconds(20)) ;
    }

    return 0;
}
