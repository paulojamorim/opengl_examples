#include <GL/glut.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>

using namespace std;

double* ViewPosTo3D(double *p);
vector<double> GetRotation(void);
vector<double> FromAxisAndAngle(vector<double> axis, double angle);
vector<double> normalized(vector<double> values);
double lengthSquared(double xp, double yp, double zp);

void Desenha(void);
void AlteraTamanhoJanela(GLsizei w, GLsizei h);
void EspecificaParametrosVisualizacao(void);
void GerenciaMouse(int button, int state, int x, int y);
void GerenciaMouseMove(int x, int y);
void Inicializa(void);

GLfloat angle, fAspect;
bool tracking_mouse = false;
double velocity = 0.0;
int win_width, win_height;

vector<double> rotation(4);
vector<double> axis(3);

time_t last_time = time(NULL);


// --------   Virtual Trackball -------------------------

double* ViewPosTo3D(double *p)
{
    double px, py;
    px = p[0];
    py = p[1];

    double point3d[3];

    point3d[0] = 2.0 * px / win_width - 1.0;
    point3d[1] = 1.0 - 2.0 * py / win_height;
    point3d[2] = 0.0;
 
    return point3d;
}

vector<double> GetRotation(void)
{
    double angle;
    if (tracking_mouse)
        return rotation;

    time_t current_time = time(NULL);
    angle = velocity * (current_time - last_time);
}

vector<double> FromAxisAndAngle(vector<double> axis, double angle)
{
    double a, s, c;
    double xp, yp, zp, len;
    vector<double> quaternion;
    vector<double> result;

    xp = axis[0];
    yp = axis[1];
    zp = axis[2];

    len = sqrt(xp * xp + yp * yp + zp * zp);

    if (!len)
    {
        xp /= len;
        yp /= len;
        zp /= len;
    }

    a = (angle / 2.0f) * M_PI / 180.0f;
    s = sin(a); 
    c = cos(a);

    result[0] = c;
    result[1] = xp * s;
    result[2] = yp * s;
    result[3] = zp * s;
    result[4] = 1;

    return normalized(result);

}


vector<double> normalized(vector<double> values)
{

    double len;
    double xp = values[0];
    double yp = values[1];
    double zp = values[2];

    len = lengthSquared(xp, yp, zp);

    if (not(len -1.0))
    {
        return values;
    }
    else if(len)
    {
        
        values[0] = values[0] / sqrt(len);
        values[1] = values[1] / sqrt(len);
        values[2] = values[2] / sqrt(len);
        return values;
    }
    else
    {
        values[0] = 0;
        values[1] = 0;
        values[2] = 0;
        return values;
    }
}


double lengthSquared(double xp, double yp, double zp)
{
    return xp * xp + yp * yp + zp * zp;
}


// ----------------------------------------------------

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
    double b[2];
    b[0] = x;
    b[1] = y;

    double *r;
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
    
    //Init the quaternion
    rotation[0] = 1;
    rotation[1] = 0;
    rotation[2] = 0;
    rotation[3] = 0;

    //Init axis
    axis[0] = 0;
    axis[1] = 0;
    axis[2] = 0;

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
