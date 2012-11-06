#include <GL/glut.h>
#include <iostream>

using namespace std;

float* ViewPosTo3D(float *p);
void Desenha(void);
void AlteraTamanhoJanela(GLsizei w, GLsizei h);
void EspecificaParametrosVisualizacao(void);
void GerenciaMouse(int button, int state, int x, int y);
void GerenciaMouseMove(int x, int y);
void Inicializa(void);

GLfloat angle, fAspect;
bool tracking_mouse = false;
float velocity = 0.0;
int last_time = 0;
int win_width, win_height;


float* ViewPosTo3D(float *p)
{
    float px, py;
    px = p[0];
    py = p[1];

    float point3d[3];

    point3d[0] = 2.0 * px / win_width - 1.0;
    point3d[1] = 1.0 - 2.0 * py / win_height;
    point3d[2] = 0.0;
 
    return point3d;
}

void Desenha(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 1.0f);
    glutWireTeapot(50.0f);
    glutSwapBuffers();
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    if (h == 0)
        h = 1;
    
    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;
    
    win_width = w;
    win_height = h;

    EspecificaParametrosVisualizacao();
}

void EspecificaParametrosVisualizacao(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle, fAspect, 0.1, 500);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 80, 200, 0, 0, 0, 0, 1, 0);
}

void GerenciaMouseMove(int x, int y)
{
    float b[2];
    b[0] = x;
    b[1] = y;

    float *r;
    r = ViewPosTo3D(b);
    cout << r[0] << " " << r[1] <<  " " << r[2] << endl;
}

void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            /*cout << x << "  "<< y << endl;
            if (angle >= 10)
                angle -= 5;*/
            
            tracking_mouse = true;
            cout << "true" << endl;
        }

        if (state == GLUT_UP)
        {
            tracking_mouse = false;
            cout << "false" << endl;
        }

    }

    /*
    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_DOWN)
        {
            if (angle <= 130)
                angle += 5;
        }
    */
    

    //cout << x << "   " << y << endl;


    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}

void Inicializa(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    angle = 45;
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(350, 300);
    win_width = 350;
    win_height = 300;

    glutCreateWindow("Teapot");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMouseFunc(GerenciaMouse);
    glutMotionFunc(GerenciaMouseMove); 
    Inicializa();
    glutMainLoop();
    return 0;
}
