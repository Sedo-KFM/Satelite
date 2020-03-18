#define _USE_MATH_DEFINES
#include "crosquare.h"
#include "iostream"
#include "cmath"
#include "clocale"
#include "iomanip"
#include "fstream"

using namespace std;

void matrixOutput(int x, int y, double** A)
{
	for (int i = y - 1; i >= 0; i--)
	{
		cout << endl;
		for (int j = 0; j < x; j++)
			cout << left << setw(7) << setprecision(2) << A[j][i];
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
				Matrix[x][y] += sqr / (len + 1);
			}
}

void setMMSize(double** Matrix, int mLen, int* length, int* heigth, int* xFirst, int* yFirst)
{
	bool clear;
	*length = 0;
	*heigth = 0;
	for (int row = mLen - 1; row >= 0; row--)
	{
		clear = true;
		for (int col = mLen - 1; col >= 0; col--)
		{
			if (Matrix[col][row] > 0)
			{
				clear = false;
				*yFirst = row;
			}
		}
		if (!clear)
			(*heigth)++;
	}
	for (int col = mLen - 1; col >= 0; col--)
	{
		clear = true;
		for (int row = mLen - 1; row >= 0; row--)
		{
			if (Matrix[col][row] > 0)
			{
				clear = false;
				*xFirst = col;
			}
		}
		if (!clear)
			(*length)++;
	}
}

void setMM(double** Matrix, int mLen, double** MatrixModel, int length, int heigth, int xFirst, int yFirst)
{
	for (int col = 0; col < length; col++)
	{
		for (int row = 0; row < heigth; row++)
		{
			MatrixModel[col][row] = Matrix[col + xFirst][row + yFirst];
		}
	}

}

int main()
{
	setlocale(0, "");
	int matrixLen;
	double angle, len;
	cout << "¬ведите угол смаза\n";
	cin >> angle;
	cout << "¬ведите длину смаза\n";
	cin >> len;
	matrixLen = int(len * 4 + 1) + 4;
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

	setShift(Shift, mainDot, angle, int(len) + 1);
	modelToMatrix(Matrix, Shift, len, angle);
	matrixOutput(matrixLen, matrixLen, Matrix);
	matrixOutputPHP(matrixLen, Matrix);
	
	int MM_length, MM_heigth, MM_xFirst, MM_yFirst;
	setMMSize(Matrix, matrixLen, &MM_length, &MM_heigth, &MM_xFirst, &MM_yFirst);
	cout << MM_length << ' ' << MM_heigth << endl;
	double** MatrixModel = new double* [MM_length];
	for (int i = 0; i < MM_length; i++)
		MatrixModel[i] = new double[MM_heigth];
	setMM(Matrix, matrixLen, MatrixModel, MM_length, MM_heigth, MM_xFirst, MM_yFirst);
	matrixOutput(MM_length, MM_heigth, MatrixModel);

	delete[] Shift;
	for (int i = 0; i < MM_length; i++)
	{
		MatrixModel[i] = NULL;
		delete[] MatrixModel[i];
	}
	delete[] MatrixModel;
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
	return 0;
}