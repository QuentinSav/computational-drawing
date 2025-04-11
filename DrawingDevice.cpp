#include "DrawingDevice.h"

DrawingDevice::DrawingDevice():
    m_plot(&m_points)
{}

void DrawingDevice::printImage()
{}

Linkage::~Linkage()
{
    if (mem_flag)
    {
        delete p[0];
        delete p[1];
    }    
}

void Linkage::link(Point_3D* p1, Point_3D* p2)
{
    if (p[0] == nullptr) p[0] = p1;
    else std::cout << "First point is either fixed or already linked" << std::endl;

    if (p[1] == nullptr) p[1] = p2;
    else std::cout << "Second point is either fixed or already linked" << std::endl;
}

void Linkage::fix()
{
    p[1] = new Point_3D;
    mem_flag = true;
}

void Linkage::compute()
{
    // Compute the third length
    float l3 = sqrt(pow(p[1]->x - p[0]->x, 2) + pow(p[1]->y - p[0]->y, 2));
    
    // Compute the angle gamma (in the triangle)
    float gamma = atan2((p[1]->y - p[0]->y), (p[1]->x - p[0]->x));

    // Compute the angle theta (in the complex plane)
    float theta = abs(acos((pow(l[0], 2) + pow(l3, 2) - pow(l[1], 2))/(2*l3*l[0])));

    // Compute the pen position
    p0.x = (p[0]->x + l[0] * cos(theta + gamma));
    p0.y = (p[0]->x + l[0] * sin(theta + gamma));
    p0.z = 0.0f;
}