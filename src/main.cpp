/*
*   Title: N-Body Simulation
*   Author: Ajay Roopakalu
*   Description: A simple simulation of an iterated approximation of the n-body
*   problem implemented in C++ and OpenGL.
*
*   Some glaring approximations:
*       - The velocity of the point is modified...this isn't how a force works.
*       - Collision of rotating objects has been greatly fudged in the following
*           ways:    
*           1. Collision with any wall flips direction of rotation.
*           2. Collision with another object gives the object with the higher  
*               rotation the positive difference of their rotations and the 
*               one with the lower rotation the negative difference of the 
*               rotations.
*/

#include "main.h"

vector<Color*> _colors;
vector<Point*> _points;
short _num;

static inline void addToList(Point *p, Color *col)
{
    _points.push_back(p);
    _colors.push_back(col);
}

static void keyboardFunc(unsigned char key, int x, int y) 
{
    switch(key)
    {
        case 'a': // Add a new point
        {
            Point *a = new Point();
            Color *col = new Color();
            addToList(a, col);
            break;
        }
        case 'd': // Delete the last point
        {
            _points.pop_back();
            _colors.pop_back();
            break;
        }
    }
}

static void reshapeFunc(int w, int h) 
{
    if (h == 0)
        h = 1;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ((float)w)/((float)h), 1, 200);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 0.0,
              0.0, 0.0, 0.0,
              0.0, 0.0, 0.0);    
}

static void displayFunc(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    gluOrtho2D(0.0f, WINDOW_X, 0.0f, WINDOW_Y);
       
    glPointSize(10);
    for (int i = 0; i < _points.size(); i++)
    {
            glColor3f(_colors[i]->r, _colors[i]->g, _colors[i]->b);

            glBegin(GL_POINTS);
                glVertex2f(_points[i]->_pos_x, _points[i]->_pos_y);
            glEnd();
    }              
      
    glFlush();
    glutSwapBuffers();
}   

static void initRendering() 
{
    glDisable(GLUT_DEPTH);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f); 
    
    cout << "=================" << endl;
    cout << "N-Body Simulation" << endl;
    cout << "=================\n" << endl;
    cout << "Enter number of particles (Hit Enter for default value): ";
    
    string cr;
    getline(cin, cr);

    int i = -1;
    stringstream(cr) >> i;
    _num = (i < 0) ? DEFAULT_NUM : i;
    
    cout << "\n\n-----------------" << endl;
    cout << "       Key       " << endl;
    cout << "-----------------" << endl;
    cout << " a: Add a new Point" << endl;
    cout << " d: Delete the last Point" << endl;
  
    srand(time(NULL));

    for (int i = 0; i < (int)_num; i++)
        addToList(new Point(), new Color());
}

static void update(int value) 
{   
    /*
    *   The heart of the program. Uses an inverse-square relation with respect
    *   to the distance between two distinct points and a pre-specified K_VAL
    *   to produce a value that will be appended to the point's velocity. Not 
    *   much that can be done about the O(n^2)-ness of this. I've also corrected
    *   for excessive increments so things don't go flying off with ridiculous
    *   speeds.
    */
    
    for (int i = 0; i < _points.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            float _delX = SQ(_points[i]->_pos_x - _points[j]->_pos_x);
            float incX = MIN(K_VAL / _delX, MAX_ACC);
            
            _points[i]->_vel_x  = (_points[i]->_pos_x - _points[j]->_pos_x > 0) ?
                MIN(_points[i]->_vel_x + incX, MAX_VEL) :
                MAX(_points[i]->_vel_x - incX, -MAX_VEL);

            float _delY = SQ(_points[i]->_pos_y - _points[j]->_pos_y);
            float incY = MIN(K_VAL/_delY, MAX_ACC);
            
            _points[i]->_vel_y  = (_points[i]->_pos_y - _points[j]->_pos_y > 0) ?
                MIN(_points[i]->_vel_y + incY, MAX_VEL) :
                MAX(_points[i]->_vel_y - incY, -MAX_VEL);
        }
    }  
    
    float _temp_x, _temp_y, _rot_1, _rot_2;
    
    for (int i = 0; i < _points.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            /*
            *   Handle collisions between particles exploiting the fact that
            *   they are squares. All collisions are elastic. This becomes a 
            *   rough approximation with rotation dynamics are turned on.
            */
            
            float curX = _points[i]->_pos_x + _points[i]->_vel_x;
            float curY = _points[i]->_pos_y + _points[i]->_vel_y;
            
            if (curX <= _points[j]->_pos_x + _points[j]->_vel_x + 5.0f
             && curX >= _points[j]->_pos_x + _points[j]->_vel_x - 5.0f
             && curY <= _points[j]->_pos_y + _points[j]->_vel_y + 5.0f
             && curY >= _points[j]->_pos_y + _points[j]->_vel_y - 5.0f)
            {
                _temp_x = _points[i]->_vel_x;
                _temp_y = _points[i]->_vel_y;
                _points[i]->_vel_x = _points[j]->_vel_x;
                _points[i]->_vel_y = _points[j]->_vel_y;
                _points[j]->_vel_x = _temp_x;
                _points[j]->_vel_y = _temp_y;
            }
        }
        
        /*
        *   Handle all collisions with the edge of the window.
        */
        
        if (_points[i]->_pos_x + _points[i]->_vel_x >= WINDOW_X - 10 ||
            _points[i]->_pos_x + _points[i]->_vel_x <= 0)
            _points[i]->_vel_x = -_points[i]->_vel_x;
        
        if (_points[i]->_pos_y + _points[i]->_vel_y >= WINDOW_Y - 10 ||
            _points[i]->_pos_y + _points[i]->_vel_y <= 0)
            _points[i]->_vel_y = -_points[i]->_vel_y;

        _points[i]->_pos_x += _points[i]->_vel_x;
        _points[i]->_pos_y += _points[i]->_vel_y;
             
    }
            
    glutPostRedisplay();
    glutTimerFunc(10, update, 0); 
}

int main( int argc, char** argv ) 
{ 
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_X, WINDOW_Y);

    glutCreateWindow("N-Body Simulation");
    initRendering();

    glutDisplayFunc(displayFunc);
    glutKeyboardFunc(keyboardFunc);
    glutReshapeFunc(reshapeFunc);

    glutTimerFunc(25, update, 0);
    glutMainLoop();
   
    return 0; 
}
