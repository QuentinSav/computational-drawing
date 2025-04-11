#ifndef DRAWINGDEVICE_H
#define DRAWINGDEVICE_H

#include "Plot.h"

enum Feature {
    CRANK,
    LINKAGE,
    PLANAR_PENDULUM,
    SPHERICAL_PENDULUM
};

enum FeatParamNames {
    // Crank parameters
    C_CENTER_X = 0,
    C_CENTER_Y,
    C_CENTER_Z,
    C_RADIUS,
    C_OMEGA,
    C_PHI,

    // Linkage parameters
    L_ENDPOINT_X = 100,
    L_ENDPOINT_Y,
    L_ENDPOINT_Z,
    L_LENGTH_1,
    L_LENGTH_2,

    // Simple (planar) pendulum parameters
    PP_ANCHOR_X = 200,
    PP_ANCHOR_Y,
    PP_ANCHOR_Z,
    PP_INITPOS_THETA,
    PP_INITSPEED_THETA,
    PP_MASS,
    PP_LENGTH_1,
    PP_LENGTH_2,
    PP_DAMPING,

    // Spherical pendulum parameters
    SP_ANCHOR_X = 300,
    SP_ANCHOR_Y,
    SP_ANCHOR_Z,
    SP_INITPOS_THETA,
    SP_INITPOS_PHI,
    SP_INITSPEED_THETA,
    SP_INITSPEED_PHI,
    SP_WEIGHT,
    SP_LENGTH_1,
    SP_LENGTH_2,
    SP_LENGTH_3,
    SP_DAMPING
};

struct Linkage {
    // Parameters
    Point_3D* p[2] = { nullptr, nullptr };
    float l[2];

    // Outputs
    Point_3D p0;
    
    //Functions
    ~Linkage();
    void link(Point_3D* p1, Point_3D* p2);
    void fix();
    void compute();

    // Needed flag
    bool mem_flag = false;
};

class DrawingDevice
{
public:
    DrawingDevice();
    virtual void run() = 0;
    virtual void setParam(Feature feat, int i_feat, FeatParamNames param, float value) = 0;
    virtual void initTransform(std::vector<int>& i, int depth = 0) = 0;

protected:
    std::vector<Point_4D> m_points;
    cv::Mat m_transform;
    Plot m_plot;

    void updatePlot();
    void printImage();
};

#endif // DRAWINGDEVICE_H