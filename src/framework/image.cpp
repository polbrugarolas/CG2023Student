#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <utility>
#include "GL/glew.h"
#include "../extra/picopng.h"
#include "image.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"

Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

// Copy constructor
Image::Image(const Image& c)
{
	pixels = NULL;
	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
}

// Assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;

	if(c.pixels)
	{
		pixels = new Color[width*height*bytes_per_pixel];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}

void Image::Render()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(width, height, bytes_per_pixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

// Change image size (the old one will remain in the top-left corner)
void Image::Resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = GetPixel(x,y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

// Change image size and scale the content
void Image::Scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = GetPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++x)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.SetPixel( x, y, GetPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::FlipY()
{
	int row_size = bytes_per_pixel * width;
	Uint8* temp_row = new Uint8[row_size];
#pragma omp simd
	for (int y = 0; y < height * 0.5; y += 1)
	{
		Uint8* pos = (Uint8*)pixels + y * row_size;
		memcpy(temp_row, pos, row_size);
		Uint8* pos2 = (Uint8*)pixels + (height - y - 1) * row_size;
		memcpy(pos, pos2, row_size);
		memcpy(pos2, temp_row, row_size);
	}
	delete[] temp_row;
}

bool Image::LoadPNG(const char* filename, bool flip_y)
{
	std::string sfullPath = absResPath(filename);
	std::ifstream file(sfullPath, std::ios::in | std::ios::binary | std::ios::ate);

	// Get filesize
	std::streamsize size = 0;
	if (file.seekg(0, std::ios::end).good()) size = file.tellg();
	if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	if (!size)
		return false;

	std::vector<unsigned char> buffer;

	// Read contents of the file into the vector
	if (size > 0)
	{
		buffer.resize((size_t)size);
		file.read((char*)(&buffer[0]), size);
	}
	else
		buffer.clear();

	std::vector<unsigned char> out_image;

	if (decodePNG(out_image, width, height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size(), true) != 0)
		return false;

	size_t bufferSize = out_image.size();
	unsigned int originalBytesPerPixel = (unsigned int)bufferSize / (width * height);
	
	// Force 3 channels
	bytes_per_pixel = 3;

	if (originalBytesPerPixel == 3) {
		pixels = new Color[bufferSize];
		memcpy(pixels, &out_image[0], bufferSize);
	}
	else if (originalBytesPerPixel == 4) {

		unsigned int newBufferSize = width * height * bytes_per_pixel;
		pixels = new Color[newBufferSize];

		unsigned int k = 0;
		for (unsigned int i = 0; i < bufferSize; i += originalBytesPerPixel) {
			pixels[k] = Color(out_image[i], out_image[i + 1], out_image[i + 2]);
			k++;
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	return true;
}

// Loads an image from a TGA file
bool Image::LoadTGA(const char* filename, bool flip_y)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int imageSize;
	unsigned int bytesPerPixel;

    std::string sfullPath = absResPath( filename );

	FILE * file = fopen( sfullPath.c_str(), "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << sfullPath.c_str() << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		fclose(file);
		delete tgainfo;
		return NULL;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	// Save info in image
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	// Convert to float all pixels
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			// Make sure we don't access out of memory
			if( (pos < imageSize) && (pos + 1 < imageSize) && (pos + 2 < imageSize))
				SetPixel(x, height - y - 1, Color(tgainfo->data[pos + 2], tgainfo->data[pos + 1], tgainfo->data[pos]));
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::SaveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	std::string fullPath = absResPath(filename);
	FILE *file = fopen(fullPath.c_str(), "wb");
	if ( file == NULL )
	{
		perror("Failed to open file: ");
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	// Convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[y*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);

	return true;
}

void Image::DrawRect(int x, int y, int w, int h, const Color& c)
{
	for (int i = 0; i < w; ++i) {
		SetPixel(x + i, y, c);
		SetPixel(x + i, y + h, c);
	}

	for (int j = 0; j < h; ++j) {
		SetPixel(x, y + j, c);
		SetPixel(x + w, y + j, c);
	}
}

#ifndef IGNORE_LAMBDAS

// You can apply and algorithm for two images and store the result in the first one
// ForEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void ForEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif

//***********OUR FUNCTIONS*****************
//***********OUR FUNCTIONS*****************
//***********OUR FUNCTIONS*****************
//***********OUR FUNCTIONS*****************
void Image::DrawLineBresenham(int x0, int y0, int x1, int y1, const Color& c)
{
	// 4th and 5th octants
	if (x1 < x0) {
		int t = x1;
		x1 = x0;
		x0 = t;

		t = y1;
		y1 = y0;
		y0 = t;
	}

	int dx, dy, iE, iNE, d, x, y;

	dx = x1 - x0;
	dy = y1 - y0;

	// 8th octant
	if (y0 > y1)
		dy = -dy;

	iE = 2 * dy;
	iNE = 2 * (dy - dx);
	d = 2 * dy - dx;
	x = x0;
	y = y0;

	SetPixelSafe(x, y, c);

	if (dx > dy) {

		while (x < x1) {

			if (d <= 0) {
				d += iE;
				x += 1;
			}
			else {
				d += iNE;
				x += 1;
				if (y0 > y1) // 8th octant
					y -= 1;
				else
					y += 1;
			}

			SetPixelSafe(x, y, c);

		}
	}
	// 2nd and 6th octants
	else {

		iE = 2 * dx;
		iNE = 2 * (dx - dy);
		d = 2 * dx - dy;

		// 3rd and 7th octants
		if (y1 < y0) {
			int t = x1;
			x1 = x0;
			x0 = t;

			t = y1;
			y1 = y0;
			y0 = t;

			x = x0;
			y = y0;
		}

		while (y < y1) {

			if (d <= 0) {
				d += iE;
				y += 1;
			}
			else {
				d += iNE;
				y += 1;
				if (x0 > x1)
					x -= 1;
				else
					x += 1;
			}

			SetPixelSafe(x, y, c);
		}
	}
}



void Image::DrawTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Color& color) {
	std::vector<cell> aet;
	aet.resize(this->height);


	for (int i = 0; i < this->height; i++){
		aet[i].maxx = INT_MIN;
		aet[i].minx = INT_MAX;
	}


	ScanLineBresenham(p0.x, p0.y, p1.x, p1.y, aet);
	ScanLineBresenham(p0.x, p0.y, p2.x, p2.y, aet);
	ScanLineBresenham(p1.x, p1.y, p2.x, p2.y, aet);
	

	for (int i = 0; i < this->height; i++) {
		for (int j = aet[i].minx; j <= aet[i].maxx; j++){
			SetPixelSafe(j, i, color);
		}
	}
	

}
void Image::DrawTriangleInterpolated(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Color& c0, const Color& c1, const Color& c2) {
	std::vector<cell> aet;
	aet.resize(this->height);


	for (int i = 0; i < this->height; i++) {
		aet[i].maxx = INT_MIN;
		aet[i].minx = INT_MAX;
	}


	ScanLineBresenham(p0.x, p0.y, p1.x, p1.y, aet);
	ScanLineBresenham(p0.x, p0.y, p2.x, p2.y, aet);
	ScanLineBresenham(p1.x, p1.y, p2.x, p2.y, aet);


	for (int i = 0; i < this->height; i++) {
		for (int j = aet[i].minx; j <= aet[i].maxx; j++) {

			Vector2 v0 = Vector2(p1.x - p0.x, p1.y - p0.y);
			Vector2 v1 = Vector2(p2.x - p0.x, p2.y - p0.y);
			Vector2 v2 = Vector2(j - p0.x, i - p0.y);

			float d00 = v0.Dot(v0);
			float d01 = v0.Dot(v1);
			float d11 = v1.Dot(v1);
			float d20 = v2.Dot(v0);
			float d21 = v2.Dot(v1);
			float denom = d00 * d11 - d01 * d01;
			float v = (d11 * d20 - d01 * d21) / denom;
			float w = (d00 * d21 - d01 * d20) / denom;
			float u = 1.0 - v - w;

			// Check that weights are in range [0..1] and add up to 1
			if (v < 0 || w < 0 || u < 0 || v+w+u > 1) {
				continue;
			}

			// Compute final color for current pixel using barycentric interpolation
			Color finalColor = Color(c0.r * u + c1.r * v + c2.r * w, c0.g * u + c1.g * v + c2.g * w, c0.b * u + c1.b * v + c2.b * w);

			// Set the pixel color
			SetPixelSafe(j, i, finalColor);
		}
	}
}


void Image::ScanLineBresenham(int x0, int y0, int x1, int y1, std::vector<cell>& table) {
	// 4th and 5th octants
	if (x1 < x0) {
		int t = x1;
		x1 = x0;
		x0 = t;

		t = y1;
		y1 = y0;
		y0 = t;
	}

	int dx, dy, iE, iNE, d, x, y;

	dx = x1 - x0;
	dy = y1 - y0;

	// 8th octant
	if (y0 > y1)
		dy = -dy;

	iE = 2 * dy;
	iNE = 2 * (dy - dx);
	d = 2 * dy - dx;
	x = x0;
	y = y0;

	if (table[y].maxx < x) {
		table[y].maxx = x;
	}
	if (table[y].minx > x) {
		table[y].minx = x;
	}

	if (dx > dy) {

		while (x < x1) {

			if (d <= 0) {
				d += iE;
				x += 1;
			}
			else {
				d += iNE;
				x += 1;
				if (y0 > y1) // 8th octant
					y -= 1;
				else
					y += 1;
			}

			if (table[y].maxx < x) {
				table[y].maxx = x;
			}
			if (table[y].minx > x) {
				table[y].minx = x;
			}

		}
	}
	// 2nd and 6th octants
	else {

		iE = 2 * dx;
		iNE = 2 * (dx - dy);
		d = 2 * dx - dy;

		// 3rd and 7th octants
		if (y1 < y0) {
			int t = x1;
			x1 = x0;
			x0 = t;

			t = y1;
			y1 = y0;
			y0 = t;

			x = x0;
			y = y0;
		}

		while (y < y1) {

			if (d <= 0) {
				d += iE;
				y += 1;
			}
			else {
				d += iNE;
				y += 1;
				if (x0 > x1)
					x -= 1;
				else
					x += 1;
			}

			if (table[y].maxx < x) {
				table[y].maxx = x;
			}
			if (table[y].minx > x) {
				table[y].minx = x;
			}
		}
	}
}

void Image::DrawTriangleInterpolated_occlu(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Color& c0, const Color& c1, const Color& c2, FloatImage* zbuffer) {
	std::vector<cell> aet;
	aet.resize(this->height);
	float z_dist;


	for (int i = 0; i < this->height; i++) {
		aet[i].maxx = INT_MIN;
		aet[i].minx = INT_MAX;
	}


	ScanLineBresenham(p0.x, p0.y, p1.x, p1.y, aet);
	ScanLineBresenham(p0.x, p0.y, p2.x, p2.y, aet);
	ScanLineBresenham(p1.x, p1.y, p2.x, p2.y, aet);


	for (int i = 0; i < this->height; i++) {
		for (int j = aet[i].minx; j <= aet[i].maxx; j++) {

			Vector2 v0 = Vector2(p1.x - p0.x, p1.y - p0.y);
			Vector2 v1 = Vector2(p2.x - p0.x, p2.y - p0.y);
			Vector2 v2 = Vector2(j - p0.x, i - p0.y);

			float d00 = v0.Dot(v0);
			float d01 = v0.Dot(v1);
			float d11 = v1.Dot(v1);
			float d20 = v2.Dot(v0);
			float d21 = v2.Dot(v1);
			float denom = d00 * d11 - d01 * d01;
			float v = (d11 * d20 - d01 * d21) / denom;
			float w = (d00 * d21 - d01 * d20) / denom;
			float u = 1.0 - v - w;

			// Check that weights are in range [0..1] and add up to 1
			if (v < 0 || w < 0 || u < 0 || v+w+u > 1) {
				continue;
			}

			// Compute final color for current pixel using barycentric interpolation
			Color finalColor = Color(c0.r * u + c1.r * v + c2.r * w, c0.g * u + c1.g * v + c2.g * w, c0.b * u + c1.b * v + c2.b * w);

			z_dist = p0.z * u + p1.z * v + p2.z * w;

			// Set the pixel color
			SetPixelSafe(j, i, finalColor);
			if (z_dist <= zbuffer->GetPixel(j, i)) {
				SetPixelSafe(j, i, finalColor);
				zbuffer->SetPixel(j, i, z_dist);
			}
		}
	}
}

FloatImage::FloatImage(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new float[width * height];
	memset(pixels, 0, width * height * sizeof(float));
}

// Copy constructor
FloatImage::FloatImage(const FloatImage& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
}

// Assign operator
FloatImage& FloatImage::operator = (const FloatImage& c)
{
	if (pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height * sizeof(float)];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
	return *this;
}

FloatImage::~FloatImage()
{
	if (pixels)
		delete pixels;
}

// Change image size (the old one will remain in the top-left corner)
void FloatImage::Resize(unsigned int width, unsigned int height)
{
	float* new_pixels = new float[width * height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for (unsigned int x = 0; x < min_width; ++x)
		for (unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * width + x] = GetPixel(x, y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}