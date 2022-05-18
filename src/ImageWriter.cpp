#include "../include/ImageWriter.h"

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../deps/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../deps/stb_image_write.h"

ImageWriter::ImageWriter(int _cellLen, int _wallLen, int _channels) {
	
	channels = _channels;
	wallLen = _wallLen;
	cellLen = _cellLen;
	width = 0;
	height = 0;
	data = nullptr;
}

void ImageWriter::update(Maze& m) {
	if (data != nullptr) free(data);
	width = m.getWidth() * (wallLen + cellLen) + wallLen;
	height = m.getHeight() * (wallLen + cellLen) + wallLen;
	
	data = (unsigned char*)malloc(channels * width * height);

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

ImageWriter::ImageWriter(Maze& m, int _cellLen, int _wallLen, int _channels) : ImageWriter(_cellLen, _wallLen, _channels) {
	update(m);
}


ImageWriter::~ImageWriter() {
	free(data);
}

int ImageWriter::getWidth() {
	return width;
}
int ImageWriter::getHeight() {
	return height;
}
int ImageWriter::getChannels() {
	return channels;
}
int ImageWriter::getWallLen() {
	return wallLen;
}
int ImageWriter::getCellLen() {
	return cellLen;
}
unsigned char* ImageWriter::getData() {
	return data;
}

void ImageWriter::setPixel(int x, int y, RGB col) {
	RGB* pixelOffset = (RGB*)(data + (x + width * y) * channels);
	pixelOffset->r = col.r;
	pixelOffset->g = col.g;
	pixelOffset->b = col.b;
}

void ImageWriter::fillRect(int x, int y, int w, int h, RGB col) {
	for (int i = x; i < x + w; ++i) {
		for (int j = y; j < y + h; ++j) {
			setPixel(i, j, col);
		}
	}
}

void ImageWriter::save_to_file(const char* fname) {
	printf("saving %s...\n", fname);
	stbi_write_png(fname, width, height, channels, data, width * channels);
}
