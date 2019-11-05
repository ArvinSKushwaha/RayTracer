#include <any>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include "raytrace/misc_func.hpp"
#include "raytrace/objects.hpp"


const int bytesPerPixel = 3; /// red, green, blue
const int fileHeaderSize = 14;
const int infoHeaderSize = 40;

void generateBitmapImage(unsigned char *image, int height, int width, char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int width, int paddingSize);
unsigned char* createBitmapInfoHeader(int height, int width);

void printArray(std::array<double, 3> n)
{
    for(int i = 0; i < 3; i++){
        std::cout << n[i] << " ";
    }
    std::cout << std::endl;
}

// For printing out a vector of Vec3D's
void printVectors(std::vector<Vec3D> n)
{
    for(int i = 0; i < n.size(); i++){
        std::cout << n[i].cstr() << std::endl << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    Camera c = Camera(Vec3D(), Vec3D(0, 0, 1), 800, 800, M_PI_4, M_PI_4);
    Sphere s1 (Vec3D(0, 0, 50), 10, Color(10, 100, 255));
    Sphere s2 (Vec3D(10, 10, 30), 5, Color(255, 0, 255));
    Sphere s3 (Vec3D(-10, 10, 30), 7, Color(100, 255, 100));
    PointLightSource l1 = PointLightSource(Vec3D(10, -10, 40), Color(255, 255, 255));
    PointLightSource l2 = PointLightSource(Vec3D(10, 10, 40), Color(255, 255, 255));
    PointLightSource l3 = PointLightSource(Vec3D(0, 0, 0), Color(255, 255, 255));
    std::vector<Sphere> scene = {s1, s2, s3};
    std::vector<PointLightSource> lights = {l1};

    unsigned char image[c.height][c.width][bytesPerPixel];
    char* imageFileName = (char*) "bitmapImage.bmp";
    for(int i = 0; i < c.height; i++)
    {
        for(int j = 0; j < c.width; j++)
        {
            // std::flush(std::cout);
            // std::cout << ((float) c.width*i + j)/(c.height*c.width) * 100 << "%          " << "\r";
            std::vector<Color> C = std::vector<Color>();
            for(int k = 0; k < 10; k++)
            {
                Ray r = c.generateRays(i, j);
                C.push_back(diffuseTrace(scene, lights, r, 0));
            }
            Color avg = Color();
            for(int i = 0; i < C.size(); i++)
            {
                avg.r += C[i].r/C.size();
                avg.g += C[i].g/C.size();
                avg.b += C[i].b/C.size();
            }
            image[i][j][2] = (unsigned char)(avg.r); ///red
            image[i][j][1] = (unsigned char)(avg.g); ///green
            image[i][j][0] = (unsigned char)(avg.b); ///blue
        }
    }

    generateBitmapImage((unsigned char *)image, c.height, c.width, imageFileName);
    printf("Image generated!!\n");
    return 0;
}

void generateBitmapImage(unsigned char *image, int height, int width, char* imageFileName){

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (width*bytesPerPixel) % 4) % 4;

    unsigned char* fileHeader = createBitmapFileHeader(height, width, paddingSize);
    unsigned char* infoHeader = createBitmapInfoHeader(height, width);

    FILE* imageFile = fopen(imageFileName, "wb");

    fwrite(fileHeader, 1, fileHeaderSize, imageFile);
    fwrite(infoHeader, 1, infoHeaderSize, imageFile);

    int i;
    for(i=0; i<height; i++){
        fwrite(image+(i*width*bytesPerPixel), bytesPerPixel, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char* createBitmapFileHeader(int height, int width, int paddingSize){
    int fileSize = fileHeaderSize + infoHeaderSize + (bytesPerPixel*width+paddingSize) * height;

    static unsigned char fileHeader[] = {
        0,0, /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize    );
    fileHeader[ 3] = (unsigned char)(fileSize>> 8);
    fileHeader[ 4] = (unsigned char)(fileSize>>16);
    fileHeader[ 5] = (unsigned char)(fileSize>>24);
    fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader(int height, int width){
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0, /// number of color planes
        0,0, /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(infoHeaderSize);
    infoHeader[ 4] = (unsigned char)(width    );
    infoHeader[ 5] = (unsigned char)(width>> 8);
    infoHeader[ 6] = (unsigned char)(width>>16);
    infoHeader[ 7] = (unsigned char)(width>>24);
    infoHeader[ 8] = (unsigned char)(height    );
    infoHeader[ 9] = (unsigned char)(height>> 8);
    infoHeader[10] = (unsigned char)(height>>16);
    infoHeader[11] = (unsigned char)(height>>24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(bytesPerPixel*8);

    return infoHeader;
}