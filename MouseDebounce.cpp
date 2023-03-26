#include <iostream>
#include <windows.h>
#include <chrono>

std::chrono::steady_clock::time_point lastdown;

HHOOK mouseLL;
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        switch (wParam) {
            case WM_LBUTTONDOWN:
                auto currentdown = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentdown - lastdown);
                if (duration.count() <= 120000) return -1;
                lastdown = currentdown;
                break;
        }
    }

    return CallNextHookEx(mouseLL, nCode, wParam, lParam);
}

int main()
{
    std::cout << "Mouse Debounce v1.0\nNow preventing double mouse left clicks\nMinimize this window." << std::endl;
    lastdown = std::chrono::high_resolution_clock::now();

    mouseLL = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, 0, 0);

    MSG message;
    while (GetMessage(&message, NULL, NULL, NULL) > 0) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    UnhookWindowsHookEx(mouseLL);

    return 0;
}