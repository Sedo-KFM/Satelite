#define _USE_MATH_DEFINES
#include <fstream>
#include <iostream>
#include <iomanip>
#include "modellingfullangle.h"

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
void matrixOutputHTML(int x, int y, double** A)
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

int main()
{
	setlocale(0, "");
	double angle, len;
	cout << "¬ведите угол смаза\n";
	cin >> angle;
	cout << "¬ведите длину смаза\n";
	cin >> len;
	shiftModel shift;
	shift = modellingfullangle_main(len, angle);

	//matrixOutputHTML(shift.width, shift.height, shift.matrix);

	for (int i = 0; i < shift.width; i++)
	{
		shift.matrix[i] = NULL;
		delete[] shift.matrix[i];
	}
	delete[] shift.matrix;
	return 0;
}