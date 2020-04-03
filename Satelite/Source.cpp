#define _USE_MATH_DEFINES
#include "crosquare.h"
#include "iostream"
#include "cmath"
#include "clocale"
#include "iomanip"
#include "fstream"

using namespace std;

struct shiftModel
{
	double** matrix;
	dot mainDot;
};

//  выводит на экран матрицу
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

//  выводит в php матрицу
void matrixOutputPHP(int x, int y, double** A)
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
		<< "	<body style=\"height: 100vh; width: 100vw; margin : 0; display: flex; justify-content: center; align-items: center; \">\n"
		<< "		<div style=\"display: flex; border: 1px solid black; \">\n"
		<< "			<table border=\"0\">\n";
	for (int i = y - 1; i >= 0; i--)
	{
		phpOut << "				<tr>\n";
		for (int j = 0; j < x; j++)
		{
			phpOut << "					<td style=\"background-color: rgb(" << 255 - 255 * A[j][i] << ", " << 255 - 255 * A[j][i] << ", " << 255 - 255 * A[j][i] << "); \">\n"
				<< "						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n"
				<< "					</td>\n";
		}
		phpOut << "				</tr>\n";
	}
	phpOut << "			</table>\n"
		<< "		</div>\n"
		<< "	</body>\n"
		<< "</html>";
	phpOut.close();
}

//  проецирует в Matrix модель смаза
void modelToMatrix(double** Matrix, int matrixLen, dot mainDot, double modelLen, double angle )
{
/*	int matrixLen = int(len) + 1;
	for (int i = 0; i < matrixLen; i++)
		for (int y = int(Shift[i].y + 0.5) - 1; y <= int(Shift[i].y + 0.5) + 1; y++)
			for (int x = int(Shift[i].x + 0.5) - 1; x <= int(Shift[i].x + 0.5) + 1; x++)
			{
				double sqr = crosquare_main({ Shift[i].x + 0.5 - x, Shift[i].y + 0.5 - y }, angle);
				Matrix[x][y] += sqr / (len + 1);
			}*/
	for (int y = 0; y < matrixLen; y++)
	{
		for (int x = 0; x < matrixLen; x++)
		{
			Matrix[x][y] = crosquare_main({mainDot.x + 0.5 - x, mainDot.y + 0.5 - y}, angle, modelLen) / (modelLen + 1);
		}
	}
}

//  возвращает высоту и длину модели + координаты первого пикселя
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

//  обрезает Matrix до MatrixModel
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

//  shiftModel main()
int main()
{
	setlocale(0, "");
	int matrixLen;
	double angle, len;
	cout << "Введите угол смаза\n";
	cin >> angle;
	cout << "Введите длину смаза\n";
	cin >> len;
	matrixLen = int(len * 4 + 1) + 4;
	double** Matrix = new double* [matrixLen];
	for (int i = 0; i < matrixLen; i++)
	{
		Matrix[i] = new double[matrixLen];
	}
	for (int i = 0; i < matrixLen; i++)
	{
		for (int j = 0; j < matrixLen; j++)
		{
			Matrix[i][j] = 0;
		}
	}
	dot mainDot = { matrixLen / 2, matrixLen / 2 };
	modelToMatrix(Matrix, matrixLen, mainDot, len, angle);
	int MM_length, MM_heigth, MM_xFirst, MM_yFirst;
	setMMSize(Matrix, matrixLen, &MM_length, &MM_heigth, &MM_xFirst, &MM_yFirst);
	mainDot.x -= MM_xFirst;
	mainDot.y -= MM_yFirst;
	shiftModel shift;
	shift.mainDot = mainDot;
	shift.matrix = new double* [MM_length];
	for (int i = 0; i < MM_length; i++)
	{
		shift.matrix[i] = new double[MM_heigth];
	}
	setMM(Matrix, matrixLen, shift.matrix, MM_length, MM_heigth, MM_xFirst, MM_yFirst);
	matrixOutputPHP(MM_length, MM_heigth, shift.matrix);

	for (int i = 0; i < matrixLen; i++)
	{
		Matrix[i] = NULL;
		delete[] Matrix[i];
	}
	delete[] Matrix;
	cout << endl << shift.mainDot.x << ' ' << shift.mainDot.y << endl;
	//return shift;
	return 0;
}