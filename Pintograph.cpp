#include "Pintograph.h"

Pintograph::Pintograph(DeviceType device_type):
    m_device_type(device_type)
{
    // Define structure
    switch (m_device_type)
    {
        case TWO_CRANKS_LINKAGE:
            // Create vectors with the mechanical objects
            m_cranks.resize(2);
            m_linkages.resize(1);

            // Link the different objects
            m_cranks[0].fix();
            m_cranks[1].fix();
            m_linkages[0].link(&(m_cranks[0].p0), &(m_cranks[1].p0));

            // Set ouptut address
            m_output = &(m_linkages[0].p0);

            break;
    
        case THREE_CRANKS_LINKAGE:
            // Create vectors with the mechanical objects
            m_cranks.resize(3);
            m_linkages.resize(2);

            // Link the different objects
            m_cranks[0].fix();
            m_cranks[1].fix();
            m_cranks[2].fix();
            m_linkages[0].link(&(m_cranks[0].p0), &(m_cranks[1].p0));
            m_linkages[1].link(&(m_cranks[2].p0), &(m_linkages[0].p0));

            // Set ouptut address
            m_output = &(m_linkages[1].p0);

            break;

        case TWO_DOUBLE_CRANKS_LINKAGE:
            // Create vectors with the mechanical objects
            m_cranks.resize(4);
            m_linkages.resize(1);

            // Link the different objects
            m_cranks[2].link(&(m_cranks[0].p0));
            m_cranks[3].link(&(m_cranks[1].p0));
            m_linkages[0].link(&(m_cranks[2].p0), &(m_cranks[3].p0));

            // Set ouptut address
            m_output = &(m_linkages[0].p0);

            break;

        case THREE_DOUBLE_CRANKS_LINKAGE:
            // Create vectors with the mechanical objects
            m_cranks.resize(6);
            m_linkages.resize(2);

            // Link the different objects
            m_cranks[3].link(&(m_cranks[0].p0));
            m_cranks[4].link(&(m_cranks[1].p0));
            m_cranks[5].link(&(m_cranks[2].p0));
            m_linkages[0].link(&(m_cranks[3].p0), &(m_cranks[4].p0));
            m_linkages[1].link(&(m_cranks[5].p0), &(m_linkages[0].p0));

            // Set ouptut address
            m_output = &(m_linkages[1].p0);

            break;
    }
}

void Pintograph::setParam(Feature feat, int i_feat, FeatParamNames param, float value)
{
    if (feat == CRANK)
    {
        switch(param)
        {
            case C_CENTER_X:
                m_cranks[i_feat].c->x = value;
                break;

            case C_CENTER_Y:
                m_cranks[i_feat].c->y = value;
                break;
            
            case C_CENTER_Z:
                m_cranks[i_feat].c->z = value;
                break; 
            
            case C_RADIUS:
                m_cranks[i_feat].radius = value;
                break;
            
            case C_OMEGA:
                m_cranks[i_feat].omega = value;
                break;
            
            case C_PHI:
                m_cranks[i_feat].phi = value;
                break;
            
            default:
                std::cout << "The parameter " << param << " does not exist for feature " << feat << std::endl;
                break; 

        }
    }
    else if (feat == LINKAGE)
    {
        switch(param)
        {
            case L_ENDPOINT_X:
                m_linkages[i_feat].p[0]->x = value;
                break;
            
            case L_ENDPOINT_Y:
                m_linkages[i_feat].p[0]->y = value;
                break;
            
            case L_ENDPOINT_Z:
                m_linkages[i_feat].p[0]->z = value;
                break;
            
            case L_LENGTH_1:
                m_linkages[i_feat].l[0] = value;
                break;
            
            case L_LENGTH_2:
                m_linkages[i_feat].l[1] = value;
                break;
            
            default:
                std::cout << "The parameter " << param << " does not exist for feature " << feat << std::endl;
                break; 
        }
    }
}

void Pintograph::initTransform(std::vector<int>& index_array, int depth)
{
    // Initialize an array for the extrema points
    static float max_x = NAN;
    static float min_x = NAN;
    static float max_y = NAN;
    static float min_y = NAN;

    // Initialize an array for the angles
    std::vector<float> angles_array;
    for (int k = 0; k < 16; k++)
    {
        angles_array.push_back(static_cast<float>(k) * M_PI/8.0f);
    }
    
    // Main loop (when innermost), i.e. depth is equal to the number of cranks. 
    if (depth == (int)m_cranks.size()) {

        for (int i = 0; i < (int)m_cranks.size(); i++) { m_cranks[i].setAngle(angles_array[index_array[i]]); }
        for (int i = 0; i < (int)m_linkages.size(); i++) { m_linkages[i].compute(); }
        
        if ((m_output->x < min_x) || isnan(min_x)) { min_x = m_output->x; }
        if ((m_output->x > max_x) || isnan(max_x)) { max_x = m_output->x; }
        if ((m_output->y < min_y) || isnan(min_y)) { min_y = m_output->y; }
        if ((m_output->y > max_y) || isnan(max_y)) { max_y = m_output->y; }
        
        // Last iteration
        if (std::accumulate(index_array.begin(), index_array.end(), 0) == depth * ((int)angles_array.size() - 1))
        {
            // Compute transformation and scaling vectors
            float S_x = abs(1.9f/(max_x - min_x));
            float S_y = abs(1.9f/(max_y - min_y));
            float T_x = S_x * -(min_x + max_x)/2.0f;
            float T_y = S_y * -(min_y + max_y)/2.0f;

            // Construct matrix
            cv::Mat transform = cv::Mat::zeros(4, 4, CV_32F);
            transform.at<float>(0, 0) = S_x;   
            transform.at<float>(1, 1) = S_y;   
            transform.at<float>(2, 2) = 0.0f;   
            transform.at<float>(3, 0) = T_x;   
            transform.at<float>(3, 1) = T_y;   
            transform.at<float>(3, 2) = 0.0f;
            transform.at<float>(3, 3) = 1.0f;

            m_plot.setTransform(transform);
        }

        return;
    }

    // Recursion
    for (int i = 0; i < (int)angles_array.size(); i++) {
        index_array.push_back(i);
        initTransform(index_array, depth + 1);
        index_array.pop_back();
    }    
}

