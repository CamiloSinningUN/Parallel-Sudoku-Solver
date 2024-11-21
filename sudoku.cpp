/*
 *  This file is part of Christian's OpenMP parallel Sudoku Solver
 *
 *  Copyright (C) 2013 by Christian Terboven <christian@terboven.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

#include "SudokuBoard.h"

// CONSTANTS
#define UNASSIGNED 0

// FUNCTIONS DEFINITION
// This function finds an entry in grid that is still unassigned
bool FindUnassignedLocation(CSudokuBoard *sudoku, int &row, int &col);
// Returns a boolean which indicates whether it will be legal to assign num to the given row, col, and box location
bool UsedInRow(CSudokuBoard *sudoku, int row, int num);
bool UsedInCol(CSudokuBoard *sudoku, int col, int num);
bool UsedInBox(CSudokuBoard *sudoku, int boxStartRow, int boxStartCol, int num);
// Checks whether it will be legal to assign num to the given row, col
bool isSafe(CSudokuBoard *sudoku, int row, int col, int num);
// Assigns values to all unassigned locations in such a way to meet the requirements for Sudoku solution
void SolveSudoku(CSudokuBoard *sudoku, int row, int col, int depth, int max_depth);

int found_sudokus = 0;

int main(int argc, char *argv[])
{
	int max_depth = argc > 4 ? atoi(argv[4]) : 3;

	// measure the time
	double t3, t4;

	// expect three command line arguments: field size, block size, and input file
	if (argc != 5)
	{
		std::cout << "Usage: sudoku.exe <field size> <block size> <input filename> <max_depth>" << std::endl;
		std::cout << std::endl;
		return -1;
	}
	else
	{
		CSudokuBoard *sudoku1 = new CSudokuBoard(atoi(argv[1]), atoi(argv[2]));
		if (!sudoku1->loadFromFile(argv[3]))
		{
			std::cout << "There was an error reading a Sudoku template from " << argv[3] << std::endl;
			std::cout << std::endl;
			return -1;
		}

		// print the Sudoku board template
		std::cout << "Given Sudoku template" << std::endl;
		sudoku1->printBoard();

		// solve the Sudoku by finding (and printing) all solutions
		t3 = omp_get_wtime();

#pragma omp parallel
		{
#pragma omp single
			{
				// IMPLEMENTATION OF THE PARALLEL SUDOKU SOLVER
				SolveSudoku(sudoku1, 0, 0, 0, max_depth);
				// END OF IMPLEMENTATION
			}
		}

		t4 = omp_get_wtime();

		delete sudoku1;
	}

	// print the time
	double time_taken = t4 - t3;
	int threads_used = omp_get_max_threads();
	std::cout << "Parallel computation took " << time_taken << " seconds (" << threads_used << " threads)." << std::endl
			  << std::endl;

	// Save the data to a CSV file
	// Create the output directory if it does not exist
	// TODO: write also max_depth
	std::ofstream csv_file;
	csv_file.open("../output/times.csv", std::ios::app);
	if (csv_file.is_open())
	{
		// Check if the file is empty
		csv_file.seekp(0, std::ios::end);
		if (csv_file.tellp() == 0)
		{
			// Write the header if the file is empty
			csv_file << "time,grid_size,block_size,threads_used,solutions,max_depth\n";
		}
		// Append the data
		csv_file << time_taken << "," << argv[1] << "," << argv[2] << "," << threads_used << "," << found_sudokus << "," << max_depth << "\n";
		csv_file.close();
	}
	else
	{
		std::cerr << "Unable to open file for writing." << std::endl;
	}

	return 0;
}

// FUNCTIONS IMPLEMENTATION
bool FindUnassignedLocation(CSudokuBoard *sudoku, int &row, int &col)
{
	for (row = 0; row < sudoku->getFieldSize(); row++)
		for (col = 0; col < sudoku->getFieldSize(); col++)
			if (sudoku->get(row, col) == UNASSIGNED)
				return true;
	return false;
}

bool UsedInRow(CSudokuBoard *sudoku, int row, int num)
{
	for (int col = 0; col < sudoku->getFieldSize(); col++)
		if (sudoku->get(row, col) == num)
			return true;
	return false;
}

bool UsedInCol(CSudokuBoard *sudoku, int col, int num)
{
	for (int row = 0; row < sudoku->getFieldSize(); row++)
		if (sudoku->get(row, col) == num)
			return true;
	return false;
}

bool usedInBox(CSudokuBoard *sudoku, int boxStartRow, int boxStartCol, int num)
{
	for (int row = 0; row < sudoku->getBlockSize(); row++)
		for (int col = 0; col < sudoku->getBlockSize(); col++)
			if (sudoku->get(row + boxStartRow, col + boxStartCol) == num)
				return true;
	return false;
}

bool isSafe(CSudokuBoard *sudoku, int row, int col, int num)
{
	return !UsedInRow(sudoku, row, num) && !UsedInCol(sudoku, col, num) && !usedInBox(sudoku, row - row % sudoku->getBlockSize(), col - col % sudoku->getBlockSize(), num) && sudoku->get(row, col) == UNASSIGNED;
}

void SolveSudoku(CSudokuBoard *sudoku, int row, int col, int depth, int max_depth)
{
	// Check if we have reached the end of the board
	int BOARD_SIZE = sudoku->getFieldSize();
	int abs_index = row * BOARD_SIZE + col;

	if (abs_index >= BOARD_SIZE * BOARD_SIZE)
	{
		found_sudokus++;
		std::cout << "Solution " << found_sudokus << std::endl;
		sudoku->printBoard();
		sudoku->incrementSolutionCounter();
		return;
	}

	// Calculate the next cell
	int row_next = (abs_index + 1) / BOARD_SIZE;
	int col_next = (abs_index + 1) % BOARD_SIZE;

	if (sudoku->get(row, col) != UNASSIGNED)
	{
		// If the cell is already filled, proceed to the next cell
		SolveSudoku(sudoku, row_next, col_next, depth, max_depth);
	}
	else
	{
		// Iterate through all possible numbers for the current cell
		for (int num = 1; num <= BOARD_SIZE; ++num)
		{
			if (isSafe(sudoku, row, col, num))
			{
				if (depth < max_depth)
				{
// Parallel task creation for shallower recursion levels
#pragma omp task firstprivate(num)
					{
						// Work on a new copy of the board
						CSudokuBoard *childSudoku = new CSudokuBoard(*sudoku);
						childSudoku->set(row, col, num);

						SolveSudoku(childSudoku, row_next, col_next, depth + 1, max_depth);

						delete childSudoku; // Clean up memory
					}
				}
				else
				{
					// Avoid task creation if we are too deep
					sudoku->set(row, col, num);

					SolveSudoku(sudoku, row_next, col_next, depth + 1, max_depth);

					sudoku->set(row, col, UNASSIGNED); // Reset for backtracking
				}
			}
		}
	}

#pragma omp taskwait
}
