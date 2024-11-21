@echo off

setlocal enabledelayedexpansion

cd /d %~dp0

set THREADS=01;02;04;06;08;10;12

@REM Making data statistically significant
for /L %%i in (1,1,3) do (
    @REM Try different max_depth
    @REM for %%j in (3,5,7,9) do (
        for %%t in (%THREADS%) do (
            @REM set OMP_NUM_THREADS=%%t
            @REM sudoku.exe 9 3 sudoku-9x9.txt %%j
            @REM set OMP_NUM_THREADS=%%t
            @REM sudoku.exe 16 4 sudoku-16x16.txt %%j
            set OMP_NUM_THREADS=%%t
            sudoku.exe 25 5 sudoku-25x25.txt 3
        )
    @REM )
)

endlocal