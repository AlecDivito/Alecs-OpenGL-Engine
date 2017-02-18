#ifndef MIDPOINTTERRAIN_H
#define MIDPOINTTERRAIN_H

#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <cstdio>

#include <GL/glew.h>
#include "Shader.h"

// Create a 1D array filled with values
// between 1.0 and 0.0. Used to create
// height maps
class MidPointTerrain
{
    public:
        // Height map as a 1D array
        std::vector<float> HeightMap;
        unsigned int Resolution;
        Shader TerrainShader;
        // Constructor
        MidPointTerrain(Shader shader, int exponent);
        virtual ~MidPointTerrain();
        // public methods to edit the height map
        void ZeroHeightMap(); // zero all elements inside the height map
        void RandomNoise(); // create random noise inside the height map
        void MidPointDisplacement();
        void InitTerrainMap();
        void Bind();
        void Draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model);

        int getSize(); // get the size of the height map
        void toString() // keeping toString for debuging
        {
            for (int i = 0; i < getSize(); i++)
            {
                printf("%f\n", this->HeightMap[i]);

            }
            printf("\n");
        };
          float getHightMapValue(int x,int y);
  protected:
    private:
        // Members to help draw the terrain
        GLuint VBO, VAO;
        GLfloat * terrainMap;
        // helper members
        int exponent; // self explaiontory
        int last;     // last elements index
        int terrainVectorCount; // count the number of vertices to draw
        // helper functions to create the height map
        void mpdInitCorners();
        void mpdDisplace(int lx, int rx, int by, int ty, float spread);
        void normalize();
//        float getHightMapValue(int x,int y);
        void setHeightMapValue(int x,int y,float val);
        // general helper functions
        float getRandom(); // get random number between 1.0 and 0.0
        float jitter(float value, float spread);    //
        int midPoint(int a, int b);
        float average2(float a, float b); // average between 2 and 4
        float average4(float a, float b, float c, float d);
};

#endif // MIDPOINTTERRAIN_H
