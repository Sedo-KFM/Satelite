#ifndef CROSQUARE_H
#define CROSQUARE_H

#define _USE_MATH_DEFINES
#include "iostream"
#include "cmath"
#include "clocale"

using namespace std;

struct dot
{
	double x;
	double y;
};

struct section
{
	struct dot A;
	struct dot B;
};

struct line
{
	double k;
	double b;
};

line crosquare_lineFromSection(section S)
{
	double k;
	k = (S.B.y - S.A.y) / (S.B.x - S.A.x);
	return { k, S.A.y - k * S.A.x };
}

double crosquare_lineFunction(line f, double x)
{
	return f.k * x + f.b;
}

double crosquare_lineFunctionReverse(line f, double y)
{
	return (y - f.b) / f.k;
}

double crosquare_zone(dot D[], int q)
{
	double sum = 0;
	for (int i = 0; i < q; i++)
		sum += (D[i].x + D[i + 1].x) * (D[i].y - D[i + 1].y);
	return sum * 0.5;
}

double crosquare_dist(dot A, dot B)
{
	double l = abs(B.x - A.x);
	double h = abs(B.y - A.y);
	return pow(pow(l, 2) + pow(h, 2), 0.5);
}

bool crosquare_equalDot(dot A, dot B)
{
	if (A.x == B.x && A.y == B.y)
		return true;
	else
		return false;
}

dot crosquare_cross(section a, section b)
{
	return
	{
		((a.A.x * a.B.y - a.A.y * a.B.x) * (b.A.x - b.B.x) - (a.A.x - a.B.x) * (b.A.x * b.B.y - b.A.y * b.B.x)) /
		((a.A.x - a.B.x) * (b.A.y - b.B.y) - (a.A.y - a.B.y) * (b.A.x - b.B.x)),
		((a.A.x * a.B.y - a.A.y * a.B.x) * (b.A.y - b.B.y) - (a.A.y - a.B.y) * (b.A.x * b.B.y - b.A.y * b.B.x)) /
		((a.A.x - a.B.x) * (b.A.y - b.B.y) - (a.A.y - a.B.y) * (b.A.x - b.B.x))
	};
}

bool crosquare_isCross(section s0, section s1)
{
	line l1 = crosquare_lineFromSection(s1);
	if (s0.A.y == s0.B.y)
	{
		if (((s1.A.y >= s0.A.y && s1.B.y <= s0.A.y) || (s1.A.y <= s0.A.y && s1.B.y >= s0.A.y)) &&
			crosquare_lineFunctionReverse(l1, s0.A.y) >= 0 && crosquare_lineFunctionReverse(l1, s0.A.y) <= 1)
			return true;
		else
			return false;
	}
	else if (s0.A.x == s0.B.x)
	{
		if (((s1.A.x >= s0.A.x && s1.B.x <= s0.A.x) || (s1.A.x <= s0.A.x && s1.B.x >= s0.A.x)) &&
			crosquare_lineFunction(l1, s0.A.x) >= 0 && crosquare_lineFunction(l1, s0.A.x) <= 1)
			return true;
		else return false;
	}
	else
		return false;
}

bool crosquare_isIn(dot* Sqr, dot D)
{
	if (Sqr[0].x == 0 && Sqr[0].y == 0 && Sqr[2].x == 1 && Sqr[2].y == 1)
	{
		if (D.x > 0 && D.x < 1 && D.y > 0 && D.y < 1)
			return true;
	}
	else if ((D.y < crosquare_lineFunction(crosquare_lineFromSection({ Sqr[0], Sqr[1] }), D.x))
		&& (D.y > crosquare_lineFunction(crosquare_lineFromSection({ Sqr[3], Sqr[4] }), D.x))
		&& (D.y < crosquare_lineFunction(crosquare_lineFromSection({ Sqr[1], Sqr[2] }), D.x))
		&& (D.y > crosquare_lineFunction(crosquare_lineFromSection({ Sqr[2], Sqr[3] }), D.x)))
		return true;
	return false;
}

int crosquare_quantityOfTops(dot* Sqr0, dot* Sqr1)
{
	int quant = 0;
	for (int i = 0; i < 4; i++)
	{
		if (crosquare_isIn(Sqr1, Sqr0[i]))
			quant++;
		if (crosquare_isIn(Sqr0, Sqr1[i]))
			quant++;
	}
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			if (crosquare_isCross({ Sqr0[j], Sqr0[j + 1] }, { Sqr1[i], Sqr1[i + 1] }))
				quant++;
		}
	return quant;
}

bool crosquare_dotInArr(int len, dot A[], dot D)
{
	for (int i = 0; i < len; i++)
		if (crosquare_equalDot(A[i], D))
			return true;
	return false;
}

double crosquare_angleOfSections(section a, section b)
{
	dot A = { a.B.x - a.A.x, a.B.y - a.A.y };
	dot B = { b.B.x - b.A.x, b.B.y - b.A.y };
	return 180 / M_PI * acos((A.x * B.x + A.y * B.y) / (pow(pow(A.x, 2) + pow(A.y, 2), 0.5) * pow(pow(B.x, 2) + pow(B.y, 2), 0.5)));
}

