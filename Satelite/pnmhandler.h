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

struct Title { std::string format; std::string size; std::string deepcolor; };

struct Size { unsigned int x; unsigned int y; };

typedef std::vector<std::vector<unsigned char>> vvc_image;

struct Pnm
{
	Title title;
	Size size;
	vvc_image image;

	Pnm() {}

	Pnm(unsigned int x, unsigned int y)
	{
		resize(x, y);
	}

	void resize(unsigned int x, unsigned int y)
	{
		image.resize(y);
		for (unsigned int i = 0; i < y; i++)
		{
			image[i].resize(x);
		}
	}
};

Size pnmhandler_parseSize(std::string title_size)
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

Pnm read_img(const std::string filename)
{
	std::ifstream ifs(filename, std::ios::binary);
	Pnm img_in;
	img_in.title.format = pnmhandler_readLine(&ifs) + '\n';
	img_in.title.size = pnmhandler_readLine(&ifs) + '\n';
	img_in.title.deepcolor = pnmhandler_readLine(&ifs) + '\n';
	img_in.size = pnmhandler_parseSize(img_in.title.size);
	unsigned int deepcolor = (unsigned int)std::stoi(img_in.title.deepcolor);
	img_in.resize(img_in.size.x, img_in.size.y);
	for (unsigned int y = 0; y < img_in.size.y; y++)
	{
		for (unsigned int x = 0; x < img_in.size.x; x++)
		{
			img_in.image[y][x] = ifs.get();
		}
	}
	ifs.close();
	return img_in;
}

void write_img(const std::string filename, const Pnm& img_out)
{
	std::ofstream ofs(filename, std::ios_base::binary);
	ofs.write(img_out.title.format.c_str(), img_out.title.format.size());
	ofs.write(img_out.title.size.c_str(), img_out.title.size.size());
	ofs.write(img_out.title.deepcolor.c_str(), img_out.title.deepcolor.size());
	for (unsigned int y = 0; y < img_out.size.y; y++)
	{
		for (unsigned int x = 0; x < img_out.size.x; x++)
		{
			ofs.put(img_out.image[y][x]);
		}
	}
	ofs.close();
}
