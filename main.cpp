#define _USE_MATH_DEFINES

#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <list>
#include <cmath>
#include <ctime>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include "circle.h"
#include "tinyxml/tinystr.h"
#include "tinyxml/tinyxml.h"

using namespace std;

list<Circle> circleList;
Circle *player;

bool jumping = false;
Circle *onCircle;
float start;

int keyFlags[256];

void keyboardUp(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'W':
        case 'w':
            keyFlags['w'] = 0;
            break;

        case 'S':
        case 's':
            keyFlags['s'] = 0;
            break;

        case 'A':
        case 'a':
            keyFlags['a'] = 0;
            break;

        case 'D':
        case 'd':
            keyFlags['d'] = 0;
			break;

		case 'P':
		case 'p':
			keyFlags['p'] = 0;
			break;

        default:
            break;
    }
}

void keyPressed(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'W':
        case 'w':
            keyFlags['w'] = 1;
            break;

        case 'S':
        case 's':
            keyFlags['s'] = 1;
            break;

        case 'A':
        case 'a':
            keyFlags['a'] = 1;
            break;

        case 'D':
        case 'd':
            keyFlags['d'] = 1;
			break;
		
		case 'P':
		case 'p':
			keyFlags['p'] = 1;
			break;

        default:
            break;
    }
}

void idle()
{
	float delta = 1;

	if (onCircle != NULL)
	{
		float dx = player->getcx() - onCircle->getcx();
		float dy = player->getcy() - onCircle->getcy();

		float d = sqrt(dx*dx + dy*dy);

		if (d >= player->getRadius() + onCircle->getRadius())
		{
			onCircle = NULL;
		}
	}

    if (keyFlags['w'] == 1)
    {
		player->moveY(-delta);
    }
    if (keyFlags['s'] == 1)
    {
		player->moveY(delta);
    }
    if (keyFlags['a'] == 1)
    {
		player->moveX(-delta);
    }
    if (keyFlags['d'] == 1)
    {
		player->moveX(delta);
	}
	if (keyFlags['p'] == 1)
	{
		if (!jumping)
		{
			jumping = true;
			start = glutGet(GLUT_ELAPSED_TIME);
		}
	}

	// Jump
	// How much should the radius change per ms
	float deltaR = (player->getRadius()/20)*0.165;
	float leastDistance = 1000;

	if (jumping)
	{	
		if ( (glutGet(GLUT_ELAPSED_TIME) - start) <= 1000 )
		{
			player->increasegr(deltaR);
		}
		else if ( (glutGet(GLUT_ELAPSED_TIME) - start) <= 2000 )
		{
			player->increasegr(-deltaR);
		}
		else
		{
			player->setgr(0);
			jumping = false;

			for (Circle &it : circleList)
			{
				if (it.getFill() != "green")
				{
					float distanceX = player->getcx() - it.getcx();
					float distanceY = player->getcy() - it.getcy();
		
					float distance = sqrt(distanceX*distanceX + distanceY*distanceY);

					if (distance < leastDistance)
					{
						leastDistance = distance;
					}
	
					if ( ( distance < ( player->getRadius() + it.getRadius() ) ) && distance == leastDistance )
					{
						onCircle = &it;
					}
				}
			}
		}
	}

	// Collision
	float distanceX, distanceY, distance;
	
	for (Circle &it : circleList)
	{
		float distanceX = player->getcx() - it.getcx();
		float distanceY = player->getcy() - it.getcy();

		float distance = sqrt(distanceX*distanceX + distanceY*distanceY);

		bool touching = distance < (player->getRadius() + it.getRadius());

		if ( it.getFill() == "black" )
		{
			if ( onCircle != NULL && it.getId() != onCircle->getId() && touching && !jumping )
			{
				player->handleCollision(keyFlags['w'], keyFlags['a'], keyFlags['s'], keyFlags['d']);
			}
			else if ( onCircle == NULL && touching && !jumping )
			{
				player->handleCollision(keyFlags['w'], keyFlags['a'], keyFlags['s'], keyFlags['d']);
			}
		}
		else if ( it.getFill() == "red" || it.getFill() == "white" )
		{
			if (touching)
			{
				player->handleCollision(keyFlags['w'], keyFlags['a'], keyFlags['s'], keyFlags['d']);
			}
		}
		else if ( it.getFill() == "blue" )
		{
			float dpx = player->getcx();
			float dpy = player->getcy();

			// Distance from player to circle border
			distanceX = dpx - it.getcx();
			distanceY = dpy - it.getcy();

			distance = sqrt(distanceX*distanceX + distanceY*distanceY);
			distance += player->getRadius();

			if (distance > it.getRadius())
			{
				player->handleCollision(keyFlags['w'], keyFlags['a'], keyFlags['s'], keyFlags['d']);
			}
		}
	}

    glutPostRedisplay();
}