double crosquare_angleDots(dot A, dot B)
{
	double t = atan2(A.y - B.y, A.x - B.x) * 180 / M_PI;
	if (t > 0)
		return 270 - t;
	else
	{
		if (t <= -90)
			return -t - 90;
		else
			return -t + 270;
	}
}

void crosquare_lastConvexer(int len, dot* D)
{
	dot* D_t = new dot[len];
	for (int i = 1; i < len; i++)
		D_t[i] = { 10, 10 };
	D_t[0] = D[0];
	for (int i = 1; i < len; i++)
		if (D[i].x < D_t[0].x)
			D_t[0] = D[i];
	for (int i = 1; i < len; i++)
	{
		for (int j = 0; j < len; j++)
			if (!crosquare_dotInArr(len, D_t, D[j]))
				D_t[i] = D[j];
		for (int j = 0; j < len; j++)
		{
			if (crosquare_angleDots(D_t[i - 1], D[j]) < crosquare_angleDots(D_t[i - 1], D_t[i]) && !crosquare_dotInArr(len, D_t, D[j]))
				D_t[i] = D[j];
		}
	}
	for (int i = 0; i < len; i++)
	{
		D[i] = D_t[i];
	}
	D[len] = D_t[0];
	delete[] D_t;
}

dot crosquare_rotateDot(dot mainDot, dot D, double angle)
{
	dot tD = { { D.x - mainDot.x }, { D.y - mainDot.y } };
	double tAngle = M_PI / 180 * angle;
	return { tD.x * cos(tAngle) + tD.y * sin(tAngle) + mainDot.x, -tD.x * sin(tAngle) + tD.y * cos(tAngle) + mainDot.y };
}

void crosquare_setSqr(dot* Sqr, dot mainDot, double angle, double modelLength)
{
	dot* tSqr = new dot[5];
	tSqr[0] = { mainDot.x - 0.5, mainDot.y - 0.5 };
	tSqr[0] = crosquare_rotateDot(mainDot, tSqr[0], angle);
	tSqr[1] = { mainDot.x - 0.5, mainDot.y + 0.5 };
	tSqr[1] = crosquare_rotateDot(mainDot, tSqr[1], angle);
	tSqr[2] = { mainDot.x + 0.5 + modelLength, mainDot.y + 0.5 };
	tSqr[2] = crosquare_rotateDot(mainDot, tSqr[2], angle);
	tSqr[3] = { mainDot.x + 0.5 + modelLength, mainDot.y - 0.5 };
	tSqr[3] = crosquare_rotateDot(mainDot, tSqr[3], angle);
	tSqr[4] = tSqr[0];
	for (int i = 0; i < 5; i++)
		Sqr[i] = tSqr[i];
	delete[] tSqr;
}

void crosquare_fillPolygon(dot* Sqr0, dot* Sqr1, dot* Polygon)
{
	int n = 0;
	for (int i = 0; i < 4; i++)
	{
		if (crosquare_isIn(Sqr1, Sqr0[i]))
		{
			Polygon[n] = Sqr0[i];
			n++;
		}
		if (crosquare_isIn(Sqr0, Sqr1[i]))
		{
			Polygon[n] = Sqr1[i];
			n++;
		}
	}
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			if (crosquare_isCross({ Sqr0[j], Sqr0[j + 1] }, { Sqr1[i], Sqr1[i + 1] }))
			{
				Polygon[n] = crosquare_cross({ Sqr0[j], Sqr0[j + 1] }, { Sqr1[i], Sqr1[i + 1] });
				n++;
			}
		}
}

double crosquare_zoneZeroAngle(dot mainDot, double len)
{
	if (mainDot.y >= 1.5 || mainDot.y <= -0.5 || mainDot.x >= 1.5 || mainDot.x + len <= -0.5)
	{
		return 0;
	}
	else
	{
		double S = 1;
		S *= (1 - abs(mainDot.y - 0.5));
		if (mainDot.x > 0.5)
		{
			S *= (1.5 - mainDot.x);
		}
		if (mainDot.x + len < 0.5)
		{
			S *= (0.5 + mainDot.x + len);
		}
		return S;
	}


	/*double dx = x - 0.5, dy = y - 0.5;
	if (abs(dx) >= 1 || abs(dy) >= 1)
		return 0;
	else
		return (1 - abs(dx)) * (1 - abs(dy));*/
}

double crosquare_main(dot mainDot, double angle, double modelLength)
{
	setlocale(0, "");
	dot Sqr0[5] = { {0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0} }, Sqr1[5];

		if (angle == 0)
			return crosquare_zoneZeroAngle(mainDot, modelLength);
		else
		{
			crosquare_setSqr(Sqr1, mainDot, angle, modelLength);
			int tops = crosquare_quantityOfTops(Sqr0, Sqr1);
			if (tops == 0)
			{
				return 0;
			}
			else
			{
				dot* Polygon = new dot[tops + 1];
				crosquare_fillPolygon(Sqr0, Sqr1, Polygon);
				crosquare_lastConvexer(tops, Polygon);
				double toRet = crosquare_zone(Polygon, tops);
				delete[] Polygon;
				return toRet;
			}
		}

	return 0;
}

#endif