/*Criador: Lucas Godoi
* Data: 30/10/2020
* 2º Período do Curso de Ciência da Computação
* Disciplina de Modelagem e Construção de Aplicações 3D
*/
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>

int chuta=0, chutou=0, gol=0;
float power=-12.5;
float bolaX=0, bolaY=-3.5, bolaZ=-8;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

static void display(void);
static void resize(int width, int height);
static void key(unsigned char key, int x, int y);
static void idle(void);
void funcao_bola();
void funcao_bola_menos();
void powerBar();
void restart();
void confereGol();
void fezGol();
void naoFezGol();

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(500,100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Penalti | Desenvolvido por Lucas Godoi");

    printf("A - mover para a esquerda\nD - mover para a direita\nR - para comecar um novo jogo!\n");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(0,0.6,0,0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void key(unsigned char key, int x, int y)
{
    switch (tolower(key))
    {
        case 27:
            exit(0);
            break;
        case 'a':
            if(chutou!=1&&bolaX>-3.9)
                bolaX-=0.1;
            break;
        case 'd':
            if(chutou!=1&&bolaX<=3.8)
            bolaX+=0.1;
            break;
        case 'r':
            restart();
            break;
        case 32:
        case 'w':
            if(chutou==0)
            {
                if(chuta>=10)
                {
                    chuta-=1;
                    while(chuta!=0)
                    {
                        funcao_bola_menos();
                        chuta-=1;
                    }
                }
                else if(chuta<=9&&chuta>=0)
                {
                    chuta+=1;
                    funcao_bola();
                }

                if(chuta==0)
                {
                    bolaY=-2.5;
                    bolaZ=-6;
                }
            }
            chutou=1;
            printf("\nFORCA: %d\n",chuta);
            break;
            case 's':
        default:
            break;
    }
    confereGol();
    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(0.9,1,1);

    //BOLA
    glPushMatrix();
        glTranslated(bolaX,bolaY,bolaZ);
        glRotated(60,1,0,0);
        glScalef(0.3, 0.3, 0.3);
        glutSolidSphere(1,60,60);
    glPopMatrix();

    //GOL FÍSICO
    glPushMatrix();
        glTranslated(-4.0,1.5,-15);
        glScalef(0.05, 1.0, 0.1);
        glutSolidCube(4);
    glPopMatrix();

    glPushMatrix();
        glTranslated(4.0,1.5,-15);
        glScalef(0.05, 1.0, 0.1);
        glutSolidCube(4);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0, 3.5,-15);
        glScalef(2.05, 0.05, 0.1);
        glutSolidCube(4);
    glPopMatrix();

    //LINHA GOL
    glPushMatrix();
        glColor3f(1,1,1);
        glTranslated(0, -0.5,-16);
        glScalef(10, 0.05, 0.1);
        glutSolidCube(4);
    glPopMatrix();

    //GRAMADO
    glPushMatrix();
        glColor3f(0,0.8,0);
        glTranslated(0, -2.5,-15);
        glScalef(10, 0.3, 0.1);
        glutSolidCube(4);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0,0.8,0);
        glTranslated(0, -5.5,-15);
        glScalef(10, 0.3, 0.1);
        glutSolidCube(4);
    glPopMatrix();

    if(gol==1)
        fezGol();
    else if(chutou==1&&gol==0)
        naoFezGol();

    powerBar();

    glutSwapBuffers();
}

void funcao_bola()
{
    int i;

    for(i=0; i<(chuta*3.5); i++)
    {
        //bolaX = bolaX - 0.1;
        bolaY = bolaY + 0.4;
        bolaZ = bolaZ - 0.7;
    }
    //printf("bolaX: %f\nbolaY: %f\nbolaZ: %f\n\n",bolaX,bolaY,bolaZ);
}
void funcao_bola_menos()
{
    int i;

    for(i=0; i<(chuta*3.5); i++)
    {
        //bolaX = bolaX + 0.1;
        bolaY = bolaY - 0.3;
        bolaZ = bolaZ + 0.5;
    }
}

