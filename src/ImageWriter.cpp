#include "../include/ImageWriter.h"

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../deps/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../deps/stb_image_write.h"

ImageWriter::ImageWriter(int w, int h, int _channels) {
	width = w;
	height = h;
	channels = _channels;
	data = (unsigned char*)malloc(channels * width * height);
}

void ImageWriter::resize(int w, int h) {
	freeImage();
	width = w;
	height = h;
	data = (unsigned char*)malloc(channels * width * height);
}

ImageWriter::~ImageWriter() {
	freeImage();
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
unsigned char* ImageWriter::getData() {
	return data;
}

void ImageWriter::setPixel(int x, int y, RGBA col) {
	RGBA* pixelOffset = (RGBA*)(data + (x + width * y) * channels);
	pixelOffset->r = col.r;
	pixelOffset->g = col.g;
	pixelOffset->b = col.b;
	pixelOffset->a = col.a;
}

void ImageWriter::fillRect(int x, int y, int w, int h, RGBA col) {
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

void ImageWriter::freeImage() {
	if (data != nullptr) free(data);
}