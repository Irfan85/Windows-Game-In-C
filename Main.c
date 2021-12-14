#include <stdio.h>

// Lowering down wrning level toh 3 for warnings related to this particular header fine only
#pragma warning(push, 3)
#include <windows.h>
#pragma warning(pop)
// Pop the 0 from the stack to get warning level back to default

#include "Main.h"

// The 'WinMain' function is the main function of Windows applications
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    // By using the macro"UNREFERENCED_PARAMETER", we're letting the compiler know that we won't be using these paremeters so don't give us warnings
    UNREFERENCED_PARAMETER(hInstPrev);
    UNREFERENCED_PARAMETER(cmdline);
    UNREFERENCED_PARAMETER(cmdshow);

    if (gameIsAlreadyRunning())
    {
        return MessageBox(NULL, "Another instance of the game is already running", "Error", MB_ICONEXCLAMATION | MB_OK);
    }

    if (createMainGameWindow(hInst) != ERROR_SUCCESS)
    {
        return MessageBox(NULL, "And error occurred while creating a window", "Error", MB_ICONEXCLAMATION | MB_OK);
    }

    // Since, the window has been created, show the window
    // We alredy styled the window as visible before so we don't need this
    //ShowWindow(windowHandle, TRUE);

    // Whatever variables are inside the "MSG" structure, initialize them with '0'
    MSG message = { 0 };

    // This is the message loop that will fetch the messages from the OS and send them to the
    // window procedure function "MainWndProc"
    while (GetMessage(&message, NULL, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return 0;
}

/*
    This function is the window procedure function that will revieve all the
    messages sent by the operating sysetem and handle them accordingly
*/
LRESULT CALLBACK windowProc(
    HWND hwnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{

    char messageBuffer[12] = { 0 };
    _itoa_s(uMsg, messageBuffer, _countof(messageBuffer), 10);

    OutputDebugString(messageBuffer);
    OutputDebugString("\n"); // We can't put escape characters or format specifiers in the "OutputDebugString" function with a string. SO we have to put them seperately

    switch (uMsg)
    {
        case WM_CREATE:
            // Initialize the window. 
            return 0;

        case WM_PAINT:
            // Paint the window's client area. 
            return 0;

        case WM_SIZE:
            // Set the size and position of the window. 
            return 0;

        case WM_DESTROY:
            // Clean up window-specific data objects. 
            return 0;

        case WM_CLOSE:
            // We want to quit the program when the user will press 'x' window button
            PostQuitMessage(0); // This will send a quit message to the window. We pass '0' because when "GetMessage" will return 0, the window will close
            break;

            // 
            // Process other messages. 
            // 

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// We're using SAL(Source Annotation Language) here. Although they're optional, they help the compiler to generate more useful warnings
// "_In_" tells the compiler that the following argument is an input argument and will work as an input the the 
// function
DWORD createMainGameWindow(_In_ HANDLE instance)
{
    /*
        In Win32 API, everyting is a window. A Window, button, progressbar and all other widgets are
        considered as windows. In order to create a window, we need a window class
    */
    WNDCLASSEX windowClass = { 0 }; // Initialize every variable inside the structure with '0'

    windowClass.cbSize = sizeof(WNDCLASSEX); // The size, in bytes, of this structure 
    windowClass.style = 0;
    windowClass.lpfnWndProc = windowProc; // Long pointer to a window procedure functoin
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = instance; // A handle to the applicatoin isntance. "GetModuleHandle(NULL)" will also work here that returns the handle(reference) to the current module or applicaiton
    windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = "MAIN_WINDOW_CLASS";
    windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // Now we will register our window class
	if (!RegisterClassEx(&windowClass))
    {
        // If window registration is failed, show an error message box
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return GetLastError();
    }

    // Now that our window is registeres we can create our window. The instance of the window
    // will be stored in a window handle
    HWND windowHandle = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        windowClass.lpszClassName,
        "The title of my window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // The styles of the window
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, instance, NULL);

    // If the window creation is failed, display an error message box
    if (windowHandle == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return GetLastError();
    }

    return ERROR_SUCCESS; // This will just return '0'
}

// We want to make sure that only one instance of the game runs at a time in the computer
BOOL gameIsAlreadyRunning()
{
    // We will create a mutex for our game to make that happen
    HANDLE mutex = CreateMutex(NULL, FALSE, GAME_NAME, "_GameMutex");

    // If the mutex already exists, it means the user has a previous instance of the game running
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}