#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <windows.h>
#include "resource.h"
#include <list>
#include <string>
#include <stdexcept>
#include "Win32FontManager.h"
#include <mmsystem.h>
#pragma comment (lib,"winmm.lib")
using namespace std;
HWND hWnd;
list<pair<HWND, Win32FontManager*>> Controls;
struct Time {
	int h, m, s;
} time{ 0,1,0 }, countup_time{ 0,0,0 }, countdown_time{ 0,0,0 };
enum TimerEnum { Countdown, Countup };
struct Timer {
	bool start;
	TimerEnum mode;
} timer{false, Countdown};
bool isSettingShow = false;
bool isPaused = false;
#define ID_TIMER 1145

#define TIME_LABEL 1001

#define HOURS_PLUS1 1002
#define HOURS_PLUS5 1003
#define HOURS_MINUS1 1004
#define HOURS_MINUS5 1005

#define MINUTE_PLUS1 1006
#define MINUTE_PLUS5 1007
#define MINUTE_MINUS1 1008
#define MINUTE_MINUS5 1009

#define SECOND_PLUS1 1010
#define SECOND_PLUS5 1011
#define SECOND_MINUS1 1012
#define SECOND_MINUS5 1013

#define SAVE 1014
#define START_CONUTDOWN 1015
#define START_CONUTUP 1016
#define PAUSE 1017
#define RESET 1018
#define STOP 1019

#define UPDATE_TIME SetWindowText(at(Controls, 0).first, ((to_wstring(time.h).length() == 1 ? L"0" : L"") + to_wstring(time.h) + L":" + (to_wstring(time.m).length() == 1 ? L"0" : L"") + to_wstring(time.m) + L":" + (to_wstring(time.s).length() == 1 ? L"0" : L"") + to_wstring(time.s)).c_str());
#define UPDATE_TIME_COUNTUP SetWindowText(at(Controls, 0).first, ((to_wstring(countup_time.h).length() == 1 ? L"0" : L"") + to_wstring(countup_time.h) + L":" + (to_wstring(countup_time.m).length() == 1 ? L"0" : L"") + to_wstring(countup_time.m) + L":" + (to_wstring(countup_time.s).length() == 1 ? L"0" : L"") + to_wstring(countup_time.s)).c_str());
#define UPDATE_TIME_COUNTDOWN SetWindowText(at(Controls, 0).first, ((to_wstring(countdown_time.h).length() == 1 ? L"0" : L"") + to_wstring(countdown_time.h) + L":" + (to_wstring(countdown_time.m).length() == 1 ? L"0" : L"") + to_wstring(countdown_time.m) + L":" + (to_wstring(countdown_time.s).length() == 1 ? L"0" : L"") + to_wstring(countdown_time.s)).c_str());

template<class T>
T at(const list<T>& l, int index) {
	if (index < 0 || index >= static_cast<int>(l.size())) {
		throw out_of_range("Index out of bounds");
	}

	auto it = l.begin();
	advance(it, index);
	return *it;
}

