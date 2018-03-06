#ifndef CIRCLE_H
#define CIRCLE_H

#include <iostream>

#include <GL/freeglut.h>
#include <string>

using namespace std;

class Circle
{
    GLint cx;
    GLint cy;
    GLfloat gr = 0;
    bool jumping = false;
    GLfloat radius;
    string fill;
    GLint id;
    GLfloat RGB[3];

public:
    Circle(GLint cx, GLint cy, GLfloat radius, string fill, GLint id)
    {
        this->cx = cx;
        this->cy = cy;
        this->radius = radius;
        this->fill = fill;
        this->id = id;

        if (this->fill == "blue")
        {
            this->RGB[0] = 0; this->RGB[1] = .15; this->RGB[2] = 1;
        }
        else if (this->fill == "white")
        {
            this->RGB[0] = 1; this->RGB[1] = 1; this->RGB[2] = 1;
        }
        else if (this->fill == "black")
        {
            this->RGB[0] = 0; this->RGB[1] = 0; this->RGB[2] = 0;
        }
        else if (this->fill == "green")
        {
            this->RGB[0] = 0; this->RGB[1] = .5; this->RGB[2] = 0;
        }
        else if (this->fill == "red")
        {
            this->RGB[0] = 1; this->RGB[1] = 0; this->RGB[2] = 0;
        }
    }

    int getcx();
    int getcy();
    int getId();
    void setgr(float value);
    void increasegr(float value);
    bool isJumping();
    float getRadius();
    float getgr();
    string getFill();

    void handleCollision(bool w, bool a, bool s, bool d);
    void jump();

    void defineArenaBoundaries();

    void moveX(GLfloat deltaX);
    void moveY(GLfloat deltaY);

    void drawCircle();
};

#endif