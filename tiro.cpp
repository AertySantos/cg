#include "tiro.h"
#include <math.h>
#define DISTANCIA_MAX 500

void Tiro::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
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

void Tiro::DesenhaTiro(GLfloat x, GLfloat y)
{
    //
    glPushMatrix();
     glTranslatef(x, y, 0);//Translada
     DesenhaCirc(10.0f, 1, 1, 1);
    glPopMatrix();
}

void Tiro::Move()
{

    gX += gVel*cos(gDirectionAng);
    gY += gVel*sin(gDirectionAng);
}

bool Tiro::Valido()
{
    GLfloat dx = gX - gXInit;
    GLfloat dy = gY - gYInit;
    GLfloat distancia = sqrt(dx*dx + dy*dy);

    return distancia < DISTANCIA_MAX;
}
