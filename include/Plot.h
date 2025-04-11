#ifndef PLOT_H
#define PLOT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

struct Point_2D;
struct Point_3D;
struct Point_4D;

struct Point_4D {
    float x, y, z, c;

    Point_4D();
    Point_4D(Point_2D point_2d);
    Point_4D(Point_3D point_3d);
};

struct Point_3D {
    float x, y, z;

    Point_3D();
    Point_3D(Point_2D point_2d);
    Point_3D(Point_4D point_4d);
};

struct Point_2D {
    float x, y;

    Point_2D();
    Point_2D(Point_3D point_3d);
    Point_2D(Point_4D point_4d);
};

class Plot
{
public:
    Plot(std::vector<Point_4D>* points);
    ~Plot();

    void update();
    void setTransform(cv::Mat transform);
    GLFWwindow* m_window;

private:
    GLuint m_VBO, m_VAO;
    GLuint m_shaderProgram;
    std::vector<Point_4D>* m_points;
    glm::mat4 m_transform;
};

#endif // PLOT_H