#define _USE_MATH_DEFINES
#include <fstream>
#include <iostream>
#include <iomanip>
#include "modellingfullangle.h"
#include "pnmhandler.h"

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

Pnm applyShift(Pnm pnm_in, ShiftModel shift)
{
	vvc_image& img_in = pnm_in.image;
	Pnm pnm_out(pnm_in.size.x, pnm_in.size.y);
	pnm_out.title = pnm_in.title;
	pnm_out.size = pnm_in.size;
	for (unsigned int img_y = 0; img_y < pnm_in.size.y; img_y++)
	{
		for (unsigned int img_x = 0; img_x < pnm_in.size.x; img_x++)
		{
			pnm_out.image[img_y][img_x] = 0;
			double brightness_sum = 0;
			for (unsigned int shift_y = 0; shift_y < shift.width && img_y + shift_y - shift.mainDot.y < pnm_in.size.y; shift_y++)
			{
				if (img_y - shift.mainDot.y >= 0)
				{
					for (unsigned int shift_x = 0; shift_x < shift.height && img_x + shift_x - shift.mainDot.x < pnm_in.size.x; shift_x++)
					{
						if (img_x - shift.mainDot.x >= 0)
						{
							pnm_out.image[img_y][img_x] += pnm_in.image[img_y + shift_y - shift.mainDot.y][img_x + shift_x - shift.mainDot.x] * shift.matrix[shift_y][shift_x];
							brightness_sum += shift.matrix[shift_y][shift_x];
						}
					}
				}
			}
		}
	}
	return pnm_out;
}

int main()
{
	setlocale(0, "");
	//pnmhandler_main();
	double angle, len;
	cout << "¬ведите угол смаза\n";
	cin >> angle;
	cout << "¬ведите длину смаза\n";
	cin >> len;
	ShiftModel shift;
	shift = modellingfullangle_main(len, angle);

	Pnm pnm_in = read_img("C:\\Temp\\img_in_1.pnm");

	// setting pnm_out
	Pnm pnm_out = applyShift(pnm_in, shift);

	// writing from pnm_out
	write_img("C:\\Temp\\img_out_1.pnm", pnm_out);

	//matrixOutput(shift.width, shift.height, shift.matrix);

	for (int i = 0; i < shift.width; i++)
	{
		shift.matrix[i] = NULL;
		delete[] shift.matrix[i];
	}
	delete[] shift.matrix;
	return 0;
}