#ifndef PINTOGRAPH_H
#define PINTOGRAPH_H

#include <cmath>
#include <vector>
#include <numeric>
#include <memory>
#include <opencv2/opencv.hpp>  
#include "DrawingDevice.h"

/* TO BE IMPLEMENTED
    - TWO_CRANKS_FOUR_BARS_LINKAGE
    - Snaky line (FIFO buffer points)
    - Color management
    - Read predefined settings from file
*/

/* TEST TO BE DONE
    - Test each variant
*/

enum DeviceType {
    TWO_CRANKS_LINKAGE,
    THREE_CRANKS_LINKAGE,
    TWO_DOUBLE_CRANKS_LINKAGE,
    THREE_DOUBLE_CRANKS_LINKAGE
};

enum PredefinedParam {
    DEFAULT_2,
    DEFAULT_3
};

struct Crank {
    // Parameters
    Point_3D* c = nullptr;
    float radius;
    float omega;
    float phi;

    // Outputs
    Point_3D p0;

    // Functions
    ~Crank();
    void link(Point_3D* p);
    void fix();
    void compute(float t); 
    void setAngle(float angle);

    // Needed flag
    bool mem_flag = false;
};

class Pintograph : public DrawingDevice
{
public:
    Pintograph(DeviceType device_type);
    void setParam(Feature feat, int i_feat, FeatParamNames param, float value) override;
    void initTransform(std::vector<int>& angles, int depth = 0) override;
    void setPredefinedParam(PredefinedParam predefined_param);
    void run() override;

protected:
    DeviceType m_device_type;
    std::vector<Crank> m_cranks;
    std::vector<Linkage> m_linkages;
    Point_3D* m_output;
};

#endif // PINTOGRAPH_H