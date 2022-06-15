

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <stdint.h>


using namespace std;

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

    POINT clicks[20];
    int sleepTimes[20];

    int recordedClicks = 0;
    
    while (recordedClicks < 20)
    {
        if (GetAsyncKeyState(VK_LBUTTON))
        {
            
            POINT p1;
            GetCursorPos(&p1);

            Sleep(2000);

            POINT p2; 
            GetCursorPos(&p2);
            if (GetAsyncKeyState(VK_LBUTTON) && p1.x == p2.x && p1.y == p2.y)
            {
                clicks[recordedClicks] = { p1.x, p1.y };
                
                cout << "Input entered.\nPlease enter in milliseconds how long the program should wait after this click at (" << p1.x << ", " << p1.y << ") (Enter -1 to cancel input) : ";
                
                int input;
                cin >> input;

                if (input >= 0)
                {
                    sleepTimes[recordedClicks] = input;
                    recordedClicks++;
                    cout << "Input sucessfully added.\n";
                }
                else
                {
                    cout << "Input canceled\n";
                }
            }
        }
        else if (GetAsyncKeyState('X') && GetAsyncKeyState(VK_LCONTROL))
        {
            break;
        }
    }

    cout << "Instructions: \n";
    for (int i = 0; i < recordedClicks; i++)
    {
        cout << "Click at (" << clicks[i].x << ", " << clicks[i].y << "), wait for " << sleepTimes[i] << "ms. \n";
    }
    cout << "\n[Hold Ctrl+Q to terminate the program]\n[Hold Ctrl+P to pause]\n";

    bool cont = true;
    while (cont)
    {

        for (int i = 0; i < recordedClicks; i++)
        {
            if (GetAsyncKeyState('Q') && GetAsyncKeyState(VK_LCONTROL))
            {
                cont = false;
                break;
            }
            else if (GetAsyncKeyState('P') && GetAsyncKeyState(VK_LCONTROL))
            {
                cout << "Program paused\n[Press Ctrl+R to resume]\n";
                while (!(GetAsyncKeyState('R') && GetAsyncKeyState(VK_LCONTROL)))
                    Sleep(1);
                cout << "Unpaused [Hold Ctrl+Q to terminate. Hold Ctrl+P to pause]\n";
            }

            moveMouseTo((double) clicks[i].x, (double) clicks[i].y);
            clickMouse();
            Sleep(sleepTimes[i]);
        }
    }
    cout << "Program terminated";

}
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
