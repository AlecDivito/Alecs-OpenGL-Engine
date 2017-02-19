#include "MidPointTerrain.h"

MidPointTerrain::MidPointTerrain(Shader shader, int exponent)
    : TerrainShader(shader), exponent(exponent), terrainVectorCount(0)
{
    srand(time(NULL));
    this->Resolution = pow(2, exponent) + 1;
    this->last = this->Resolution - 1;
    // allocate memory for the turrain array
    this->terrainMap = new GLfloat[this->last*this->last*18];
    // store vertices in a buffer to manage them on the graphics card
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    // Initilize the height map
    ZeroHeightMap();
}

MidPointTerrain::~MidPointTerrain()
{
    HeightMap.clear();
    delete[] this->terrainMap;
}

// zero all elements inside the height map
void MidPointTerrain::ZeroHeightMap()
{
    if(this->getSize() == 0)
    {
        for (unsigned int i = 0; i < this->Resolution * this->Resolution; ++i)
        {
            this->HeightMap.push_back(0);
        }
    }
    else
    {
        for(unsigned int i = 0; i < this->Resolution * this->Resolution; ++i)
        {
            this->HeightMap[i] = 0;
        }
    }
}

// create random noise inside the height map
void MidPointTerrain::RandomNoise()
{
    for (int i = 0; i < getSize(); ++i)
    {
        this->HeightMap[i] = getRandom();
    }
}

// main function to create the height map
void MidPointTerrain::MidPointDisplacement()
{
    // 1. Initialize the four corners of the height map to random values
    mpdInitCorners();
    // 2. Set the midpoints of each edge to the average of the two corners it’s between, plus or minus a random amount.
    // mpdDisplace(0, this->last, 0, this->last, 0.1);
    float spread = 0.3;
    for (int i = 0; i < this->exponent; ++i)
    {
        int chunks = pow(2, i);
        int chunkWidth = (this->Resolution - 1) / chunks;
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

// Init the height map to 3D and to be passed to openGL to be draw
void MidPointTerrain::InitTerrainMap()
{
    // clear and load the 3D terrain Map
    int counter = 0;
    float span = (float)this->Resolution;
    for(int x = 0; x < this->last; ++x)
    {
        int tempX = x + 1; // normalize the coordinates
        GLfloat xCoord_zero = 2 * ((float)x/span) - 1;// x = 0
        GLfloat xCoord_one =  2 * ((float)tempX/span) - 1;// x = 1
        for(int y = 0; y < this->last; ++y)
        {
            int tempY = y + 1;
            GLfloat yCoord_zero = 2 * ((float)y/span) - 1;// y = 0
            GLfloat yCoord_one = 2 * ((float)tempY/span) - 1;// y = 1
            // triangle 1 (0, 0, z)
            this->terrainMap[counter++] = (xCoord_zero);
            this->terrainMap[counter++] = (yCoord_zero);
            this->terrainMap[counter++] = ((GLfloat)this->getHightMapValue(x, y)); // z value
            // triangle 1 (0, 1, z)
            this->terrainMap[counter++] = (xCoord_zero);
            this->terrainMap[counter++] = (yCoord_one);
            this->terrainMap[counter++] = ((GLfloat)this->getHightMapValue(x, tempY)); // z value
            // triangle 1 (1,0, z)
            this->terrainMap[counter++] = (xCoord_one);
            this->terrainMap[counter++] = (yCoord_zero);
            this->terrainMap[counter++] = ((GLfloat)this->getHightMapValue(tempX, y)); // z value
            // triangle 2 (0, 1, z)
            this->terrainMap[counter++] = (xCoord_zero);
            this->terrainMap[counter++] = (yCoord_one);
            this->terrainMap[counter++] = ((GLfloat)this->getHightMapValue(x, tempY)); // z value
            // triangle 2 (1, 0, z)
            this->terrainMap[counter++] = (xCoord_one);
            this->terrainMap[counter++] = (yCoord_zero);
            this->terrainMap[counter++] = ((GLfloat)this->getHightMapValue(tempX, y)); // z value
            // triangle 2 (1, 1, z)
            this->terrainMap[counter++] = (xCoord_one);
            this->terrainMap[counter++] = (yCoord_one);
            this->terrainMap[counter++] = ((GLfloat)this->getHightMapValue(tempX, tempY)); // z value
            this->terrainVectorCount+=6;
        }
    }
    // 1. Bind vertex Array Object
    glBindVertexArray(this->VAO);
    // 2. copy our vertices array in a buffer for opengl to use
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->last*this->last*18, this->terrainMap, GL_DYNAMIC_DRAW);
    // We then tell how opengl should interprit the vertex data
    // (layout_Location, size of vertex attribute(vec3), dataType, GL_FALSE, length of stride, offset of where the position data begins in the buffer)
    // 3. Then we set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 4. We unBind the vertex array object
    glBindVertexArray(0);
}

void MidPointTerrain::Bind()
{
    glBindVertexArray(this->VAO);
}

void MidPointTerrain::Draw()
{
    glDrawArrays(GL_TRIANGLES, 0, this->terrainVectorCount);
}

// get the size of the height map
int MidPointTerrain::getSize()
{
    return this->HeightMap.size();
}

        /* PRIVATE FUNCITONS */

// Initialize the corners
void MidPointTerrain::mpdInitCorners()
{
    // Set the 4 corners of the map to random values between 0 and 1
/*    setHeightMapValue(0, 0, getRandom());
    setHeightMapValue(this->last, 0, getRandom());
    setHeightMapValue(this->last, this->last, getRandom());
    setHeightMapValue(0, this->last, getRandom());*/
    setHeightMapValue(0, 0, -1);
    setHeightMapValue(this->last, 0, -1);
    setHeightMapValue(this->last, this->last, -1);
    setHeightMapValue(0, this->last, -1);
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

// Compress the height map to make all values between 1.0 and -1.0
void MidPointTerrain::normalize()
{
    float max = 1.0f,min = 0.0f;
    for (int i = 0; i < getSize(); ++i)
    {
        float temp = this->HeightMap[i];
        if(max < temp) {max = temp;}
        if(min > temp) {min = temp;}
    }
    float span = max - min;
    for (int i = 0; i < getSize(); ++i)
    {
        float temp = this->HeightMap[i];
        this->HeightMap[i] = 2 * ((temp - min) / span) - 1;
    }
}

// get a value on the 1D height map
float MidPointTerrain::getHightMapValue(int x,int y)
{
    if (x < 0 || x > this->last || y < 0 || y > this->last) return -1;
    return this->HeightMap[x + this->Resolution * y];
};

// set a value on the 1D height map
void MidPointTerrain::setHeightMapValue(int x,int y,float val)
{
    this->HeightMap[x + this->Resolution * y] = val;
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
    return ((spread * getRandom() * 2) * spread) + value;
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
