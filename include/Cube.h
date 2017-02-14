#ifndef CUBE_H
#define CUBE_H


class Cube : public GameObject
{
    public:
        Cube();
        virtual ~Cube();

        // Draw sprite
        virtual void Draw() = 0;
        // collision
        virtual Glboolean CheckCollision(GameObject &obj) = 0;

    protected:
    private:
};

#endif // CUBE_H
