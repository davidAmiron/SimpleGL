#include <iostream>
#include "simplegl.h"

SimpleGL::SimpleGL(int width, int height) :
        windowWidth_(width), windowHeight_(height)
{
    std::cout << "Created" << std::endl;
}

void SimpleGL::init()
{
}

void SimpleGL::update()
{
}

void SimpleGL::drawRect(int x, int y, int width, int height)
{
}

void SimpleGL::drawLine(int x1, int y1, int x2, int y2)
{
}
