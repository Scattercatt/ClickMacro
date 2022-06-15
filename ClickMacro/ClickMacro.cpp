
/*
    Gavin Green
    6/15/2022
    https://github.com/Scattercatt/ClickMacro
    gavinr11213@gmail.com

    ClickMacro

    This is just a simple little program that can be useful when needing the schedule timed clicks on your computer. Very lightweight. Here is the general roadmap:

    - Ability to save macros
    - Ability to add keyboard strokes
    - GUI potentially?
    - Right clicks
    - Any kind of inputs (?)
    - ???


*/

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <stdint.h>


using namespace std;

bool ctrlKeyDetect(const char c);
void moveMouse(long x, long y);
void moveMouseTo(double x, double y);
void clickMouse();

void drive();

int main()
{
    drive();
}

void drive()
{
    cout << "To begin, hold a click on the same position for 2 seconds to mark a click location.\nTo start the macro, enter Ctrl+X.\n";

    const short MAX_CLICKS = 50;

    //These store the various clicks and sleep times
    POINT clicks[MAX_CLICKS];
    int sleepTimes[MAX_CLICKS];

    int recordedClicks = 0;
    
    while (recordedClicks < MAX_CLICKS)
    {
        if (GetAsyncKeyState(VK_LBUTTON))
        {
            
            POINT p1;
            GetCursorPos(&p1);

            Sleep(2000);

            POINT p2; 
            GetCursorPos(&p2);
            //The above 5 lines essentially record if the user has held their mouse in the same spot for 2 seconds.
            //This if statement tests it and begins creating a record if it was sucessful. 
            if (GetAsyncKeyState(VK_LBUTTON) && p1.x == p2.x && p1.y == p2.y)
            {
                clicks[recordedClicks] = { p1.x, p1.y };
                
                cout << "Input entered.\nPlease enter in milliseconds how long the program should wait after this click at (" << p1.x << ", " << p1.y << ") (Enter -1 to cancel input) : ";
                
                //Input validation
                int input;
                while (!(cin >> input))
                {
                    cout << "Input must be an integer value. Try again: ";
                }

                if (input >= 0)
                {
                    sleepTimes[recordedClicks] = input;
                    recordedClicks++;
                    cout << "Input sucessfully added.\n";
                }
                else
                    cout << "Input canceled\n";
                
            }
        }

        //Detect when the user enters Ctrl+X 
        else if (ctrlKeyDetect('X'))
        {
            if (recordedClicks == 0)
                cout << "No instructions have been inputed. Resuming input sequence.\n";
            else
                break;
        }
    }
    cout << "Instructions: \n";

    //Display given instructions
    for (int i = 0; i < recordedClicks; i++)
    {
        cout << "Click at (" << clicks[i].x << ", " << clicks[i].y << "), wait for " << sleepTimes[i] << "ms. \n";
    }

    //Info to user
    cout << "\n[Hold Ctrl+Q to terminate the program]\n[Hold Ctrl+P to pause]\n";


    //Execution sequence
    bool cont = true;
    while (cont)
    {
        for (int i = 0; i < recordedClicks; i++)
        {
            //Exit key detection
            if (ctrlKeyDetect('Q'))
            {
                cont = false;
                break;
            }

            //Pause key detection
            else if (ctrlKeyDetect('P'))
            {
                cout << "Program paused\n[Press Ctrl+R to resume]\n";
                while (!ctrlKeyDetect('R'))
                    Sleep(1);
                cout << "Unpaused [Hold Ctrl+Q to terminate. Hold Ctrl+P to pause]\n";
            }

            //Execution
            moveMouseTo((double) clicks[i].x, (double) clicks[i].y);
            clickMouse();
            Sleep(sleepTimes[i]);
        }
    }
    cout << "Program terminated";

}

//Simple moveMouse function. This is the relative movement function. Currently unused. 
void moveMouse(long x, long y)
{
    int mouseSpeed = 0;
    SystemParametersInfo(SPI_GETMOUSESPEED, 0, &mouseSpeed, 0);

    INPUT ip = { 0 };


    ip.type = INPUT_MOUSE;
    ip.mi.dwFlags = MOUSEEVENTF_MOVE;
    ip.mi.dwExtraInfo = 0x200;
    ip.mi.dx = static_cast<int>(10.0 / static_cast<double>(mouseSpeed) * x);
    ip.mi.dy = static_cast<int>(10.0 / static_cast<double>(mouseSpeed) * y);

    SendInput(1, &ip, sizeof(INPUT));
    ZeroMemory(&ip, sizeof(ip));
}

//Simple mouseMoveTo function. This is absolute mouse movement. 
void moveMouseTo(double x, double y)
{
    INPUT ip = { 0 };


    ip.type = INPUT_MOUSE;
    ip.mi.dx = x * (65536.0 / GetSystemMetrics(SM_CXSCREEN));
    ip.mi.dy = y * (65536.0 / GetSystemMetrics(SM_CYSCREEN));
    ip.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    ip.mi.mouseData = 0;
    ip.mi.time = 0;
    
    SendInput(1, &ip, sizeof(INPUT));

    ZeroMemory(&ip, sizeof(ip));
}

//Self explainatory. 
void clickMouse()
{
    INPUT ip = { 0 };
    ip.type = INPUT_MOUSE;
    ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    SendInput(1, &ip, sizeof(INPUT));

    Sleep(1);

    ip.mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(1, &ip, sizeof(INPUT));

    ZeroMemory(&ip, sizeof(ip));
}

//Shortcut function to detect if a character is being held in unison with the left control key. 
bool ctrlKeyDetect(const char c)
{
    return (GetAsyncKeyState(c) && GetAsyncKeyState(VK_LCONTROL));
}