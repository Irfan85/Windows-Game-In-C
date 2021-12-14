#pragma once

#define GAME_NAME "My Game"

// Function prototypes
LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

DWORD createMainGameWindow(_In_ HANDLE instance);

BOOL gameIsAlreadyRunning();