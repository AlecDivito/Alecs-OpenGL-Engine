#ifndef MIDPOINTTERRAIN_H
#define MIDPOINTTERRAIN_H

#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>

// Create a 1D array filled with values
// between 1.0 and 0.0. Used to create
// height maps
class MidPointTerrain
{
    public:
        // Height map as a 1D array
        std::vector<float> heightMap;
        // Constructor
        midPointTerrain(int exponent);
        virtual ~midPointTerrain();
        // public methods to edit the height map
        void zeroHeightMap(); // zero all elements inside the height map
        void randomNoise(); // create random noise inside the height map
        void midPointDisplacement();
        int getSize(); // get the size of the height map

    protected:
    private:
        // helper members
        unsigned int resolution;
        int exponent; // self explaiontory
        int last;     // last elements index
        // helper functions to create the height map
        void mpdInitCorners();
        void mpdDisplace(int lx, int rx, int by, int ty, float spread);
        void normalize();
        float getHightMapValue(int x,int y);
        void setHeightMapValue(int x,int y,float val);
        // general helper functions
        float getRandom(); // get random number between 1.0 and 0.0
        float jitter(float value, float spread);    //
        int midPoint(int a, int b);
        // average between 2 and 4
        float average2(float a, float b);
        float average4(float a, float b, float c, float d);
};

#endif // MIDPOINTTERRAIN_H
