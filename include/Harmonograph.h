#ifndef HARMONOGRAPH_H
#define HARMONOGRAPH_H

#include "DrawingDevice.h"

enum DeviceType {
    DOUBLE_PLANAR_PENDULUM,
    DOUBLE_SPHERICAL_PENDULUM,
    PENDULUM_3D
};

enum PredefinedParam {
    DEFAULT
};

struct PlanarPendulum {
    // Parameters
    Point_3D* c = nullptr;
    float init_position_theta;
    float init_speed_theta;
    float weight;
    float length_1, length_2;
    float b;

    // Outputs
    Point_3D p0;

    // Functions
    ~PlanarPendulum();
    Point_3D getPlane();
    void compute(float t); 
    void setAngle(float angle);

    // Needed flag
    bool mem_flag = false;
};

class Harmonograph : protected DrawingDevice
{
public:
    Harmonograph();
    void setParam(Feature feat, int i_feat, FeatParamNames param, float value) override;
    void initTransform(std::vector<int>& angles, int depth = 0) override;
    void setPredefinedParam(PredefinedParam predefined_param);
    void run() override;

protected:
    DeviceType m_device_type;
    std::vector<PlanarPendulum> m_pendula;
    Point_3D* m_output;
};

#endif // HARMONOGRAPH_H