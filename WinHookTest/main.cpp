#define _WIN32_WINNT 0x0500
#include <windows.h>

HHOOK MouseHook;

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam){

        PKBDLLHOOKSTRUCT k = (PKBDLLHOOKSTRUCT)(lParam);
        POINT p;


        if(wParam == WM_RBUTTONDOWN)
        { 
          // right button clicked 
          GetCursorPos(&p);
        }

		return CallNextHookEx(MouseHook, nCode, wParam, lParam);
}

void StayAlive(){
        while(1) { }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nShowCmd){
        FreeConsole();

        MouseHook = SetWindowsHookEx(WH_MOUSE_LL,MouseHookProc,hInstance,0);
		if(!MouseHook)
			exit(0);

        StayAlive();

        UnhookWindowsHookEx(MouseHook);
        return 0;
}