#define _USE_MATH_DEFINES
#include "crosquare.h"
#include "iostream"
#include "cmath"
#include "clocale"
#include "iomanip"
#include "fstream"

using namespace std;

void matrixOutput(int N, double** A)
{
	for (int i = N - 1; i >= 0; i--)
	{
		cout << endl;
		for (int j = 0; j < N; j++)
			cout << left << setw(8) << A[j][i];
	}
	cout << endl;
}

void matrixOutputPHP(int N, double** A)
{
	ofstream phpOut;
	phpOut.open("C:\\Users\\Fedor\\Desktop\\visual.html");
	phpOut << "<!DOCTYPE html>\n"
		<< "<html lang=\"en\">\n"
		<< "<head>\n"
		<< "	<meta charset=\"UTF-8\">\n"
		<< "	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
		<< "	<title>Document</title>\n"
		<< "</head>\n"
		<< "<body>\n"
		<< "	<table border=\"0\">\n";
	for (int i = N - 1; i >= 0; i--)
	{
		phpOut << "		<tr>\n";
		for (int j = 0; j < N; j++)
		{
			phpOut << "			<td style=\"background-color: rgb(" << 255 - 255 * A[j][i] << ", " << 255 - 255 * A[j][i] << ", " << 255 - 255 * A[j][i] << "); \">\n"
				<< "				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n"
				<< "			</td>";
		}
		phpOut << "		</tr>";
	}
	phpOut << "	</table>\n"
		<< "</body>\n"
		<< "<html>";
	phpOut.close();
}

void setShiftDots(dot** Shift, dot mainDot, double angle, int len)
{
	for (int i = 0; i < len; i++)
	{
		crosquare_setSqr(Shift[i], mainDot, angle);
		mainDot.x += cos(angle * M_PI / 180);
		mainDot.y -= sin(angle * M_PI / 180);
	}
}

void setShift(dot* Shift, dot mainDot, double angle, int len)
{
	for (int i = 0; i < len; i++)
	{
		Shift[i] = mainDot;
		mainDot.x += cos(angle * M_PI / 180);
		mainDot.y -= sin(angle * M_PI / 180);
	}
}

void modelToMatrix(double** Matrix, dot* Shift, double len, double angle )
{
	int matrixLen = int(len) + 1;
	for (int i = 0; i < matrixLen; i++)
		for (int y = int(Shift[i].y + 0.5) - 1; y <= int(Shift[i].y + 0.5) + 1; y++)
			for (int x = int(Shift[i].x + 0.5) - 1; x <= int(Shift[i].x + 0.5) + 1; x++)
			{
				double sqr = crosquare_main({ Shift[i].x + 0.5 - x, Shift[i].y + 0.5 - y }, angle);
				Matrix[x][y] += sqr / len;
			}
}

int main()
{
	setlocale(0, "");
	int matrixLen;
	double angle, len;
	cout << "������� ���� �����\n";
	cin >> angle;
	cout << "������� ����� �����\n";
	cin >> len;
	matrixLen = int(len * 4);
	double** Matrix = new double* [matrixLen];
	for (int i = 0; i < matrixLen; i++)
		Matrix[i] = new double[matrixLen];
	for (int i = 0; i < matrixLen; i++)
		for (int j = 0; j < matrixLen; j++)
			Matrix[i][j] = 0;
	dot mainDot = { matrixLen / 2, matrixLen / 2 };
	//Matrix[MatrixLen / 2][MatrixLen / 2] = 1;
	//matrixOutput(MatrixLen, Matrix);
	dot** ShiftDots = new dot * [int(len) + 1];
	for (int i = 0; i < int(len) + 1; i++)
		ShiftDots[i] = new dot[5];
	dot* Shift = new dot[int(len) + 1];

	//for (int i = 1; i < int(len) + 1; i++)
		//Shift[i]
	//cout << crosquare_main({ 0.5, 0.5 }, 45) << endl;
	/*cout << "mainDot.x: " << mainDot.x << "   " << "mainDot.y: " << mainDot.y << endl
		<< "angle: = "*/
	//setShift(Shift, mainDot, angle, int(len) + 1);
	/*for (int i = 0; i < int(len) + 1; i++)
	{
		cout << Shift[i][0].x << ' ' << Shift[i][0].y << endl;
		cout << Shift[i][1].x << ' ' << Shift[i][1].y << endl;
		cout << Shift[i][2].x << ' ' << Shift[i][2].y << endl;
		cout << Shift[i][3].x << ' ' << Shift[i][3].y << endl;
		cout << Shift[i][4].x << ' ' << Shift[i][4].y << endl << endl;
	}*/
	setShift(Shift, mainDot, angle, int(len) + 1);
	/*for (int i = 0; i < int(len) + 1; i++)
		cout << Shift[i].x << ' ' << Shift[i].y << endl;*/
	modelToMatrix(Matrix, Shift, len, angle);

	matrixOutputPHP(matrixLen, Matrix);
	
	for (int i = 0; i < int(len) + 1; i++)
	{
		ShiftDots[i] = NULL;
		delete[] ShiftDots[i];
	}
	delete[] ShiftDots;
	for (int i = 0; i < matrixLen; i++)
	{
		Matrix[i] = NULL;
		delete[] Matrix[i];
	}
	delete[] Matrix;
	delete[] Shift;
	return 0;
}