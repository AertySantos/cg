#include "alvo.h"
#include <math.h>
#include <iostream>

void Alvo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    
    float raio = radius;
    
    glBegin(GL_TRIANGLE_FAN);

        glVertex2f(0.0f, 0.0f); // centro

        for (int i = 0; i <= 360; i+= 1) {
            float angulo = i * 3.14159f / 180.0f;

            glVertex2f(
                raio * cos(angulo),
                raio * sin(angulo)
            );
        }

    glEnd();
}

void Alvo::DesenhaAlvo(GLfloat x, GLfloat y)
{
    glPushMatrix();
        glTranslatef(x, y, 0);//Translada
        if(gColor == 0)
            DesenhaCirc(radiusAlvo, 1, 0, 0);
        else if(gColor == 1)
            DesenhaCirc(radiusAlvo, 0, 1, 0);
        else
            DesenhaCirc(radiusAlvo, 0, 0, 1);
    glPopMatrix();
}

void Alvo::Recria(GLfloat x, GLfloat y)
{   
    gX = x;
    gY = y;

    //std::cout << "valor x=" << gX << "valor y ="<< gY<< std::endl;

    if(gColor == 0)
        gColor = 1;
    else if(gColor == 1)
        gColor = 2;
    else
        gColor = 0;
}

bool Alvo::Atingido(Tiro *tiro)
{
    GLfloat xTiro, yTiro;
    tiro->GetPos(xTiro, yTiro);
    GLfloat dx = xTiro - gX;
    GLfloat dy = yTiro - gY;
    GLfloat distancia = sqrt(dx*dx + dy*dy);

    return distancia < radiusAlvo;
}
