#ifndef __CORE_H__
#define __CORE_H__


//pre-include definitions
#ifndef UNICODE
#define UNICODE
#endif


//libraries
#pragma comment (lib, "Opengl32.lib")


//includes
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <gl/GL.h>
#include "core.h"


//definitions
#define WINDOW_CLASS_NAME	(L"gl_test class")
#define HEIGHT				(500)
#define WIDTH				(500)
#define TIMER_60HZ			(1)

//global variables
HINSTANCE hInst;
HDC ghDC;
HGLRC ghRC;


//function declarations
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI update_call_thread(LPVOID lpParameter);
BOOL bSetupPixelFormat(HDC hdc);
void print_error(char *state);
BOOL swap_buffers();
extern void cleanup();
extern void update();
extern void init();
#endif
