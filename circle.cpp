#include <GL/freeglut.h>
#include <cmath>
#include "circle.h"

void Circle::drawCircle()
{
    glColor3f(this->RGB[0], this->RGB[1], this->RGB[2]);

    int x, y;
	int vertices = 50;

	// Divide o circulo em 40 fatias (triangulos)
	for (int i = 0; i <= vertices; i++)
	{
		x = (this->radius + gr)*cosf(i*2.0f*M_PI / vertices) + this->cx;
        y = (this->radius + gr)*sinf(i*2.0f*M_PI / vertices) + this->cy;
        
        glVertex2f(x, y);
	}
}

void Circle::handleCollision(bool w, bool a, bool s, bool d)
{
    int q = 1;

    if (w)
    {
        this->cy += q;
    }
    if (a)
    {
        this->cx += q;
    }
    if (s)
    {
        this->cy -= q;
    }
    if (d)
    {
        this->cx -= q;
    }
}

void Circle::jump()
{
    this->jumping = true;
    int numberOfMS = 0;

    // How much should the radius change per ms
    float deltaR = (this->radius*1.5 - this->radius)/1000;
    float start = glutGet(GLUT_ELAPSED_TIME);

    // A cada 1ms
    if ((glutGet(GLUT_ELAPSED_TIME) - start) == 1 && numberOfMS <= 2000)
    {
        numberOfMS += 1;

        if (numberOfMS <= 1000)
        {
            cout << "Increasing. gr = " << gr << endl;
            gr += deltaR;
        }
        else if (numberOfMS > 1000 && numberOfMS <= 2000)
        {
            cout << "Decreasing. gr = " << gr << endl;
            gr -= deltaR;
        }
        else if (numberOfMS > 2000)
        {
            cout << "End of jump" << endl;
            gr = 0;
            this->jumping = false;
        }
    }

    return;
}

void Circle::moveX(GLfloat deltaX)
{
    this->cx += deltaX;
}

void Circle::moveY(GLfloat deltaY)
{
    this->cy += deltaY;
}

int Circle::getcx()
{
    return this->cx;
}

int Circle::getcy()
{
    return this->cy;
}

void Circle::increasegr(float value)
{
    this->gr += value;
}

int Circle::getId()
{
    return this->id;
}

void Circle::setgr(float value)
{
    this->gr = value;
}

float Circle::getgr()
{
    return this->gr;
}

float Circle::getRadius()
{
    return this->radius;
}

bool Circle::isJumping()
{
    return this->jumping;
}

string Circle::getFill()
{
    return this->fill;
}