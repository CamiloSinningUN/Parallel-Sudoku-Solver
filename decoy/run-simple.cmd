@echo off
:: Set the executable name
set EXE_NAME=openmp_tasks.exe

:: List of thread counts
set THREAD_COUNTS=01 02 04 06 08 10 12 14 16

:: Loop through each thread count and execute the program
for %%T in (%THREAD_COUNTS%) do (
    echo Running with %%T threads:
    %EXE_NAME% %%T
    echo ----------------------------------------
)
pause
