#include "modellinglowangle.h"
#include <iomanip>
#include <iostream>
#include <clocale>

using namespace std;

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

void matrixRotate(shiftModel* shift)
{
	int height = shift->height;
	int width = shift->width;
	double** matrixBuf = new double* [width];
	for (int i = 0; i < width; i++)
	{
		matrixBuf[i] = new double[height];
	}
	for (int col = 0; col < width; col++)
	{
		for (int row = 0; row < height; row++)
		{
			matrixBuf[col][row] = shift->matrix[col][row];
		}
	}
	for (int i = 0; i < width; i++)
	{
		shift->matrix[i] = NULL;
		delete[] shift->matrix[i];
	}
	delete[] shift->matrix;

	shift->matrix = new double* [height];
	for (int i = 0; i < height; i++)
	{
		shift->matrix[i] = new double[width];
	}
	for (int col = 0; col < height; col++)
	{
		for (int row = 0; row < width; row++)
		{
			shift->matrix[col][row] = matrixBuf[row][height - 1 - col];
		}
	}

	int buf = shift->mainDot.y;
	shift->mainDot.y = width - 1 - shift->mainDot.x;
	shift->mainDot.x = buf;

	double temp = shift->width;
	shift->width = shift->height;
	shift->height = temp;

	for (int i = 0; i < width; i++)
	{
		matrixBuf[i] = NULL;
		delete[] matrixBuf[i];
	}
	delete[] matrixBuf;
}

int main()
{
	setlocale(0, "");
	double angle, len;
	int rotate = 0;
	cout << "¬ведите угол смаза\n";
	cin >> angle;
	cout << "¬ведите длину смаза\n";
	cin >> len;
	if (angle >= 90)
	{
		rotate = angle / 90;
		angle = angle - rotate * 90;
	}
	shiftModel shift;
	shift = modellinglowangle_main(len, angle);
	matrixOutputPHP(shift.width, shift.height, shift.matrix);
	if (rotate > 0)
	{
		for (int i = 0; i < rotate; i++)
		{
			matrixRotate(&shift);
		}
	}
	matrixOutputPHP(shift.width, shift.height, shift.matrix);
	cout << "mainDot: " << shift.mainDot.x << ' ' << shift.mainDot.y << endl;

	for (int i = 0; i < shift.width; i++)
	{
		shift.matrix[i] = NULL;
		delete[] shift.matrix[i];
	}
	delete[] shift.matrix;
	return 0;
}