#ifndef __MAIN_H__
#define __MAIN_H__

#define GLUT_DISABLE_ATEXIT_HACK

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <GL/glut.h>
#include <string>
#include <sstream>

#define WINDOW_X 400
#define WINDOW_Y 400
#define MAX_VEL 3
#define MAX_ACC 3
#define MAX_ROT 3
#define K_VAL 0.01
#define DEFAULT_NUM 3

#define SIGN(x) !!((x) >= 0.0f)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define SQ(x) ((x) * (x))

using namespace std;

struct Point
{
    public: 
        float _pos_x;
        float _pos_y;
        float _vel_x;
        float _vel_y;
        
        Point() :   _pos_x(float(rand() % WINDOW_X)),
                    _pos_y(float(rand() % WINDOW_Y)), 
                    _vel_x(float((rand() % (2*MAX_VEL)) - MAX_VEL)), 
                    _vel_y(float((rand() % (2*MAX_VEL)) - MAX_VEL)) {}
};

struct Color
{
    public:
        float r;
        float g;
        float b;
        
        Color() :   r(float(rand() % 255)/255),
                    g(float(rand() % 255)/255),
                    b(float(rand() % 255)/255) {}
};

#endif
