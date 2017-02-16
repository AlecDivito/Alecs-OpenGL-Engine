#include "MidPointTerrain.h"

MidPointTerrain::MidPointTerrain(int exponent)
{
    this->resolution = pow(2, exponent) + 1;
    this->last = this->resolution - 1;
    zeroHeightMap();
}

MidPointTerrain::~MidPointTerrain()
{
    heightMap.clear();
}

// zero all elements inside the height map
void MidPointTerrain::zeroHeightMap()
{
    for (unsigned int i = 0; i < this->resolution * this->resolution; ++i)
    {
        this->heightMap.push_back(0);
    }
}

// create random noise inside the height map
void MidPointTerrain::randomNoise()
{
    for (int i = 0; i < getSize(); ++i)
    {
        this->heightMap[i] = getRandom();
    }
}

// main function to create the height map
void MidPointTerrain::midPointDisplacement()
{
    // 1. Initialize the four corners of the height map to random values
    mpdInitCorners();
    // 2. Set the midpoints of each edge to the average of the two corners it’s between, plus or minus a random amount.
    mpdDisplace(0, this->last, 0, this->last, 0.1);
    float spread = 0.3;
    for (int i = 0; i < this->exponent; ++i)
    {
        int chunks = pow(2, i);
        int chunkWidth = (this->resolution - 1) / chunks;
        // this part needs more explination about whats happening
        for (int xchunk = 0; xchunk < chunks; ++xchunk)
        {
            for (int ychunk = 0; ychunk < chunks; ++ychunk)
            {
                int leftx = xchunk * chunkWidth;
                int rightx = leftx + chunkWidth;
                int bottomy = chunkWidth * ychunk;
                int topy = bottomy + chunkWidth;
                mpdDisplace(leftx, rightx, bottomy, topy, spread);
            }
        }
        spread *= 0.5;
    }
    // normalize the map to compress the values between 1.0 and 0.0
    normalize();
}

// get the size of the height map
int MidPointTerrain::getSize()
{
    return heightMap.size();
}

        /* PRIVATE FUNCITONS */

// Initialize the corners
void MidPointTerrain::mpdInitCorners()
{
    // Set the 4 corners of the map to random values between 0 and 1
    setHeightMapValue(0, 0, getRandom());
    setHeightMapValue(this->last, 0, getRandom());
    setHeightMapValue(this->last, this->last, getRandom());
    setHeightMapValue(0, this->last, getRandom());
}

// Set the center and the middle of rows and cols
void MidPointTerrain::mpdDisplace(int lx, int rx, int by, int ty, float spread)
{
    int cx = midPoint(lx, rx);
    int cy = midPoint(by, ty);

    float bottomLeft  = getHightMapValue(lx, by);
    float bottomRight = getHightMapValue(rx, by);
    float topLeft     = getHightMapValue(lx, ty);
    float topRight    = getHightMapValue(rx, ty);

    float top    = average2(topLeft, topRight);
    float left   = average2(bottomLeft, topLeft);
    float bottom = average2(bottomLeft, bottomRight);
    float right  = average2(bottomRight, topRight);
    float center = average4(top, left, bottom, right);

    setHeightMapValue(cx, by, jitter(bottom, spread));
    setHeightMapValue(cx, ty, jitter(top, spread));
    setHeightMapValue(lx, cy, jitter(left, spread));
    setHeightMapValue(rx, cy, jitter(right, spread));
    setHeightMapValue(cx, cy, jitter(center, spread));
}

// Compress the height map to make all values between 1.0 and 0.0
void MidPointTerrain::normalize()
{
    float max = 1.0f,min = 0.0f;
    for (int i = 0; i < getSize(); ++i)
    {
        float temp = this->heightMap[i];
        if(max < temp) {max = temp;}
        if(min > temp) {min = temp;}
    }
    float span = max - min;
    for (int i = 0; i < getSize(); ++i)
    {
        float temp = this->heightMap[i];
        this->heightMap[i] = (temp - min) / span;
    }
}

// get a value on the 1D height map
float MidPointTerrain::getHightMapValue(int x,int y)
{
    if (x < 0 || x > this->last || y < 0 || y > this->last) return -1;
    return this->heightMap[x + this->resolution * y];
};

// set a value on the 1D height map
void MidPointTerrain::setHeightMapValue(int x,int y,float val)
{
    this->heightMap[x + this->resolution * y] = val;
};

// general helper functions
// get random number between 1.0 and 0.0
float MidPointTerrain::getRandom()
{
    return (float)(rand() % 1000) / 1000;
}

// return a random number
float MidPointTerrain::jitter(float value, float spread)
{
    return (getRandom() + spread) + value;
}

// return the index of the mid point
int MidPointTerrain::midPoint(int a, int b)
{
    return (a + b) / 2;
}
// average between 2 and 4
float MidPointTerrain::average2(float a, float b)
{
    return (a + b) / 2;
}
float MidPointTerrain::average4(float a, float b, float c, float d)
{
    return (a + b + c + d) / 4;
}
