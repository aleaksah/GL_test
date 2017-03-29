#include "core.h"

int main(int argc, char **argv)
{
	WNDCLASSEX wClass;
	MSG msg;
	HWND hWnd;

	srand((unsigned int)time(NULL));

	ZeroMemory(&wClass, sizeof(WNDCLASSEX));
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.style = CS_HREDRAW | CS_VREDRAW;
	wClass.lpfnWndProc = (WNDPROC)WindowProc;
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;
	wClass.hInstance = hInst;
	wClass.hIcon = NULL;
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = WINDOW_CLASS_NAME;
	wClass.hIconSm = NULL;

	if(!RegisterClassEx(&wClass))
	{
		int nResult=GetLastError();
		print_error("Window class creation failed");
	}

	if(!(hWnd = CreateWindowEx(0, WINDOW_CLASS_NAME, L"GL_test", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 200, 200, 360, 290, NULL, NULL, hInst, NULL)))
	{
		print_error("Window creation failed");
		return -1;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	ZeroMemory(&msg, sizeof(MSG));

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		update();
	}

	if(!UnregisterClass(WINDOW_CLASS_NAME, NULL))
	{
		print_error("error unregister class");
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	LONG result = 1;

	switch(uMsg)
	{
	case WM_PAINT: 
		{
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break; 

	case WM_CREATE: 
		{
			puts("Window created");
			if (!SetWindowPos(hWnd, HWND_TOP, 0, 0, WIDTH, HEIGHT, SWP_NOMOVE))
			{
				print_error("error SetWindowPos");
			}

			if (!(ghDC = GetDC(hWnd)))
			{
				print_error("error GetDC");
			}

			if (!bSetupPixelFormat(ghDC))
			{
				print_error("error bSetupPixelFormat");
			}

			if (!(ghRC = wglCreateContext(ghDC)))
			{
				print_error("error wglCreateContext");
			}

			if (!wglMakeCurrent(ghDC, ghRC))
			{
				print_error("error wglMakeCurrent");
			}

			init();
		}
		break;

	case WM_DESTROY:
		{
			puts("Window destroed");

			if (ghRC)
			{
				if (!wglDeleteContext(ghRC))
				{
					print_error("error wglDeleteContext");
				}
			}

			if (ghDC)
			{
				if (!ReleaseDC(hWnd, ghDC))
				{
					print_error("error ReleaseDC");
				}
			}

			cleanup(hWnd);

			PostQuitMessage(0);
			return 0;
		}
		break;

	default: 
		{
			result = DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
        break; 
	}

	return result;
}

BOOL bSetupPixelFormat(HDC hdc) 
{ 
	PIXELFORMATDESCRIPTOR pfd; 
	int pixelformat; 

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); 
	pfd.nVersion = 1; 
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
	pfd.dwLayerMask = PFD_MAIN_PLANE; 
	pfd.iPixelType = PFD_TYPE_COLORINDEX; 
	pfd.cColorBits = 8; 
	pfd.cDepthBits = 16; 
	pfd.cAccumBits = 0; 
	pfd.cStencilBits = 0; 

	if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == 0) 
	{ 
		print_error("ChoosePixelFormat failed");
		return FALSE; 
	} 

	if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) 
	{ 
		print_error("SetPixelFormat failed");
		return FALSE; 
	} 

	return TRUE; 
}

void print_error(char *state)
{
	DWORD error = GetLastError();
	char *error_description = NULL, *error_description_oem = NULL;
	if (FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&error_description, 0, 0) != 0)
	{
		error_description_oem = (char*)malloc(strlen(error_description) + 1);
		CharToOemA(error_description, error_description_oem);
		HeapFree(GetProcessHeap(), 0, error_description);
		fprintf(stderr, "%s, winerr %d: %s", state, error, error_description_oem);
		free(error_description_oem);
	}
	else
	{
		fprintf(stderr, "%s, winerr %d (FormatMessageA fail with winerr %d)", state, error, GetLastError());
	}
}

BOOL swap_buffers()
{
	return SwapBuffers(ghDC);
}