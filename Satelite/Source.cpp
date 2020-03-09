#define _USE_MATH_DEFINES
#include "crosquare.h"
#include "iostream"
#include "cmath"
#include "clocale"
#include "iomanip"

using namespace std;

void matrixOutput(int N, double** A)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << left << setw(8) << A[i][j];
		cout << endl;
	}
	cout << endl;
}

void setShift(dot** Shift, dot mainDot, double angle, int len)
{
	for (int i = 0; i < len; i++)
	{
		crosquare_setSqr(Shift[i], mainDot, angle);
		mainDot.x += cos(angle * M_PI / 180);
		mainDot.y -= sin(angle * M_PI / 180);
	}
}

void modelToMatrix(double** M, double Sqr[])
{

}

int main()
{
	setlocale(0, "");
	int MatrixLen;
	double angle, len;
	cout << "¬ведите угол смаза\n";
	cin >> angle;
	cout << "¬ведите длину смаза\n";
	cin >> len;
	MatrixLen = int(len * 4);
	double** Matrix = new double* [MatrixLen];
	for (int i = 0; i < MatrixLen; i++)
		Matrix[i] = new double[MatrixLen];
	for (int i = 0; i < MatrixLen; i++)
		for (int j = 0; j < MatrixLen; j++)
			Matrix[i][j] = 0;
	dot mainDot = { MatrixLen / 2, MatrixLen / 2 };
	//Matrix[MatrixLen / 2][MatrixLen / 2] = 1;
	//matrixOutput(MatrixLen, Matrix);
	dot** Shift = new dot * [int(len) + 1.];
	for (int i = 0; i < int(len) + 1; i++)
		Shift[i] = new dot[5];
	//for (int i = 1; i < int(len) + 1; i++)
		//Shift[i]
	cout << crosquare_main({ 0.5, 0.5 }, 45) << endl;
	/*cout << "mainDot.x: " << mainDot.x << "   " << "mainDot.y: " << mainDot.y << endl
		<< "angle: = "*/
	setShift(Shift, mainDot, angle, len);
	for (int i = 0; i < int(len) + 1; i++)
	{
		cout << Shift[i][0].x << ' ' << Shift[i][0].y << endl;
		cout << Shift[i][1].x << ' ' << Shift[i][1].y << endl;
		cout << Shift[i][2].x << ' ' << Shift[i][2].y << endl;
		cout << Shift[i][3].x << ' ' << Shift[i][3].y << endl;
		cout << Shift[i][4].x << ' ' << Shift[i][4].y << endl << endl;
	}
	for (int i = 0; i < int(len) + 1; i++)
	{
		Shift[i] = NULL;
		delete[] Shift[i];
	}
	delete[] Shift;
	for (int i = 0; i < MatrixLen; i++)
	{
		Matrix[i] = NULL;
		delete[] Matrix[i];
	}
	delete[] Matrix;
	return 0;
}