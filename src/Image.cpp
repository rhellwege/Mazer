#include "../include/Image.h"

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

Image::Image(int _cellLen, int _wallLen, int _channels) {
	channels = _channels;
	wallLen = _wallLen;
	cellLen = _cellLen;
	width = 0;
	height = 0;
	data = nullptr;
}

Image::Image(Maze& m, int _cellLen, int _wallLen, int _channels) {
	Image(_cellLen, _wallLen, _channels);
	update(m);
}

void Image::update(Maze& m) {
	if (data) free(data);
	width = m.getWidth() * (wallLen + cellLen) + wallLen;
	height = m.getHeight() * (wallLen + cellLen) + wallLen;
	data = (unsigned char*)malloc(channels * width * height);
	printf("w: %i, h: %i.\n", width, height);

	// draw the cells:
	for (int x = 0; x < m.getWidth(); ++x) {
		for (int y = 0; y < m.getHeight(); ++y) {
			std::pair<int, int> topLeft = std::make_pair(x*(cellLen + wallLen), y*(cellLen+wallLen));
			int outsideLen = cellLen+(wallLen*2);
			Cell* c = m.getCell(x, y);
			fillRect(topLeft.first, topLeft.second, outsideLen, outsideLen, cell_colors[c->getVal()]);
			if (c->north()) fillRect(topLeft.first, topLeft.second, outsideLen, wallLen, black);
			if (c->east()) fillRect(topLeft.first + cellLen + wallLen, topLeft.second, wallLen, outsideLen, black);
			if (c->south()) fillRect(topLeft.first, topLeft.second + cellLen + wallLen, outsideLen, wallLen, black);
			if (c->west()) fillRect(topLeft.first, topLeft.second, wallLen, outsideLen, black);
			fillRect(topLeft.first, topLeft.second, wallLen, wallLen, black);
		}
	}
}

Image::~Image() {
	free(data);
}

int Image::getWidth() {
	return width;
}
int Image::getHeight() {
	return height;
}
int Image::getChannels() {
	return channels;
}
int Image::getWallLen() {
	return wallLen;
}
int Image::getCellLen() {
	return cellLen;
}
unsigned char* Image::getData() {
	return data;
}

void Image::setPixel(int x, int y, RGB col) {
	RGB* pixelOffset = (RGB*)(data + (x + width * y) * channels);
	pixelOffset->r = col.r;
	pixelOffset->g = col.g;
	pixelOffset->b = col.b;
}

void Image::fillRect(int x, int y, int w, int h, RGB col) {
	for (int i = x; i < x + w; ++i) {
		for (int j = y; j < y + h; ++j) {
			setPixel(i, j, col);
		}
	}
}

void Image::save_to_file(const char* fname) {
	stbi_write_png(fname, width, height, channels, data, width * channels);
}
