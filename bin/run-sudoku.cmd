@echo off

setlocal enabledelayedexpansion

cd /d %~dp0

set THREADS=01;02;04;06;08

for %%t in (%THREADS%) do (
  set OMP_NUM_THREADS=%%t
  sudoku.exe 9 3 sudoku-9x9.txt
@REM   set OMP_NUM_THREADS=%%t
@REM   sudoku.exe 16 4 sudoku-16x16.txt
@REM   set OMP_NUM_THREADS=%%t
@REM   sudoku.exe 25 5 sudoku-25x25.txt
)

endlocal