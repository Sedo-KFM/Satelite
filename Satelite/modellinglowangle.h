#ifndef MODELLINGLOWANGLE
#define MODELLINGLOWANGLE

#include "crosquare.h"

struct ShiftModel
{
	double** matrix = nullptr;
	double width;
	double height;
	dot mainDot;
};

//  проецирует в Matrix модель смаза
void modellinglowangle_modelToMatrix(double** Matrix, int matrixLen, dot mainDot, double modelLen, double angle)
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
			Matrix[x][y] = crosquare_main({ mainDot.x + 0.5 - x, mainDot.y + 0.5 - y }, angle, modelLen) / (modelLen + 1);
		}
	}
}

//  возвращает высоту и длину модели + координаты первого пикселя
void modellinglowangle_setMMSize(double** Matrix, int mLen, int* length, int* heigth, int* xFirst, int* yFirst)
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
void modellinglowangle_setMM(double** Matrix, int mLen, double** MatrixModel, int length, int heigth, int xFirst, int yFirst)
{
	for (int col = 0; col < length; col++)
	{
		for (int row = 0; row < heigth; row++)
		{
			MatrixModel[col][row] = Matrix[col + xFirst][row + yFirst];
		}
	}

}

//  возвращает проекцию созданной модели смаза (angle < 90)
ShiftModel modellinglowangle_main(double len, double angle)
{
	int matrixLen;

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
	modellinglowangle_modelToMatrix(Matrix, matrixLen, mainDot, len, angle);
	int MM_width, MM_height, MM_xFirst, MM_yFirst;
	modellinglowangle_setMMSize(Matrix, matrixLen, &MM_width, &MM_height, &MM_xFirst, &MM_yFirst);
	mainDot.x -= MM_xFirst;
	mainDot.y -= MM_yFirst;
	ShiftModel shift;
	shift.width = MM_width;
	shift.height = MM_height;
	shift.mainDot = mainDot;
	shift.matrix = new double* [MM_width];
	for (int i = 0; i < MM_width; i++)
	{
		shift.matrix[i] = new double[MM_height];
	}
	modellinglowangle_setMM(Matrix, matrixLen, shift.matrix, MM_width, MM_height, MM_xFirst, MM_yFirst);
	for (int i = 0; i < matrixLen; i++)
	{
		Matrix[i] = NULL;
		delete[] Matrix[i];
	}
	delete[] Matrix;
	return shift;
}

#endif