void Pintograph::setPredefinedParam(PredefinedParam predefined_param)
{
    switch(predefined_param)
    {
        case DEFAULT_2:
            // Define first crank
            setParam(CRANK, 0, C_RADIUS, 1.0f);
            setParam(CRANK, 0, C_CENTER_X, 0.0f);
            setParam(CRANK, 0, C_CENTER_Y, 0.0f);
            setParam(CRANK, 0, C_CENTER_Z, 0.0f);
            setParam(CRANK, 0, C_OMEGA, 2.0f);
            setParam(CRANK, 0, C_PHI, 0.01f);

            // Define second crank
            setParam(CRANK, 1, C_RADIUS, 0.71f);
            setParam(CRANK, 1, C_CENTER_X, 4.0f);
            setParam(CRANK, 1, C_CENTER_Y, 0.0f);
            setParam(CRANK, 1, C_CENTER_Z, 0.0f);
            setParam(CRANK, 1, C_OMEGA, 2.7423f);
            setParam(CRANK, 1, C_PHI, 0.13f);

            // Define bars lengths
            setParam(LINKAGE, 0, L_LENGTH_1, 6.0f);
            setParam(LINKAGE, 0, L_LENGTH_2, 7.13f);
        
        case DEFAULT_3:
            // Define first crank
            setParam(CRANK, 0, C_RADIUS, 0.8f);
            setParam(CRANK, 0, C_CENTER_X, 0.0f);
            setParam(CRANK, 0, C_CENTER_Y, 0.0f);
            setParam(CRANK, 0, C_CENTER_Z, 0.0f);
            setParam(CRANK, 0, C_OMEGA, 1.2f);
            setParam(CRANK, 0, C_PHI, 0.01f);

            // Define second crank
            setParam(CRANK, 1, C_RADIUS, 0.71f);
            setParam(CRANK, 1, C_CENTER_X, 6.0f);
            setParam(CRANK, 1, C_CENTER_Y, 4.0f);
            setParam(CRANK, 1, C_CENTER_Z, 0.0f);
            setParam(CRANK, 1, C_OMEGA, 3.6);
            setParam(CRANK, 1, C_PHI, 0.13f);

            // Define third crank
            setParam(CRANK, 2, C_RADIUS, 0.72f);
            setParam(CRANK, 2, C_CENTER_X, 2.0f);
            setParam(CRANK, 2, C_CENTER_Y, 6.0f);
            setParam(CRANK, 2, C_CENTER_Z, 0.0f);
            setParam(CRANK, 2, C_OMEGA, 3.03f);
            setParam(CRANK, 2, C_PHI, 0.65f);

            // Define bars lengths
            setParam(LINKAGE, 0, L_LENGTH_1, 5.1f);
            setParam(LINKAGE, 0, L_LENGTH_2, 7.13f);
            setParam(LINKAGE, 1, L_LENGTH_1, 11.0f);
            setParam(LINKAGE, 1, L_LENGTH_2, 10.0f);
    }
}

void Pintograph::run()
{
    // Initialize transform
    std::vector<int> index_array;
    initTransform(index_array);
    index_array.clear();

    // Define time step
    float t = 0.0f;
    float delta_t = 0.01f;
    int iter = 0;

    while (!glfwWindowShouldClose(m_plot.m_window))
    {
        for (int i = 0; i < (int)m_cranks.size(); i++) { m_cranks[i].compute(t); }
        for (int i = 0; i < (int)m_linkages.size(); i++) { m_linkages[i].compute(); }

        // Adds the point to vector and update time
        m_points.push_back(Point_4D(*m_output));
        
        t += delta_t;

        // Update figure
        if (!(iter%50)) { m_plot.update(); }

        iter++;
    }
}

Crank::~Crank()
{
    if (mem_flag)
    {
        delete c;
    }
}

void Crank::link(Point_3D* p)
{
    c = p;
}

void Crank::fix()
{
    c = new Point_3D;
    mem_flag = true;
}

void Crank::compute(float t)
{
    p0.x = radius*cos(omega*t + phi) + c->x;
    p0.y = radius*sin(omega*t + phi) + c->y;
    p0.z = 0.0f;
}

void Crank::setAngle(float angle)
{
    p0.x = radius*cos(angle) + c->x;
    p0.y = radius*sin(angle) + c->y;
    p0.z = 0.0f;
}
    
    