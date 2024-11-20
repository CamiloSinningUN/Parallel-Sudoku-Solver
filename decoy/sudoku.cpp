#include <iostream>
#include <vector>
#include <omp.h>

#define N 9

bool isSafe(int grid[N][N], int row, int col, int num)
{
    for (int x = 0; x < N; x++)
    {
        if (grid[row][x] == num || grid[x][col] == num)
        {
            return false;
        }
    }
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (grid[i + startRow][j + startCol] == num)
            {
                return false;
            }
        }
    }
    return true;
}

bool solveSudoku(int grid[N][N], int row, int col)
{
    if (row == N - 1 && col == N)
    {
        return true;
    }
    if (col == N)
    {
        row++;
        col = 0;
    }
    if (grid[row][col] > 0)
    {
        return solveSudoku(grid, row, col + 1);
    }

    bool solved = false;
#pragma omp parallel
    {
#pragma omp single nowait
        {
            for (int num = 1; num <= N; num++)
            {
#pragma omp task firstprivate(num)
                {
                    if (isSafe(grid, row, col, num))
                    {
                        grid[row][col] = num;
                        if (solveSudoku(grid, row, col + 1))
                        {
                            solved = true;
                        }
                        grid[row][col] = 0;
                    }
                }
            }
        }
    }
#pragma omp taskwait
    return solved;
}

void printGrid(int grid[N][N])
{
    for (int r = 0; r < N; r++)
    {
        for (int d = 0; d < N; d++)
        {
            std::cout << grid[r][d] << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    int grid[N][N] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}};

    if (solveSudoku(grid, 0, 0))
    {
        printGrid(grid);
    }
    else
    {
        std::cout << "No solution exists" << std::endl;
    }

    return 0;
}