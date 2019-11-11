#define _USE_MATH_DEFINES
#include <cmath>

double					// Исходные значения параметров L, R, B, T
L = -M_PI,
R = M_PI,
B = -1.5,
T = 1.5;



double Parabola(double x)
{
	return x * x - 2;
}

double Sinusoid(double x)
{
	return sin(x);
}

double polarRose(double phi)
{
	double r = cos(2 * phi);
	return r;
}

double xFromPolarToDecart(double r, double phi)
{
	double x;
	return  x = r * cos(phi);
}

double yFromPolarToDecart(double r, double phi)
{
	double y;
	return  y = r * sin(phi);
}
