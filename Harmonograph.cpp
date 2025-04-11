#include "Harmonograph.h"

Harmonograph::Harmonograph(DeviceType device_type):
    m_device_type(device_type)
{
    // Define structure
    switch (m_device_type)
    {
        case DOUBLE_PLANAR_PENDULUM:
            // Create vectors with the mechanical objects
            m_pendula.resize(2);

            // Link the different objects
            m_linkages[0].link(&(m_cranks[0].p0), &(m_cranks[1].p0));

            // Set ouptut address
            m_output = &(m_linkages[0].p0);

            break;
    
    }
}

void Harmonograph::setPredefinedParam(PredefinedParam predefined_param)
{
    switch(predefined_param)
    {
        case DEFAULT:
            // Define pendulum
            setParam(PLANAR_PENDULUM, 0, PP_INITPOS_THETA, 1.0f);
            setParam(PLANAR_PENDULUM, 0, PP_INITSPEED_THETA, 0.0f);
            setParam(PLANAR_PENDULUM, 0, PP_MASS, 0.0f);
            setParam(PLANAR_PENDULUM, 0, PP_LENGTH_1, 2.0f);
            setParam(PLANAR_PENDULUM, 0, PP_LENGTH_2, 1.0f);
            setParam(PLANAR_PENDULUM, 0, PP_DAMPING, 0.01f);
    }
}