void powerBar()
{
    glPushMatrix();
        glColor3f(0,0,1);
        glTranslated(-29.5,6.5,-15);
        glScalef(10, 0.05, 0.1);
        glutSolidCube(4);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0,0,1);
        glTranslated(power,6.5,-15);
        glScalef(1.5, 0.05, 0.1);
        glutSolidCube(4);
    glPopMatrix();

    if(chutou==0)
    {
        power+=0.5;
        chuta+=1;
        if(power>=-7.5&&power>=-12.5)
        {
            while(power!=-12.5)
            {
                power-=0.5;
            }
        }
        if(power==-12.5)
        {
            chuta=0;
        }
    }
}

void restart()
{
    chuta=0;
    chutou=0;
    gol=0;
    bolaX=0;
    bolaY=-3.5;
    bolaZ=-8;
    power=-12.5;
}

void confereGol()
{
    if(chutou==1)
    {
        if(chuta>=3 && chuta<=6)
        {
            if(bolaX<=5.5 && bolaX>=-5.5)
            {
                if(bolaY>=-0.1 && bolaY<=5)
                {
                    printf("GOOOOOOOLLL!!\n");
                    gol=1;
                }
                else printf("NAO FOI GOL!!\n");
            }
            else printf("NAO FOI GOL!!\n");
        }
        else printf("NAO FOI GOL!!\n");

        //printf("A - mover para a esquerda\nD - mover para a direita\nR - para comecar um novo jogo!\n");
    }
}
void fezGol()
{
    //G
    glPushMatrix();
        glColor3f(1,1,0);
        glTranslated(-4.8, -1.5,-10);
        glScalef(0.3, 2, 0.1);
        glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1,1,0);
        glTranslated(-3.6, -3.5,-10);
        glScalef(1.5, 0.3, 0.1);
        glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1,1,0);
        glTranslated(-3.6, 0.2,-10);
        glScalef(1.5, 0.3, 0.1);
        glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1,1,0);
        glTranslated(-2.4, -2.8,-10);
        glScalef(0.3, 1, 0.1);
        glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1,1,0);
        glTranslated(-2.7, -2,-10);
        glScalef(0.6, 0.3, 0.1);
        glutSolidCube(2);
    glPopMatrix();

    //O
    glPushMatrix();
        glColor3f(1,1,0);
        glTranslated(-0.8, -1.7,-10);
        glScalef(0.3, 2.16, 0.1);
        glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1,1,0);
        glTranslated(0.4, -3.57,-10);
        glScalef(1.5, 0.3, 0.1);
        glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1,1,0);
        glTranslated(1.6, -1.7,-10);
        glScalef(0.3, 2.16, 0.1);
        glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1,1,0);
        glTranslated(0.4, 0.2,-10);
        glScalef(1.5, 0.3, 0.1);
        glutSolidCube(2);
    glPopMatrix();

    //L
    glPushMatrix();
        glColor3f(1,1,0);
        glTranslated(3.2, -1.7,-10);
        glScalef(0.3, 2.16, 0.1);
        glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1,1,0);
        glTranslated(4, -3.56,-10);
        glScalef(1, 0.3, 0.1);
        glutSolidCube(2);
    glPopMatrix();
}

void naoFezGol()
{

    glPushMatrix();
        glColor3f(1,0,0);
        glTranslated(-1, 0.5,-10);
        glScalef(0.3, 1.5, 0.1);
        glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1,0,0);
        glTranslated(1, 0.5,-10);
        glScalef(0.3, 1.5, 0.1);
        glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1,0,0);
        glTranslated(1, -1.5,-10);
        glScalef(0.3, 0.3, 0.3);
        glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
        glColor3f(1,0,0);
        glTranslated(-1, -1.5,-10);
        glScalef(0.3, 0.3, 0.3);
        glutSolidCube(2);
    glPopMatrix();
}
