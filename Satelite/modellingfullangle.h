#ifndef MODELLINGFULLANGLE
#define MODELLINGFULLANGLE

#include "modellinglowangle.h"


//  поворачивает матрицу и точку на 90 градусов против часовой
void modellingfullangle_matrixRotate(ShiftModel* shift)
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
	shift->mainDot.y = width - 1. - shift->mainDot.x;
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

//  возвращает проекцию созданной модели смаза
ShiftModel modellingfullangle_main(double len, double angle)
{
	int rotate = 0;
	if (angle >= 90)
	{
		rotate = angle / 90;
		angle = angle - rotate * 90.;
	}
	ShiftModel shift;
	shift = modellinglowangle_main(len, angle);
	if (rotate > 0)
	{
		for (int i = 0; i < rotate; i++)
		{
			modellingfullangle_matrixRotate(&shift);
		}
	}
	return shift;
}

#endif