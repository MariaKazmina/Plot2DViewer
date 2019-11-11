#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>

class Camera2D
{
protected:
	double L, R, B, T;					// Мировые координаты границ рабочей области окна
	int W, H;							// Разрешение рабочей области окна
	int WorldToScreenX(double X)		// Переход от мировых координат к экранным (для абсциссы)
	{
		return (X - L) * 1. / (R - L) * W;
	}
	int WorldToScreenY(double Y)		// Переход от мировых координат к экранным (для ординаты)
	{
		return (T - Y) * 1. / (T - B) * H;
	}
	double ScreenToWorldX(int X)		// Переход от экранных координат к мировым (для абсциссы)
	{
		return (double)L + (R - L) * (X + 0.5) / W;
	}
	double ScreenToWorldY(int Y)		// Переход от экранных координат к мировым (для ординаты)
	{
		return (double)T - (T - B) * (Y + 0.5) / H;
	}
private:
	double k = 1;
	bool isDragging;
	double prvX, prvY;
	double posX, posY;					// Позиция графического курсора в мировых координатах (для функций MoveTo и LineTo)
public:
	Camera2D(double L, double R, double B, double T) : L(L), R(R), B(B), T(T)
	{
	}
	void Clear(HDC dc)
	{
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		Rectangle(dc, 0, 0, r.right, r.bottom);
		// Очистка рабочей области окна
	}
	void SetResolution(HDC dc)
	{
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		H = r.bottom + 1;
		W = r.right + 1;
		double BpT = (B + T) / 2;
		double RmL = (R - L) / 2 * H / W;
		T = BpT + RmL;
		B = BpT - RmL;
		// Данная процедура вызывается при изменении размеров окна
		// В ней задаются значения величин W, H, а также настраиваются значения параметров L, R, B, T таким образом, чтобы обеспечить одинаковые масштабы по координатным осям
	}
	void MoveTo(double X, double Y)
	{
		posX = X;
		posY = Y;
		// Перемещение графического курсора (posX, posY)
		// Обратите внимание, что мы действуем в мировых координатах
	}
	void LineTo(HDC dc, double X, double Y)
	{
		::MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), nullptr);
		::LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));
		MoveTo(X, Y);
		// Отрисовка линии из текущей позиции графического курсора в точку (X, Y) и его перемещение в эту точку
		// Обратите внимание, что мы действуем в мировых координатах
		// При отрисовке линии могут быть использованы WinApi функции
		// ::MoveToEx(dc, Xs, Ys, nullptr) и ::LineTo(dc, Xs, Ys)
	}
	void Axes(HDC dc)
	{
		MoveTo(L, 0);
		LineTo(dc, R, 0);
		MoveTo(0, B);
		LineTo(dc, 0, T);
		// Отрисовка координатных осей
	}
	void StartDragging(int X, int Y)
	{
		isDragging = true;
		prvX = X;
		prvY = Y;
	}
	void StopDragging() {
		isDragging = false;
	}
	void Drag(int X, int Y) {
		double
			stepX = (X - prvX) * ((L - R) / W),
			stepY = (Y - prvY) * ((T - B) / H);
		L += stepX;
		R += stepX;
		B += stepY;
		T += stepY;
		prvX = X;
		prvY = Y;
	}
	bool IsDragging() {
		return isDragging;
	}
	void Reset() {
		L = -M_PI;
		R = M_PI;
		B = -1.5;
		T = 1.5;
	}
	bool Scale(double scale, int mX, int mY) {
		if (mX > 0 && mX < W && mY > 0 && mY < H) {


			double Xw = ScreenToWorldX(mX);
			double Yw = ScreenToWorldY(mY);
			scale = (scale > 0) ? 1.1 : 0.9;
			//int Xw = mX;
			//int Yw = mY;
			L = Xw - (Xw - L) / scale;
			R = Xw + (R - Xw) / scale;
			B = Yw - (Yw - B) / scale;
			T = Yw + (T - Yw) / scale;
			return 1;
		}
		return 0;
	}
};

#endif CAMERA_2D_H

