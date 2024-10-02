// Main.cpp : Main entrace of the application

#include "framework.h"
#include "Main.h"
#include "TGame.h"

#define MAX_LOADSTRING 100

// Global variables
HINSTANCE hInst;                                // Current instance of app
WCHAR szTitle[MAX_LOADSTRING];                  // Text of title string
WCHAR szWindowClass[MAX_LOADSTRING];            // Class name of main window
HWND hWnd;
HDC fDC, tempDC;
HBITMAP oldBmp, tempBmp;
TGame *Game;

const int WndX = 600, WndY = 525;

// Submit a declaration of functions included in this module's code:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    HandlerInfoWindow(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialization of global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TETRIS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Application initialization
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRIS));

    MSG msg;

    // Cycle for messages listening
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Window class registration
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRIS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(BACKGROUND_COLOR);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TETRIS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Instance marker saving and main window creating
//
//   COMMENTARY:
//
//        With that function instance mark saves in global variable,
//        as well as application main window creates and outputs  
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
      10, 10, WndX, WndY, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processing of messages in main window
//
//  WM_COMMAND  - process the menu of application
//  WM_PAINT    - draw the main window
//  WM_DESTROY  - send the message for exit and then return
//  WM_KEYDOWN  - process the key pressing from keyboard
//  WM_TIMER    - timer message

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		fDC = GetDC(hWnd); //Display context
		tempDC = CreateCompatibleDC(fDC); //Additional context for double buferization
		tempBmp = CreateCompatibleBitmap(fDC, WndX, WndY); //Bitmaps
		SelectObject(tempDC, tempBmp);
		Game = new TGame(hInst, hWnd, tempDC);
		//Creating additional keys
		CreateWindowEx(NULL,
			L"button", L"Пауза",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			375, 400, 150, 25,
			hWnd,
			(HMENU)IDM_PAUSE_BUTTON,
			hInst,
			NULL);
		CreateWindowEx(NULL,
			L"button", L"Новая игра",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			375, 350, 150, 25,
			hWnd,
			(HMENU)IDM_NEWGAME_BUTTON,
			hInst,
			NULL);
		break;
    case WM_COMMAND:
        {			
            int wmId = LOWORD(wParam), command = HIWORD(wParam);
            //Process the menu selection
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, HandlerInfoWindow);
                break;
			case IDM_CONTROL:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_CONTROLBOX), hWnd, HandlerInfoWindow);
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDM_NEWGAME_BUTTON:
				if (command == BN_CLICKED)
				{
					Game->NewGame();
					SetFocus(hWnd);
				}
					break;
			case IDM_PAUSE_BUTTON:
				if (command == BN_CLICKED)
				{
					Game->Pause();
					SetFocus(hWnd);
				}
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_KEYDOWN:
		Game->OnKeyPress(wParam);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);			
			BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, tempDC, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY); //Обновление изображения экрана
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_ERASEBKGND:
		break;
	case WM_TIMER:
		Game->OnTimer();
		break;
    case WM_DESTROY:
		delete Game;
		DeleteObject(tempBmp);
		DeleteDC(tempDC);
		ReleaseDC(hWnd, fDC);		
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Messages processing for informational windows with static text only
INT_PTR CALLBACK HandlerInfoWindow(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