LRESULT CALLBACK WndProc(HWND hwNd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
	case WM_CREATE: {
		SetWindowPos(hwNd, HWND_TOPMOST, 0, 0, 100, 100, SWP_NOMOVE | SWP_NOSIZE);
		{
			HWND hwnd = CreateWindow(L"static", L"00:01:00", WS_CHILD | WS_VISIBLE | BS_CENTER | SS_CENTER, 0, 185, 640, 70, hwNd, (HMENU)TIME_LABEL, GetModuleHandle(0), NULL);
			Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 75)));
		}
		{
			// Сʱ
			{
				HWND hwnd = CreateWindow(L"button", L"Сʱ +1", WS_CHILD | BS_CENTER | SS_CENTER, 170, 150, 80, 35, hwNd, (HMENU)HOURS_PLUS1, GetModuleHandle(0), NULL);
				Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
			}
			{
				HWND hwnd = CreateWindow(L"button", L"Сʱ +5", WS_CHILD | BS_CENTER | SS_CENTER, 170, 110, 80, 35, hwNd, (HMENU)HOURS_PLUS5, GetModuleHandle(0), NULL);
				Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
			}
			{
				HWND hwnd = CreateWindow(L"button", L"Сʱ -1", WS_CHILD | BS_CENTER | SS_CENTER, 170, 260, 80, 35, hwNd, (HMENU)HOURS_MINUS1, GetModuleHandle(0), NULL);
				Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
			}
			{
				HWND hwnd = CreateWindow(L"button", L"Сʱ -5", WS_CHILD | BS_CENTER | SS_CENTER, 170, 300, 80, 35, hwNd, (HMENU)HOURS_MINUS5, GetModuleHandle(0), NULL);
				Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
			}
		}
		{
			// ����
			{
				HWND hwnd = CreateWindow(L"button", L"���� +1", WS_CHILD | BS_CENTER | SS_CENTER, 280, 150, 80, 35, hwNd, (HMENU)MINUTE_PLUS1, GetModuleHandle(0), NULL);
				Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
			}
			{
				HWND hwnd = CreateWindow(L"button", L"���� +5", WS_CHILD | BS_CENTER | SS_CENTER, 280, 110, 80, 35, hwNd, (HMENU)MINUTE_PLUS5, GetModuleHandle(0), NULL);
				Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
			}
			{
				HWND hwnd = CreateWindow(L"button", L"���� -1", WS_CHILD | BS_CENTER | SS_CENTER, 280, 260, 80, 35, hwNd, (HMENU)MINUTE_MINUS1, GetModuleHandle(0), NULL);
				Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
			}
			{
				HWND hwnd = CreateWindow(L"button", L"���� -5", WS_CHILD | BS_CENTER | SS_CENTER, 280, 300, 80, 35, hwNd, (HMENU)MINUTE_MINUS5, GetModuleHandle(0), NULL);
				Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
			}
		}
		{
			// ����
			{
				HWND hwnd = CreateWindow(L"button", L"���� +1", WS_CHILD | BS_CENTER | SS_CENTER, 390, 150, 80, 35, hwNd, (HMENU)SECOND_PLUS1, GetModuleHandle(0), NULL);
				Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
			}
			{
				HWND hwnd = CreateWindow(L"button", L"���� +5", WS_CHILD | BS_CENTER | SS_CENTER, 390, 110, 80, 35, hwNd, (HMENU)SECOND_PLUS5, GetModuleHandle(0), NULL);
				Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
			}
			{
				HWND hwnd = CreateWindow(L"button", L"���� -1", WS_CHILD | BS_CENTER | SS_CENTER, 390, 260, 80, 35, hwNd, (HMENU)SECOND_MINUS1, GetModuleHandle(0), NULL);
				Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
			}
			{
				HWND hwnd = CreateWindow(L"button", L"���� -5", WS_CHILD | BS_CENTER | SS_CENTER, 390, 300, 80, 35, hwNd, (HMENU)SECOND_MINUS5, GetModuleHandle(0), NULL);
				Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
			}
		}
		{
			HWND hwnd = CreateWindow(L"button", L"����", WS_CHILD | BS_CENTER | SS_CENTER, 280, 360, 80, 35, hwNd, (HMENU)SAVE, GetModuleHandle(0), NULL);
			Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
		}
		{
			HWND hwnd = CreateWindow(L"button", L"��ʼ (����ʱ)", WS_CHILD | WS_VISIBLE | BS_CENTER | SS_CENTER, 190, 360, 120, 35, hwNd, (HMENU)START_CONUTDOWN, GetModuleHandle(0), NULL);
			Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
		}
		{
			HWND hwnd = CreateWindow(L"button", L"��ʼ (����ʱ)", WS_CHILD | WS_VISIBLE | BS_CENTER | SS_CENTER, 330, 360, 120, 35, hwNd, (HMENU)START_CONUTUP, GetModuleHandle(0), NULL);
			Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
		}
		{
			HWND hwnd = CreateWindow(L"button", L"��ͣ", WS_CHILD | BS_CENTER | SS_CENTER, 170, 360, 80, 35, hwNd, (HMENU)PAUSE, GetModuleHandle(0), NULL);
			Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
		}
		{
			HWND hwnd = CreateWindow(L"button", L"����", WS_CHILD | BS_CENTER | SS_CENTER, 280, 360, 80, 35, hwNd, (HMENU)RESET, GetModuleHandle(0), NULL);
			Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
		}
		{
			HWND hwnd = CreateWindow(L"button", L"ֹͣ", WS_CHILD | BS_CENTER | SS_CENTER, 390, 360, 80, 35, hwNd, (HMENU)STOP, GetModuleHandle(0), NULL);
			Controls.push_back(make_pair(hwnd, new Win32FontManager(hwnd, 15)));
		}
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case TIME_LABEL:
		case SAVE: {
			if (!timer.start) {
				isSettingShow = !isSettingShow;
				for (int i = 0; i < 13; i++)
					ShowWindow(at(Controls, i + 1).first, isSettingShow ? SW_SHOW : SW_HIDE);
				for (int i = 13; i < 15; i++)
					ShowWindow(at(Controls, i + 1).first, !isSettingShow ? SW_SHOW : SW_HIDE);
				if (!(time.h || time.m || time.s)) time.s = 1;
			}
			break;
		}
		case HOURS_PLUS1:
			time.h++;
			break;
		case HOURS_PLUS5:
			time.h += 5;
			break;
		case HOURS_MINUS1:
			time.h--;
			break;
		case HOURS_MINUS5:
			time.m -= 5;
			break;
		case MINUTE_PLUS1:
			time.m++;
			break;
		case MINUTE_PLUS5:
			time.m += 5;
			break;
		case MINUTE_MINUS1:
			time.m--;
			break;
		case MINUTE_MINUS5:
			time.m -= 5;
			break;
		case SECOND_PLUS1:
			time.s++;
			break;
		case SECOND_PLUS5:
			time.s += 5;
			break;
		case SECOND_MINUS1:
			time.s--;
			break;
		case SECOND_MINUS5:
			time.s -= 5;
			break;
		case START_CONUTDOWN:
			if (!isSettingShow && !timer.start) {
				countdown_time = time;
				timer.start = true;
				isPaused = false;
				SetWindowText(at(Controls, 16).first, L"��ͣ");
				timer.mode = Countdown;
				SetTimer(hwNd, ID_TIMER, 1000, NULL);
				for (int i = 13; i < 15; i++)
					ShowWindow(at(Controls, i + 1).first, SW_HIDE);
				for (int i = 15; i < 18; i++)
					ShowWindow(at(Controls, i + 1).first, SW_SHOW);
			}
			break;
		case START_CONUTUP:
			if (!isSettingShow && !timer.start) {
				countup_time = { 0,0,0 };
				timer.start = true;
				isPaused = false;
				SetWindowText(at(Controls, 16).first, L"��ͣ");
				timer.mode = Countup;
				UPDATE_TIME_COUNTUP;
				SetTimer(hwNd, ID_TIMER, 1000, NULL);
				for (int i = 13; i < 15; i++)
					ShowWindow(at(Controls, i + 1).first, SW_HIDE);
				for (int i = 15; i < 18; i++)
					ShowWindow(at(Controls, i + 1).first, SW_SHOW);
			}
			break;
		case STOP:
			if (!isSettingShow && timer.start) {
				timer.start = false;
				isPaused = false;
				SetWindowText(at(Controls, 16).first, L"��ͣ");
				KillTimer(hwNd, ID_TIMER);
				for (int i = 13; i < 15; i++)
					ShowWindow(at(Controls, i + 1).first, SW_SHOW);
				for (int i = 15; i < 18; i++)
					ShowWindow(at(Controls, i + 1).first, SW_HIDE);
				UPDATE_TIME;
			}
			break;
	    case RESET:
			if (!isSettingShow && timer.start) {
				if (timer.mode == Countdown) { countdown_time = time; UPDATE_TIME_COUNTDOWN; }
				else { countup_time = { 0,0,0 }; UPDATE_TIME_COUNTUP; }
			}
			break;
	    case PAUSE: 
			isPaused = !isPaused;
			if (isPaused) SetWindowText(at(Controls, 16).first, L"����");
			else SetWindowText(at(Controls, 16).first, L"��ͣ");
			break;
		}

		if (time.h < 0) time.h = 100 + time.h % 100;
		if (time.h >= 100) time.h %= 100;
		if (time.m < 0) time.m = 60 + time.m % 60;
		if (time.m >= 60) time.m %= 60;
		if (time.s < 0) time.s = 60 + time.s % 60;
		if (time.s >= 60) time.s %= 60;

		if (!timer.start) { UPDATE_TIME; }
		break;
	}
	case WM_CTLCOLORSTATIC: {
		if (timer.start && timer.mode == Countdown && countdown_time.h == 0 && countdown_time.m == 0 && countdown_time.s <= 10) {
		    HDC hdc = (HDC)wParam;
			SetTextColor(hdc, RGB(255, 0, 0));
		}
		break;
	}
	case WM_TIMER: {
		if (LOWORD(wParam) == ID_TIMER && timer.start && !isPaused) {
			if(timer.mode == Countdown) {
				countdown_time.s--;
				if (countdown_time.s < 0) {
					countdown_time.m--;
					countdown_time.s += 60;
				}
				if (countdown_time.m < 0) {
					countdown_time.h--;
					countdown_time.m += 60;
				}
				
				if (countdown_time.h == 0 && countdown_time.m == 0 && countdown_time.s == 0) {
					// ʱ�䵽
					timer.start = false;
					KillTimer(hwNd, ID_TIMER);
					for (int i = 13; i < 15; i++)
						ShowWindow(at(Controls, i + 1).first, SW_SHOW);
					for (int i = 15; i < 18; i++)
						ShowWindow(at(Controls, i + 1).first, SW_HIDE);
					if (!PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC)) {
						// ����ʧ��, ֻ����MessageBox������, QAQ
						MessageBox(hwNd, L"ʱ�䵽��! ", L"��ʾ", MB_ICONINFORMATION);
					}
					UPDATE_TIME;
					break;
				}
				UPDATE_TIME_COUNTDOWN;
			}
			else if (timer.mode == Countup) {
				countup_time.s++;
				if (countup_time.s >= 60) {
					countup_time.m++;
					countup_time.s = 0;
				}
				if (countup_time.m >= 60) {
					countup_time.h++;
					countup_time.m = 0;
				}
				UPDATE_TIME_COUNTUP;
			}
		}
		break;
	}
	case WM_ERASEBKGND:{
		HDC hdc;
		RECT rect;
		HBRUSH hBrush;
		hdc = (HDC)wParam;
		GetClientRect(hwNd, &rect);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(hdc, &rect, hBrush);
		DeleteObject(hBrush);
		return 1;
	}
	case WM_CLOSE:
		if (timer.start) {
			if (MessageBox(hwNd, L"��ǰ���ڼ�ʱ, �˳����޷���ʱ, �Ƿ��˳�? ", L"��ʾ", MB_ICONQUESTION | MB_YESNO) == IDNO) return 0;
			else DestroyWindow(hwNd);
		} else DestroyWindow(hwNd);
		break;
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwNd, Message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	MSG Msg;

	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"InkeysApp";

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	int Width = 640, Height = 480, x, y;
	{
		int ScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN), ScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
		x = (ScreenWidth - Width) / 2;
		y = (ScreenHeight - Height) / 2;
	}
	hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, wc.lpszClassName, L"��ʱ�� - �ǻ��Inkeys", WS_VISIBLE | WS_CAPTION | WS_POPUPWINDOW, x, y, Width, Height, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) {
		MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}