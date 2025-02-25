// PomodoroApplication Source.cpp

# include <iostream>
# include <windows.h>
# include <stdlib.h>
# include <string.h>
# include <tchar.h>
# include <sstream>
# include "resource.h"

// Global variables
static TCHAR szWindowClass[] = _T("DesktopApp");  // Main Windows Class Name
static TCHAR szTitle[] = _T("My Pomodoro Application");  // String in title bar

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;
HWND hStartPauseButton;
HWND hResetButton;
HWND hwndChangeSettingsButton;
HWND hStaticText;

int targetWorkTime = 25;
int targetBreakTime = 5;
int minutesCompleted = 0;

// Function declarations
LRESULT CALLBACK WndProc(_In_ HWND   hWnd, _In_ UINT   message, _In_ WPARAM wParam, _In_ LPARAM lParam);

void AddButtons(HWND hWnd);
void AddUserInputBoxes(HWND hWnd);
void showPopup(HWND hWnd);
void ShowSettingsPopup(HWND hWnd);
void SaveSettings(HWND hSettingsWnd);
void UpdateDisplay(HWND hWnd);

// Windows Main Function 
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) 
{
	// Window class registration code
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	// CreateWindowEx with parameters explained:
	HWND hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,  // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
		szWindowClass,  // szWindowClass: the name of the application
		szTitle,  // szTitle: the text that appears in the title bar
		WS_OVERLAPPEDWINDOW,  // WS_OVERLAPPEDWINDOW: the type of window to create
		CW_USEDEFAULT, CW_USEDEFAULT,  // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
		500, 500,  // 500, 500: initial size (width, length)
		NULL,  // NULL: the parent of this window
		NULL,  // NULL: this application does not have a menu bar
		hInstance,  // hInstance: the first parameter from WinMain
		NULL  // NULL: not used in this application
	);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindowEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	// Store instance handle in our global variable
	hInst = hInstance;
	AddButtons(hWnd);
	AddUserInputBoxes(hWnd);
	UpdateDisplay(hWnd);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//  PURPOSE:  Processes messages for the main window.
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(
	HWND hWnd, 
	UINT message,
	WPARAM wParam, 
	LPARAM lParam
)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Welcome to my Pomodoro App.");

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
		EndPaint(hWnd, &ps);
		break;

	case WM_COMMAND: // This is the message that gets sent when a button is clicked
	{
		int wmId = LOWORD(wParam);  // ID of button which was clicked 
		int wmEvent = HIWORD(wParam);  // ID of button which was clicked 

		switch (wmId)
		{
		case 1: // ID start/pause button
			if (wmEvent == BN_CLICKED) {showPopup(hWnd);}
			break;
		case 2: // ID of reset button
			if (wmEvent == BN_CLICKED) {showPopup(hWnd);}
			break;		
		case 3: // ID of the settings button (defined in AddButtons)
			if (wmEvent == BN_CLICKED) {ShowSettingsPopup(hWnd);}
			break;		
		case ID_FILE_SETTINGS: // Settings menu item
			ShowSettingsPopup(hWnd);
			break;
		case ID_FILE_QUIT: // Exit menu item
			DestroyWindow(hWnd);
			break;
		case IDM_SAVE: // Save button in settings dialog
			if (wmEvent == BN_CLICKED) { SaveSettings(GetParent((HWND)lParam)); }
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}


void AddButtons(HWND hWnd)  // Single Function which holds all types of buttons ... I think 
{
	hStartPauseButton = CreateWindowEx(
		0L,  // Extended styles
		_T("BUTTON"),  // Predefined class; Unicode assumed 
		_T("Start/Pause"),      // Button text 
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // Styles 
		75,	// x position
		150,	// x position
		100,	// Button width
		50,	// Button width
		hWnd,	// Parent window
		(HMENU)1,	// Button ID (used in WM_COMMAND) - VERY IMPORTANT!
		hInst,
		NULL);      // Pointer not needed.
	if (!hStartPauseButton) {
		MessageBox(hWnd, _T("Could not create start/pause button."), _T("Error"), MB_OK);
	}

	hResetButton = CreateWindowEx(
		0L,  // Extended styles
		_T("BUTTON"),  // Predefined class; Unicode assumed 
		_T("Reset Timer"),      // Button text 
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // Styles 
		185,	// x position
		150,	// x position
		100,	// Button width
		50,	// Button width
		hWnd,	// Parent window
		(HMENU)2,	// Button ID (used in WM_COMMAND) - VERY IMPORTANT!
		hInst,
		NULL);      // Pointer not needed.
	if (!hResetButton) {
		MessageBox(hWnd, _T("Could not create reset button."), _T("Error"), MB_OK);
	}
}

void AddUserInputBoxes(HWND hWnd)
{
	hStaticText = CreateWindowEx(
		0, _T("STATIC"), _T("Work Time: %d min, Break Time: %d min", targetWorkTime, targetBreakTime),
		WS_CHILD | WS_VISIBLE | SS_CENTER, 50, 50, 400, 30, hWnd, NULL, hInst, NULL);
}

void showPopup(HWND hWnd) {
	MessageBox(hWnd, _T("You clicked the button mate!"), _T("Popup"), MB_OK);
}

void ShowSettingsPopup(HWND hWnd) {

	HWND hSettingsWnd = CreateWindowEx(
		WS_EX_DLGMODALFRAME, _T("#32770"), _T("Settings"),  // "#32770" is the standard class name for dialog boxes 
		WS_POPUP | WS_CAPTION | WS_SYSMENU, 100, 100, 300, 200, hWnd, NULL, hInst, NULL);

	if (!hSettingsWnd) {
		MessageBox(hWnd, _T("Failed to create settings window."), _T("Error"), MB_OK);
		return;
	}

	CreateWindowEx(0, _T("STATIC"), _T("Work Time (min):"), WS_CHILD | WS_VISIBLE, 10, 10, 120, 20, hSettingsWnd, NULL, hInst, NULL);

	HWND hWorkTimeInput = CreateWindowEx(
		WS_EX_CLIENTEDGE, _T("EDIT"), std::to_wstring(targetWorkTime).c_str(),
		WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER, 140, 10, 50, 25,
		hSettingsWnd, (HMENU)ID_WORK_TIME_EDIT, hInst, NULL);

	CreateWindowEx(0, _T("STATIC"), _T("Break Time (min):"), WS_CHILD | WS_VISIBLE, 10, 40, 120, 20, hSettingsWnd, NULL, hInst, NULL);

	HWND hBreakTimeInput = CreateWindowEx(
		WS_EX_CLIENTEDGE, _T("EDIT"), std::to_wstring(targetBreakTime).c_str(),
		WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER,
		140, 40, 50, 25, hSettingsWnd, (HMENU)ID_BREAK_TIME_EDIT, hInst, NULL);

	HWND hSaveButton = CreateWindowEx(0, _T("BUTTON"), _T("Save"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 100, 80, 30, hSettingsWnd, (HMENU)IDM_SAVE, hInst, NULL);

	ShowWindow(hSettingsWnd, SW_SHOW);
	UpdateWindow(hSettingsWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (IsDialogMessage(hSettingsWnd, &msg)) {
			continue; // Handle dialog messages
		}
		if (msg.message == WM_CLOSE && msg.hwnd == hSettingsWnd) {
			DestroyWindow(hSettingsWnd);
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void SaveSettings(HWND hSettingsWnd)
{
	HWND hWorkTimeInput = GetDlgItem(hSettingsWnd, ID_WORK_TIME_EDIT);
	HWND hBreakTimeInput = GetDlgItem(hSettingsWnd, ID_WORK_TIME_EDIT);


	TCHAR workTimeBuffer[16], breakTimeBuffer[16]; // Buffer for text retrieval
	GetWindowText(hWorkTimeInput, workTimeBuffer, 16);
	GetWindowText(hBreakTimeInput, breakTimeBuffer, 16);

	int newWorkTime = _ttoi(workTimeBuffer);
	int newBreakTime = _ttoi(breakTimeBuffer);

	if (newWorkTime > 0 && newBreakTime > 0)
	{
		targetWorkTime = newWorkTime;
		targetBreakTime = newBreakTime;
		UpdateDisplay(GetParent(hSettingsWnd)); // Update main window display
		DestroyWindow(hSettingsWnd);
	}
	else
	{
		MessageBox(hSettingsWnd, _T("Invalid input. Please enter positive numbers."), _T("Error"), MB_OK);
	}
}

void UpdateDisplay(HWND hWnd) {
	TCHAR buffer[256];
	_stprintf_s(buffer, _T("Work Time: %d min, Break Time: %d min"), targetWorkTime, targetBreakTime);
	SetWindowText(hStaticText, buffer);
}

// PomodoroApplication Source.cpp