void display()
{
	// Limpar todos os pixels
	glClear(GL_COLOR_BUFFER_BIT);

	for (Circle &it : circleList)
	{
		glBegin(GL_TRIANGLE_FAN);
			it.drawCircle();
		glEnd();
	}

	glutSwapBuffers();
	glFlush();
}

float returnWindowSize()
{
	float radius, diameter;
	
	// Finding the blue circle
	for (Circle &it : circleList)
	{
		if (it.getFill() == "blue")
		{
			radius = it.getRadius();
		}
	}
	
	diameter = radius * 2;

	return diameter;
}

void init()
{
	// Selecionar cor de fundo
	glClearColor(1, 1, 1, 1.0);

	// Inicializar sistema de viz
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float windowSize = returnWindowSize();

	int cx, cy, radius;

	for (Circle &it : circleList)
	{
		if (it.getFill() == "blue")
		{
			cx = it.getcx();
			cy = it.getcy();
			radius = it.getRadius();
		}
	}

	glOrtho(cx - radius, cx + radius, cy + radius, cy - radius, -1.0, 1.0);
}

/* Parsing do svg */
void loadArenaScenario(string path)
{
	TiXmlDocument doc(path.c_str());
	bool loaded = doc.LoadFile();
	if (loaded)
	{
		// Le o arquivo svg e cria as instancias dos circulos
		TiXmlElement* objectTree = doc.FirstChildElement("svg")->FirstChildElement("circle");
		for(TiXmlElement* e = objectTree; e != NULL; e = e->NextSiblingElement())
		{
			Circle novo(atof(e->Attribute("cx")), atof(e->Attribute("cy")),
						atof(e->Attribute("r")),
						e->Attribute("fill"),
						atoi(e->Attribute("id")));

			circleList.push_back(novo);
		}

		// Move circulo verde para o final da lista
		// Para ser desenhado sempre por último
		std::list<Circle>::iterator it;
		for (it = circleList.begin(); it != circleList.end(); ++it)
		{
			if (it->getFill() == "green")
			{
				circleList.splice(circleList.end(), circleList, it);
				break;
			}
		}

		// Guarda ponteiro para o circulo verde
		for (Circle &it : circleList)
		{
			if (it.getFill() == "green")
			{
				player = &it;
			}
		}
	}
	else
	{
		printf("Could not find or load file\n");
		exit(0);
	}

	return;
}
/* */

/* Parsing do xml */
void parsing(const char* loc)
{
	TiXmlDocument doc(loc);
	bool loadOkay = doc.LoadFile();

	if (!loadOkay)
	{
        printf("Could not find or load file\n");
        exit(0);
  	}

	TiXmlHandle xmlHandler(&doc);
	TiXmlElement *aplicacao, *arquivoDaArena;

	aplicacao = doc.FirstChildElement("aplicacao");
    arquivoDaArena = aplicacao->FirstChildElement("arquivoDaArena");
    
    // nome do arquivo
    const char* nome = arquivoDaArena->Attribute("nome");
    // extensao do arquivo
    const char* tipo = arquivoDaArena->Attribute("tipo");
    // caminho do arquivo
    const char* caminho = arquivoDaArena->Attribute("caminho");

	string n = nome;
	string c = caminho;
	string t = tipo;
	string path = c + n + "." + t;
	
	loadArenaScenario(path);
}
/* */

int main(int argc, char** argv)
{
    /* Parsing do xml */
    if (argc != 2)
    {
        printf("Modelo: %s local_do_arquivo\n", argv[0]);
        return 1;
    }

	string s = argv[1];
	string filename = "config.xml";

	string path = s + filename;

	const char* localizacao = path.c_str();
	parsing(localizacao);
	/* */

	float windowSize = returnWindowSize();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(windowSize, windowSize);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("arena");
	init();

	// funcoes de callback
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyboardUp);
    glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}
