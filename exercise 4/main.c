#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/**
 * Author: Marek Èupr
 * Description: Simple WinAPI application to demonstrate an interaction.
 * Date: 23.10.2024
*/

// Global variable
HINSTANCE hInst;
UINT  MessageCount = 0;
UINT  Count = 0;
int posX = 0;
int posY = 0;

// Function prototypes.
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition);
void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps);

COLORREF colors[] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 255, 0), RGB(0, 255, 255), RGB(255, 0, 255) };
int currentColorIndex = 0;
const int COLORS_LEN = sizeof(colors) / sizeof(colors[0]);

// Application entry point. This is the same as main() in standart C.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;
	WNDCLASS wcx;          // register class
	HWND hWnd;

	hInst = hInstance;     // Save the application-instance handle.
		// Fill in the window class structure with parameters that describe the main window.

	wcx.style = CS_HREDRAW | CS_VREDRAW;              // redraw if size changes
	wcx.lpfnWndProc = (WNDPROC)MainWndProc;          // points to window procedure
	wcx.cbClsExtra = 0;                               // no extra class memory
	wcx.cbWndExtra = 0;                               // no extra window memory
	wcx.hInstance = hInstance;                        // handle to instance
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);      // predefined app. icon
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);        // predefined arrow
	wcx.hbrBackground = GetStockObject(WHITE_BRUSH);  // white background brush
	wcx.lpszMenuName = (LPCSTR)"MainMenu";          // name of menu resource
	wcx.lpszClassName = (LPCSTR)"MainWClass";        // name of window class

	// Register the window class.

	if (!RegisterClass(&wcx)) return FALSE;

	// create window of registered class

	hWnd = CreateWindow(
		"MainWClass",        // name of window class
		"ITU",               // title-bar string
		WS_OVERLAPPEDWINDOW, // top-level window
		200,                  // default horizontal position
		25,                 // default vertical position
		1000,                // default width
		700,                 // default height
		(HWND)NULL,         // no owner window
		(HMENU)NULL,        // use class menu
		hInstance,           // handle to application instance
		(LPVOID)NULL);      // no window-creation data
	if (!hWnd) return FALSE;

	// Show the window and send a WM_PAINT message to the window procedure.
	// Record the current cursor position.

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// loop of message processing
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}


LRESULT CALLBACK MainWndProc(
	HWND hWnd,        // handle to window
	UINT uMsg,        // message identifier
	WPARAM wParam,    // first message parameter
	LPARAM lParam)    // second message parameter
{
	HDC         hDC;
	PAINTSTRUCT ps;
	POINT cursorPosition;

	// init cursor position 
	GetCursorPos(&cursorPosition);
	ScreenToClient(hWnd, &cursorPosition);

	switch (uMsg)
	{
	case WM_CREATE:
		break;

	// character input 
	case WM_CHAR:
		switch (wParam) {
		case 0x08:  // backspace
		case 0x0A:  // linefeed
		case 0x1B:  // escape
			break;

		case 0x09:  // tab
			break;

		default:
			break;
		}
		break;

	// key input
	case WM_KEYDOWN:
		switch (wParam) {
	        // update posX and posY in order to move object
		case VK_LEFT: // left arrow
			posX -= 5;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_RIGHT: // right arrow
			posX += 5;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_UP: // up arrow
			posY += 5;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_DOWN: // down arrow
			posY -= 5;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		// react on the other pressed keys 
		case VK_SPACE: // space
			break;
		case VK_BACK: // backspace
			break;
		case VK_TAB: // tab
			currentColorIndex = (currentColorIndex >= COLORS_LEN) ? 0 : currentColorIndex + 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
	        // more virtual codes can be found here: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		}
		break;

	// get cursor position 
	case WM_MOUSEMOVE:
		GetCursorPos(&cursorPosition);
		ScreenToClient(hWnd, &cursorPosition);
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	// react on mouse clicks
	case WM_LBUTTONDOWN:
		currentColorIndex = (currentColorIndex >= COLORS_LEN) ? 0 : currentColorIndex + 1;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		break;

	case WM_RBUTTONDOWN:
		currentColorIndex = (currentColorIndex < 0) ? COLORS_LEN - 1: currentColorIndex - 1;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_RBUTTONUP:
		break;

	// paint objects
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		paintObject(hWnd, hDC, ps, posX, posY, cursorPosition);
		paintPosition(hWnd, hDC, ps);
		// paint other objects
		// paintObject2(hWnd, hDC, ps, posX, posY, cursorPosition);
		// paintObject3(hWnd, hDC, ps, posX, posY, cursorPosition);
		EndPaint(hWnd, &ps);
		DeleteDC(hDC);
		break;

		//
		// Process other messages.
		//
	
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition)
{
	// Paint rectangles, ellipses, polygons, lines etc.
	HPEN pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	SelectObject(hDC, pen);

	// Left leg
	MoveToEx(hDC, 470 + posX, 600 - posY, NULL);
	LineTo(hDC, 500 + posX, 550 - posY);

	// Right leg
	MoveToEx(hDC, 530 + posX, 600 - posY, NULL);
	LineTo(hDC, 500 + posX, 550 - posY);

	// Left hand
	MoveToEx(hDC, 500 + posX, 460 - posY, NULL);
	LineTo(hDC, 470 + posX, 500 - posY);

	// Right hand
	MoveToEx(hDC, 500 + posX, 460 - posY, NULL);
	LineTo(hDC, 530 + posX, 500 - posY);

	// Body
	MoveToEx(hDC, 500 + posX, 550 - posY, NULL);
	LineTo(hDC, 500 + posX, 450 - posY);

	// Head
	SelectObject(hDC, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hDC, colors[currentColorIndex]);
	Ellipse(hDC, 475 + posX, 400 - posY, 525 + posX, 450 - posY);

	// Eyes
	SelectObject(hDC, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hDC, RGB(255, 255, 255));
	Ellipse(hDC, 483 + posX, 410 - posY, 498 + posX, 420 - posY);
	Ellipse(hDC, 503 + posX, 410 - posY, 517 + posX, 420 - posY);

	// Reset pen
	DeleteObject(pen);
	return;
}

void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps)
{
	char        text[40];          // buffer to store an output text
	HFONT       font;              // new large font
	HFONT       oldFont;           // saves the previous font

	font = CreateFont(25, 0, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 0);
	oldFont = (HFONT)SelectObject(hDC, font);
	sprintf(text, "Position -- x:%d, y:%d", posX, posY);
	TextOut(hDC, 50, 600, text, (int)strlen(text));
	SelectObject(hDC, oldFont);
	DeleteObject(font);
}
