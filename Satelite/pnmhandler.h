#pragma once
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>

void pnmhandler_skipComments(std::istream* is)
{
	while (is->peek() == '#')
	{
		char next_char = is->get();
		while (next_char != 0x0a)
		{
			next_char = is->get();
		}
	}
}

std::string pnmhandler_readLine(std::istream* is)
{
	pnmhandler_skipComments(is);
	std::string result;
	char lastchar = is->get();
	while (lastchar != 0x0a)
	{
		result += lastchar;
		lastchar = is->get();
	}
	return result;
}

struct size { unsigned int x; unsigned int y; };

size pnmhandler_parseSize(std::string title_size)
{
	unsigned int len = title_size.length();
	std::string
		str_x = "",
		str_y = "";
	bool space_jumped = false;
	for (int i = 0; i < len; i++)
	{
		char next_char = title_size[i];
		if (next_char == ' ')
		{
			space_jumped = true;
			continue;
		}
		if (space_jumped)
		{
			str_y += next_char;
		}
		else
		{
			str_x += next_char;
		}
	}
	return { (unsigned int)std::stoi(str_x), (unsigned int)std::stoi(str_y) };
}

int pnmhandler_main()
{
	// reading in img_in
	std::ifstream ifs("C:\\Temp\\img_in.pnm", std::ios::binary);
	std::string
		title_format = pnmhandler_readLine(&ifs) + '\n',
		title_size = pnmhandler_readLine(&ifs) + '\n',
		title_deepcolor = pnmhandler_readLine(&ifs) + '\n';
	size img_size = pnmhandler_parseSize(title_size);
	unsigned int deepcolor = (unsigned int)std::stoi(title_deepcolor);
	std::vector<std::vector<unsigned char>> img_in{};
	img_in.resize(img_size.y);
	for (unsigned int i = 0; i < img_size.y; i++)
	{
		img_in[i].resize(img_size.x);
	}
	for (unsigned int y = 0; y < img_size.y; y++)
	{
		for (unsigned int x = 0; x < img_size.x; x++)
		{
			img_in[y][x] = ifs.get();
		}
	}

	// copying in img_out
	std::vector<std::vector<unsigned char>> img_out{};
	img_out.resize(img_size.y);
	for (unsigned int i = 0; i < img_size.y; i++)
	{
		img_out[i].resize(img_size.x);
	}
	for (unsigned int y = 0; y < img_size.y; y++)
	{
		for (unsigned int x = 0; x < img_size.x; x++)
		{
			img_out[y][x] = img_in[y][x];
		}
	}

	// work with img_out
	for (int y = 0; y < img_size.y; y++)
	{
		for (unsigned int x = 0; x < img_size.x; x++)
		{
			img_out[y][x] = 255 - img_out[y][x];
		}
	}

	// writing from img_out
	std::ofstream ofs("C:\\Temp\\img_out.pnm", std::ios_base::binary);
	ofs.write(title_format.c_str(), title_format.size());
	ofs.write(title_size.c_str(), title_size.size());
	ofs.write(title_deepcolor.c_str(), title_deepcolor.size());
	for (unsigned int y = 0; y < img_size.y; y++)
	{
		for (unsigned int x = 0; x < img_size.x; x++)
		{
			ofs.put(img_out[y][x]);
		}
	}
	ofs.close();

	return 0;
}