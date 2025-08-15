@echo off
echo MinGW Installation and Setup Guide
echo ===============================
echo.
echo This file contains instructions to install and set up MinGW (GCC for Windows)
echo Follow these steps to get your C compiler working:
echo.
echo Step 1: Download MinGW
echo -----------------
echo Visit: https://sourceforge.net/projects/mingw/
echo Click "Download" to get the MinGW installer
echo.
echo Step 2: Install MinGW
echo ----------------
echo Run the downloaded installer
echo When prompted, select at minimum:
echo   - mingw32-base
echo   - mingw32-gcc-g++
echo.
echo Step 3: Add MinGW to your PATH
echo ------------------------
echo 1. Right-click on "This PC" or "My Computer" and select "Properties"
echo 2. Click on "Advanced system settings"
echo 3. Click "Environment Variables"
echo 4. Under "System variables", find and select "Path", then click "Edit"
echo 5. Click "New" and add: C:\MinGW\bin
echo 6. Click "OK" on all dialogs
echo.
echo Step 4: Verify Installation
echo ----------------------
echo Restart VS Code, then open a new terminal and type:
echo   gcc --version
echo.
echo If you see version information, GCC is properly installed!
echo.
echo After completing these steps, you should be able to compile your C program using:
echo   gcc -o student_grading_system student_grading_system.c
echo.
pause
