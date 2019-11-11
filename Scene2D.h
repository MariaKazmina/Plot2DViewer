#define _USE_MATH_DEFINES
#include <cmath>
#ifndef SCENE_2D_H
#define SCENE_2D_H

#include "Camera2D.h"

class Scene2D : public Camera2D
{
private:
	typedef double(*Func)(double);
public:
	Scene2D(double L, double R, double B, double T) : Camera2D(L, R, B, T)
	{
	}
	void Plot(HDC dc, Func f, bool axes = true)
	{
		Axes(dc);
		MoveTo(L, 0);
		double step = M_PI / 4;

		for (double phi = 0; phi < 2 * M_PI; phi+=step)
		{
			double rad = polarRose(phi);
		}

		// Построение графика функции f
		// Здесь передаваемая в качестве параметра функция f вызывается как обычная функция:
		// f(x);
	}



	//void Plot(HDC dc, Func f, bool axes=true)
	//{
	//	if (axes)
	//		Axes(dc);
	//	MoveTo(L, 0);
	//	for (double x = L; x <= R; x += (R - L) / W) {
	//		LineTo(dc, x, f(x));
	//	}
	//	// Построение графика функции f
	//	// Здесь передаваемая в качестве параметра функция f вызывается как обычная функция:
	//	// f(x);
	//}

	//void BiPlot(HDC dc, Func R1, Func R2) {
	//	Axes(dc);
	//	MoveTo(L, 0);
	//	//SetA(R - L);
	//	//SetC(sqrt((R - L)*(R - L) - (T - B)*(T - B)));
	//	double tmin = GetTmin();
	//	double tmax = GetTmax();
	//	double step = GetStep();

	//	for (double t = tmin; t <= tmax; t += step) {
	//		coords c = ToCentreBipolarPlot(R1, R2, t);
	//		LineTo(dc, c.x, c.y);
	//	}

	//	for (double t = tmax; t >= tmin; t -= step) {
	//		coords c = ToCentreBipolarPlot(R1, R2, t);
	//		LineTo(dc, c.x, -c.y);
	//	}
	//	coords c = ToCentreBipolarPlot(R1, R2, tmin);
	//	LineTo(dc, c.x, c.y);
	//}

};

#endif SCENE_2D_H
