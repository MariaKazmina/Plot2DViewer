#include <windows.h>
#include <windowsx.h>
#include "Data.h"
#include "Scene2D.h"

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// прототип оконной процедуры
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// основная процедура
{
	// Первая составляющая часть основной процедуры - создание окна: сначала описывается оконный класс wc, затем создаётся окно hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// имя оконной процедуры, закрепленной за данным классом
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// идентификатор приложения, содержащий адрес начала выделенной ему области памяти
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// меню в оконном классе отсутствует
	wc.lpszClassName = (LPCSTR)"MainWindowClass";	// имя оконного класса, используемое при создании экземпляров окна
	RegisterClass(&wc);								// регистрация класса wc

	HWND hWnd = CreateWindow(						// hWnd - дескриптор, идентифицирующий окно; функция создания окна заполняет дескриптор hWnd ненулевым значением
		(LPCSTR)"MainWindowClass",					// имя оконного класса
		(LPCSTR)"Plot2D Viewer",					// заголовок окна
		WS_OVERLAPPEDWINDOW,						// стиль окна
		200, 200, 400, 400,							// координаты на экране левого верхнего угла окна, его ширина и высота
		nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Вторая составляющая часть основной процедуры - основной цикл обработки системных сообщений, который ожидает сообщения и рассылает их соответствующим окнам
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))				// функция GetMessage выбирает из очереди сообщение и заносит его в структуру msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// функция DispatchMessage оповещает систему о необходимости вызова оконной процедуры
	}

	return 0;
}

// В основном модуле объявляется только одна глобальная переменная - создаётся объект класса Scene2D
// Все дальнейшие действия осуществляются посредством обращения к методам, реализованным в этом классе
Scene2D scene(L, R, B, T);
typedef double(*Func)(double);
Func R1 = ER1;
Func R2 = ER2;
LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// оконная процедура принимает и обрабатывает все сообщения, отправленные окну
{
	switch (msg)
	{
	case WM_PAINT:
	{
		HDC dc = GetDC(hWnd);
		scene.Clear(dc);				// Вызов реализованного в классе Camera2D метода, отвечающего за очистку рабочей области окна hWnd
		scene.BiPlot(dc, R1, R2);		// Вызов реализованного в классе Scene2D метода, отвечающего за отрисовку графика синусоиды
		ReleaseDC(hWnd, dc);
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	case WM_SIZE:
	{
		HDC dc = GetDC(hWnd);
		scene.SetResolution(dc);
		ReleaseDC(hWnd, dc);
		InvalidateRect(hWnd, nullptr, false);
		return 0;
	}
	case WM_CHAR:
	{
		switch (wParam)
		{
		case '1':  break;
		case '2':  break;
		case '3':  break;
		case 'r': scene.Reset(); break;
		default: return 0;
		}
		InvalidateRect(hWnd, nullptr, false);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		POINT point;
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);
		ScreenToClient(hWnd, &point);
		if (scene.Scale(GET_WHEEL_DELTA_WPARAM(wParam), point.x, point.y))
			InvalidateRect(hWnd, nullptr, false);
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		scene.StartDragging(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		if (scene.IsDragging())
		{
			scene.Drag(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			InvalidateRect(hWnd, nullptr, false);
		}
		return 0;
	}
	case WM_LBUTTONUP:
	{
		scene.StopDragging();
		return 0;
	}
	default:
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	}
	return 0;
}