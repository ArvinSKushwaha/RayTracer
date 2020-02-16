#include <any>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <random>
#include <thread>
#include <eigen3/Eigen/Core>
#include "raytrace/misc_func.hpp"
#include "raytrace/objects.hpp"

#define Vec Eigen::Vector3d
#define RAYS_PER_PIXEL 100

const int bytesPerPixel = 3; /// red, green, blue
const int fileHeaderSize = 14;
const int infoHeaderSize = 40;

void generateBitmapImage(unsigned char *image, int height, int width, char *imageFileName);
unsigned char *createBitmapFileHeader(int height, int width, int paddingSize);
unsigned char *createBitmapInfoHeader(int height, int width);

void printArray(std::array<double, 3> n)
{
    for (int i = 0; i < 3; i++)
    {
        std::cout << n[i] << " ";
    }
    std::cout << std::endl;
}

// For printing out a vector of Vec3D's
void printVectors(std::vector<Vec> n)
{
    for (int i = 0; i < n.size(); i++)
    {
        std::cout << n[i][0] << " " << n[i][1] << " " << n[i][2] << std::endl
                  << std::endl;
    }
}

Color colorPixel(int i, int j, std::vector<Sphere> scene, std::vector<PointLightSource> lights, Camera c, unsigned char *image)
{
    Color C;
    Color avg;
    for (int k = 0; k < RAYS_PER_PIXEL; k++)
    {
        Ray r = c.generateRays(i + ((double)rand() / RAND_MAX - 0.5), j + ((double)rand() / RAND_MAX - 0.5));
        C = trace(scene, lights, r, 0);
        avg.r += C.r / RAYS_PER_PIXEL;
        avg.g += C.g / RAYS_PER_PIXEL;
        avg.b += C.b / RAYS_PER_PIXEL;
    }
    return avg;
}

int main(int argc, char const *argv[])
{
    Camera c = Camera(Vec(0, 0, 0), Vec(0, 0, 1), 8000, 8000, M_PI_4, M_PI_4);
    PointLightSource l1 = PointLightSource(Vec(0, 0, 0), Color(255, 255, 255));
    std::vector<Sphere> scene = std::vector<Sphere>();
    for (int z = 0; z < 5; ++z)
    {
        for (int i = -3; i <= 3; ++i)
        {
            for (int j = -3; j <= 3; ++j)
            {
                scene.push_back(Sphere(Vec(i/2.5, j/2.5, z + 6), 0.2, Color(255, 0, 0)));
            }
        }
    }
    std::vector<PointLightSource> lights = {l1};

    unsigned char *image = (unsigned char *)calloc(sizeof(unsigned char *), c.height * c.width * bytesPerPixel);
    char *imageFileName = (char *)"bitmapImage.bmp";
    int n = 0;
    #pragma opt parallel
    {
        #pragma opt for schedule(static, 4000);
        for (int m = 0; m < c.width * c.height; ++m)
        {
            int i = m / c.width;
            int j = m - i * c.width;
            // std::flush(std::cout);
            // std::cout << ((float) c.width*i + j)/(c.height*c.width) * 100 << "%          " << "\r";
            Color avg = colorPixel(i, j, scene, lights, c, image);
            image[(i * c.width + j) * bytesPerPixel + 2] = (unsigned char)(avg.r); ///red
            image[(i * c.width + j) * bytesPerPixel + 1] = (unsigned char)(avg.g); ///green
            image[(i * c.width + j) * bytesPerPixel] = (unsigned char)(avg.b);     ///blue
            ++n;
            std::cout << (double)n / (c.height * c.width) * 100 << "%     \r";
            std::flush(std::cout);
        }
    }

    generateBitmapImage(image, c.height, c.width, imageFileName);
    printf("Image generated!!\n");
    return 0;
}

void generateBitmapImage(unsigned char *image, int height, int width, char *imageFileName)
{

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (width * bytesPerPixel) % 4) % 4;

    unsigned char *fileHeader = createBitmapFileHeader(height, width, paddingSize);
    unsigned char *infoHeader = createBitmapInfoHeader(height, width);

    FILE *imageFile = fopen(imageFileName, "wb");

    fwrite(fileHeader, 1, fileHeaderSize, imageFile);
    fwrite(infoHeader, 1, infoHeaderSize, imageFile);

    int i;
    for (i = 0; i < height; i++)
    {
        fwrite(image + (i * width * bytesPerPixel), bytesPerPixel, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char *createBitmapFileHeader(int height, int width, int paddingSize)
{
    int fileSize = fileHeaderSize + infoHeaderSize + (bytesPerPixel * width + paddingSize) * height;

    static unsigned char fileHeader[] = {
        0, 0,       /// signature
        0, 0, 0, 0, /// image file size in bytes
        0, 0, 0, 0, /// reserved
        0, 0, 0, 0, /// start of pixel array
    };

    fileHeader[0] = (unsigned char)('B');
    fileHeader[1] = (unsigned char)('M');
    fileHeader[2] = (unsigned char)(fileSize);
    fileHeader[3] = (unsigned char)(fileSize >> 8);
    fileHeader[4] = (unsigned char)(fileSize >> 16);
    fileHeader[5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);

    return fileHeader;
}

unsigned char *createBitmapInfoHeader(int height, int width)
{
    static unsigned char infoHeader[] = {
        0, 0, 0, 0, /// header size
        0, 0, 0, 0, /// image width
        0, 0, 0, 0, /// image height
        0, 0,       /// number of color planes
        0, 0,       /// bits per pixel
        0, 0, 0, 0, /// compression
        0, 0, 0, 0, /// image size
        0, 0, 0, 0, /// horizontal resolution
        0, 0, 0, 0, /// vertical resolution
        0, 0, 0, 0, /// colors in color table
        0, 0, 0, 0, /// important color count
    };

    infoHeader[0] = (unsigned char)(infoHeaderSize);
    infoHeader[4] = (unsigned char)(width);
    infoHeader[5] = (unsigned char)(width >> 8);
    infoHeader[6] = (unsigned char)(width >> 16);
    infoHeader[7] = (unsigned char)(width >> 24);
    infoHeader[8] = (unsigned char)(height);
    infoHeader[9] = (unsigned char)(height >> 8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(bytesPerPixel * 8);

    return infoHeader;
}