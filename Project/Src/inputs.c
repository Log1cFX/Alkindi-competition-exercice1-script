#include <windows.h>
#include <stdio.h>

#include "inputs.h"
#include "cursor.h"

static HInputs *hInputsList[5] = {0};

// Keyboard hook callback function
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && wParam == WM_KEYDOWN)
    {
        KBDLLHOOKSTRUCT *kbStruct = (KBDLLHOOKSTRUCT *)lParam;
        switch (kbStruct->vkCode)
        {
        case VK_LEFT:
            // printf("Left Arrow Pressed\n");
            cursor_moveLeft();
            break;
        case VK_RIGHT:
            // printf("Right Arrow Pressed\n");
            cursor_moveRight();
            break;
        case VK_UP:
            // printf("Up Arrow Pressed\n");
            cursor_moveUp();
            break;
        case VK_DOWN:
            // printf("Down Arrow Pressed\n");
            cursor_moveDown();
            break;
        case 'N' :
            // cursor_leftDown();
            break;
        case 'F':
            for (int i = 0; i < 5; i++)
            {
                HInputs *handle = hInputsList[i];
                if (handle != NULL)
                {
                    if (handle->stopFunction(handle, handle->customParamFunction()))
                    {
                        inputs_stopListening(handle);
                    }
                }
            }
            break;
        case VK_ESCAPE: // Exit program when ESC is pressed
            printf("Exiting the program\n");
            exit(0);
            break;
        }
    }
    // return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int inputs_INIT(HInputs *hInputs, int (*stopFunction)(HInputs*, POINT*), POINT *(*customParamFunction)(void))
{
    // Set the functions that will be executed with a keyPress, if stopFunction returns 1 the thread will stop listening
    hInputs->stopFunction = stopFunction;
    hInputs->customParamFunction = customParamFunction;
    // Put it in a list a static list
    for (int i = 0; i < 5; i++)
    {
        if (hInputsList[i] == NULL)
        {
            hInputsList[i] = hInputs;
            break;
        }
    }
    // Set the keyboard hook
    hInputs->hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
    if (!hInputs->hHook)
    {
        printf("Failed to install hook!\n");
        return 1;
    }

    printf("Keyboard hook installed.\n");
    return 0;
}

int inputs_startListening(HInputs *hInputs)
{
    hInputs->isListening = true;
    while (GetMessage(&hInputs->msg, NULL, 0, 0))
    {
        TranslateMessage(&hInputs->msg);
        DispatchMessage(&hInputs->msg);

        if (!hInputs->isListening)
        {
            return 0;
        }
        if (hInputs->error)
        {
            printf("An error occured inside the inputs logic.\n");
            inputs_stopListening(hInputs);
            return 1;
        }
    }
    return 0;
}

void inputs_stopListening(HInputs *hInputs)
{
    hInputs->isListening = false;
    PostQuitMessage(0);
    printf("stoped Listening\n");
}

void inputs_poll(HInputs *hInputs)
{
    while (PeekMessage(&hInputs->msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&hInputs->msg);
        DispatchMessage(&hInputs->msg);
    }
}
