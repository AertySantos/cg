#include "robo.h"
#include <math.h>
#include <iostream>

void Robo::DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    glBegin(GL_QUADS);
        glVertex2f(-width, 0);//inferior esq
        glVertex2f(width, 0);//inferir dir
        glVertex2f(width, height);//superior dir
        glVertex2f(-width, height);//superior esq
    glEnd();
}


void Robo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    
    float raio = radius;
    glPointSize(2);
    glBegin(GL_POINTS);

        glVertex2f(0.0f, 0.0f); // centro

        for (int i = 0; i <= 360; i+= 20) {
            float angulo = i * 3.14159f / 180.0f;

            glVertex2f(
                raio * cos(angulo),
                raio * sin(angulo)
            );
        }

    glEnd();
    
}

void Robo::DesenhaRoda(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();
        glTranslatef(x, y, 0);//Translada
        glRotatef(thetaWheel, 0, 0, 1);
        DesenhaCirc(20.0f, R, G, B);
    glPopMatrix();
}

void Robo::DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{
    glPushMatrix();
    glTranslatef(0, y, 0);//Translada mudará o sistema de coordenada do mundo para a base do robô
    glRotatef(theta1, 0, 0, 1);
    DesenhaRect(paddleHeight, paddleWidth, 0, 0, 1);

    glTranslatef(0, paddleHeight, 0);//Translada 
    glRotatef(theta2, 0, 0, 1);
    DesenhaRect(paddleHeight, paddleWidth, 1, 1, 0);
    glTranslatef(0, paddleHeight, 0);//Translada 
    glRotatef(theta3, 0, 0, 1);
    
    DesenhaRect(paddleHeight, paddleWidth, 0, 1, 0);
    
    glPopMatrix();
}

void Robo::DesenhaRobo(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{
    
    glPushMatrix();
    glTranslatef(x, y, 0);//Translada mudará o sistema de coordenada do mundo para a base do robô
    DesenhaBraco(0, baseHeight, theta1, theta2, theta3);
    DesenhaRect(baseHeight, baseWidth, 1, 0, 0);
    DesenhaRoda(-baseWidth, 0, thetaWheel, 1, 1, 1);
    DesenhaRoda(baseWidth, 0, thetaWheel, 1, 1, 1);
    glPopMatrix();

}

void Robo::RodaBraco1(GLfloat inc)
{
    gTheta1=gTheta1+inc;
}

void Robo::RodaBraco2(GLfloat inc)
{
    gTheta2=gTheta2+inc;
}

void Robo::RodaBraco3(GLfloat inc)
{
    gTheta3=gTheta3+inc;
}

void Robo::MoveEmX(GLfloat dx)
{
    gX = gX + dx*500;
    gThetaWheel = gThetaWheel + (dx*500/20)*(180/3.14159);
}

//Funcao auxiliar de rotacao
void RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat &xOut, GLfloat &yOut){
    GLfloat rad = angle * 3.14159f / 180.0f;
    xOut = x * cos(rad) - y * sin(rad);
    yOut = x * sin(rad) + y * cos(rad);
}


// robo.cpp

struct Mat3 {
    GLfloat m[9]; // row-major
};

Mat3 Identidade()
{
    Mat3 r = {{1,0,0, 0,1,0, 0,0,1}};
    return r;
}

Mat3 MatTranslacao(GLfloat tx, GLfloat ty)
{
    Mat3 r = {{1,0,tx, 0,1,ty, 0,0,1}};
    return r;
}

Mat3 MatRotacao(GLfloat thetaGraus)
{
    GLfloat rad = thetaGraus * 3.14159f / 180.0f;
    GLfloat c = cos(rad), s = sin(rad);
    Mat3 r = {{c,-s,0, s,c,0, 0,0,1}};
    return r;
}

Mat3 MultMat(const Mat3 &A, const Mat3 &B) // retorna A * B
{
    Mat3 r;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            GLfloat soma = 0;
            for (int k = 0; k < 3; k++)
                soma += A.m[i*3+k] * B.m[k*3+j];
            r.m[i*3+j] = soma;
        }
    return r;
}



void Robo::GetPosTiro(GLfloat x, GLfloat y,
                       GLfloat theta1, GLfloat theta2, GLfloat theta3,
                       GLfloat &xPonta, GLfloat &yPonta, GLfloat &angulo)
{
    // Construindo a matriz "de trás pra frente":
    // sempre pré-multiplicando (nova matriz à esquerda da acumulada)
    Mat3 M = Identidade();

    M = MultMat(MatRotacao(theta3), M);                   // 1. rotação gTheta3
    M = MultMat(MatTranslacao(0, paddleHeight), M);       // 2. translação paddleHeight
    M = MultMat(MatRotacao(theta2), M);                   // 3. rotação gTheta2
    M = MultMat(MatTranslacao(0, paddleHeight), M);       // 4. translação paddleHeight
    M = MultMat(MatRotacao(theta1), M);                   // 5. rotação gTheta1
    M = MultMat(MatTranslacao(0, baseHeight), M);         // 6. translação baseHeight
    M = MultMat(MatTranslacao(x, y), M);                  // 7. translação (gX, gY)

    GLfloat xBase = M.m[2];
    GLfloat yBase = M.m[5];

    // PONTA: trata o ponto (0, paddleHeight) como uma translação a mais
    Mat3 Mponta = MultMat(M, MatTranslacao(0, paddleHeight));
    xPonta = Mponta.m[2];
    yPonta = Mponta.m[5];

    angulo = atan2(yPonta - yBase, xPonta - xBase);
    GLfloat radianos = 1.234f;
    GLfloat graus = radianos * (180.0f / 3.14159f);
    //angulo = graus;
    std::cout << "ponta: x=" << xPonta << " y=" << yPonta
               << " | angulo(grau)=" << angulo << std::endl;
}

Tiro* Robo::Atira()
{
    GLfloat xPonta, yPonta, angulo;
    GetPosTiro(gX, gY, gTheta1, gTheta2, gTheta3, xPonta, yPonta, angulo);

    Tiro* novoTiro = new Tiro(xPonta, yPonta, angulo);
    return novoTiro;
}