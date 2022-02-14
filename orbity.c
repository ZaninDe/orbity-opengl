#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

typedef struct BMP
{
    char *data;
    int   width;
    int   height;
}IMG;

#define TEXTURE_LIMIT 4

GLuint texture_id[TEXTURE_LIMIT];

char* textureArray[TEXTURE_LIMIT] = {
        "terra.bmp", "texture1.bmp", "texture2.bmp", "texture3.bmp",
};

GLUquadricObj *obj;

double s1 = 5.5;
double s2 = 2.5;

double rt = 0.25;
double Rt = 0.75;

float cameraY=0.7;
int stop = 0;

GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;
float angle = 10.0;

double phi = 0.0;
double theta = 0.0;
double r = 0.5;
int n = 450;
int index_texture = 0;



void getBitmapImageData( char *pFileName, IMG *pImage )
{
    FILE *pFile = NULL;
    unsigned short nNumPlanes;
    unsigned short nNumBPP;
    int i;

    if( (pFile = fopen(pFileName, "rb") ) == NULL )
        printf("ERROR: getBitmapImageData - %s not found.\n", pFileName);

    fseek( pFile, 18, SEEK_CUR );

    
    if( (i = fread(&pImage->height, 4, 1, pFile) ) != 1 )
        printf("ERROR");

    if( (i = fread(&pImage->width, 4, 1, pFile) ) != 1 )
        printf("ERROR");


    if( (fread(&nNumPlanes, 2, 1, pFile) ) != 1 )
        printf("ERROR");
    
    if( (i = fread(&nNumBPP, 2, 1, pFile)) != 1 )
        printf("ERROR");

    if( nNumPlanes != 1 )
       printf("ERROR");


    if( nNumBPP != 24 )
        printf("ERROR");

    fseek( pFile, 24, SEEK_CUR );

    int sizeImg = (pImage->width * pImage->height) * 3;

    pImage->data = (char*) malloc( sizeImg );

    if( (i = fread(pImage->data, sizeImg, 1, pFile) ) != 1 )
        printf("ERROR");

    char aux;
    for( i = 0; i < sizeImg; i += 3 )
    {
        aux = pImage->data[i];
        pImage->data[i] = pImage->data[i+2];
        pImage->data[i+2] = aux;
    }
}

void light(){
    GLfloat pos[] = {2.0f, 2.0f, 2.0f, 1.0f};
    GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat cyan[] = {0.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0,GL_POSITION,pos);
    glLightfv(GL_LIGHT0,GL_AMBIENT,black);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,cyan);
    glLightfv(GL_LIGHT0,GL_SPECULAR,white);

    GLfloat light1_pos[] = {-2.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT1,GL_POSITION,light1_pos);
    glLightfv(GL_LIGHT1,GL_DIFFUSE, cyan);
    glLightfv(GL_LIGHT1,GL_SPECULAR, white);

    GLfloat dir[] = {1.0f, 0.0f, 0.0f};
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,dir);
    glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,45.0f);          
    glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,0.3f);             

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void textureLoader()
{
    IMG texture;

    glGenTextures(TEXTURE_LIMIT, texture_id);
                                
    int i;
    for ( i=0; i<TEXTURE_LIMIT; i++ ) {
        getBitmapImageData( textureArray[i], &texture);
        glBindTexture(GL_TEXTURE_2D, texture_id[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }
}

void textureStarter (void)
{

    glEnable ( GL_TEXTURE_2D );
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    textureLoader();

}



void init(void)
{
    glEnable ( GL_COLOR_MATERIAL );
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 3.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.5, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 10.0);
    glViewport(0, 0, 1000, 1000);

    light();
}

void quadratic(){
    int i,k;
    glClear(GL_COLOR_BUFFER_BIT);

    for(i=0;i<n;i++){

        glBegin(GL_POINTS);
            for(k=0;k<n;k++){
                glVertex3f(r*cos(phi)*cos(theta),r*cos(phi)*sin(theta),r*sin(phi));
                phi = phi + (2*M_PI)/n;
            }

        glEnd();
        phi = 0.0;
        theta = theta+(2*M_PI)/n;
    }
}

void drawBalls(){
     obj = gluNewQuadric();
     gluQuadricTexture(obj,GL_TRUE);


     gluSphere(obj,0.3,65,65);
}



void centralBall(){
    glPushMatrix();
    GLfloat diffuse[5];
    GLfloat specular[5];
    GLfloat sec;

    specular[0] = 1;
    specular[1] = 1;
    specular[2] = 1;
    specular[3] = 1.0f;

    diffuse[0] = 1.0;
    diffuse[1] = 1.0;
    diffuse[2] = 1.0;
    diffuse[3] = 1.0f;

    sec = 30.0f;

    glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, sec);
    glBindTexture(GL_TEXTURE_2D, texture_id[0]);
        glRotatef(angle,0.0,1.0,1.0);
        quadratic();
    glPopMatrix();
    
}

void draw(){
    float vel01 = 1.9, vel02 = 1.0;

        
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texture_id[1]);
        glRotatef(-angle * vel01,0.0,1.0,0.0);
        glTranslatef(-0.80,0.0,0.8);
        drawBalls();
    glPopMatrix();

    
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texture_id[3]);
        glRotatef(angle * vel02,0.0,1.0,0.0);
        glTranslatef(0.0,0.0,2.0);
        drawBalls();
    glPopMatrix();
}

void display() {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);

        centralBall();
        draw();

        glFlush();
}

void planetRotation(int key, int x, int y){
     switch (key){
        case GLUT_KEY_DOWN :
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0.0, -7.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(50.0, 1.5, 2.5, 10.0);
           break ;

        case GLUT_KEY_UP :
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            gluLookAt(0.0, 3.0, 9.0, 0.0, 0.0, 0.0,  0.0, 1.0, 0.0);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(50.0, 1.5, 2.5, 10.0);
           break ;

        case GLUT_KEY_LEFT :
           angle-=30;
           break ;

        case GLUT_KEY_RIGHT :
           angle+=30;
           break ;
        default:
           break;
     }
     glutPostRedisplay() ;
}

void animationExecute(){
    if(stop == 0){
        angle+=10;
        glutTimerFunc(80, animationExecute, 1);
        glutPostRedisplay();
    }
}

void mouse(int btn, int state, int x, int y)
{
    switch (btn)
    {
    case GLUT_RIGHT_BUTTON:
        stop = 1;
        break;

    case GLUT_LEFT_BUTTON:
        stop = 0;
                glutTimerFunc(80, animationExecute, 1);
        glutPostRedisplay();
        break;

    default:
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char *argv[]){
    glutInit(&argc,argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_SINGLE);

    glutInitWindowSize(1000,800);
    glutCreateWindow("Orbity");

    glutDisplayFunc(display);

    glutTimerFunc(5, animationExecute, 1);

    glutSpecialFunc(planetRotation);

    
    glutMouseFunc(mouse);

    init();
    textureStarter();

    glutMainLoop();

    return 0;
}