@echo off

setlocal enabledelayedexpansion

cd /d %~dp0

set THREADS=01;02;04;06;08;10;12;14;16;18;20;22;24;26;28;30;32;34;36;38;40;42;44;46;48

for %%t in (%THREADS%) do (
  set OMP_NUM_THREADS=%%t
  sudoku.exe 9 3 sudoku-9x9.txt
  rem set OMP_NUM_THREADS=%%t
  rem sudoku.exe 16 4 sudoku-16x16.txt
  rem set OMP_NUM_THREADS=%%t
  rem sudoku.exe 25 5 sudoku-25x25.txt
)

endlocal