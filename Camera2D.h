#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>

class Camera2D
{
protected:
	double L, R, B, T;					// ������� ���������� ������ ������� ������� ����
	int W, H;							// ���������� ������� ������� ����
	int WorldToScreenX(double X)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		return (X - L) * 1. / (R - L) * W;
	}
	int WorldToScreenY(double Y)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		return (T - Y) * 1. / (T - B) * H;
	}
	double ScreenToWorldX(int X)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return (double)L + (R - L) * (X + 0.5) / W;
	}
	double ScreenToWorldY(int Y)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return (double)T - (T - B) * (Y + 0.5) / H;
	}
private:
	double k = 1;
	bool isDragging;
	double prvX, prvY;
	double posX, posY;					// ������� ������������ ������� � ������� ����������� (��� ������� MoveTo � LineTo)
public:
	Camera2D(double L, double R, double B, double T) : L(L), R(R), B(B), T(T)
	{
	}
	void Clear(HDC dc)
	{
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		Rectangle(dc, 0, 0, r.right, r.bottom);
		// ������� ������� ������� ����
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
		// ������ ��������� ���������� ��� ��������� �������� ����
		// � ��� �������� �������� ������� W, H, � ����� ������������� �������� ���������� L, R, B, T ����� �������, ����� ���������� ���������� �������� �� ������������ ����
	}
	void MoveTo(double X, double Y)
	{
		posX = X;
		posY = Y;
		// ����������� ������������ ������� (posX, posY)
		// �������� ��������, ��� �� ��������� � ������� �����������
	}
	void LineTo(HDC dc, double X, double Y)
	{
		::MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), nullptr);
		::LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));
		MoveTo(X, Y);
		// ��������� ����� �� ������� ������� ������������ ������� � ����� (X, Y) � ��� ����������� � ��� �����
		// �������� ��������, ��� �� ��������� � ������� �����������
		// ��� ��������� ����� ����� ���� ������������ WinApi �������
		// ::MoveToEx(dc, Xs, Ys, nullptr) � ::LineTo(dc, Xs, Ys)
	}
	void Axes(HDC dc)
	{
		MoveTo(L, 0);
		LineTo(dc, R, 0);
		MoveTo(0, B);
		LineTo(dc, 0, T);
		// ��������� ������������ ����
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

