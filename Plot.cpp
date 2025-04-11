#include "Plot.h"

// Vertex shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in float aCol;
    uniform mat4 transform;
    
    out float Col;

    void main() {
        gl_Position = transform * vec4(aPos, 1.0);
        Col = aCol;
    }
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(
    #version 330 core
    
    in float Col;
    out vec4 FragColor;

    void main() {
        FragColor = vec4(Col, 1.0, 1.0, 1.0); // Red color
    }
)";

Plot::Plot(std::vector<Point_4D>* points):
    m_points(points)
{
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
    }

    // Create a GLFW window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    m_window = glfwCreateWindow(900, 900, "OpenGL Line", nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(m_window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }

    // Compile and link the shader program
    GLuint vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_points->size() * sizeof(Point_4D), m_points->data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point_4D), (void*)offsetof(Point_4D, x));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Point_4D), (void*)offsetof(Point_4D, c));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 1);
    glBindVertexArray(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_DEPTH_TEST);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

Plot::~Plot()
{
    // Cleanup and exit
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteProgram(m_shaderProgram);

    glfwTerminate();
}

void Plot::update()
{   
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "transform"), 1, GL_FALSE, &m_transform[0][0]);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Create VAO, VBO, and set up vertex data for the lines
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_points->size() * sizeof(Point_4D), m_points->data(), GL_STATIC_DRAW);

    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_VAO);
    
    glDrawArrays(GL_LINE_STRIP, 0, m_points->size());

    glBindVertexArray(0);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Plot::setTransform(cv::Mat transform)
{
    m_transform = glm::mat4(1.0f);

    if (transform.rows == 4 && transform.cols == 4) {
        for (int i = 0; i < transform.rows; ++i) {
            for (int j = 0; j < transform.cols; ++j) {
                m_transform[i][j] = transform.at<float>(i, j); 
            }
        }
    } 
}

Point_4D::Point_4D()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    c = 1.0f;
}

Point_4D::Point_4D(Point_2D point_2d)
{
    x = point_2d.x;
    y = point_2d.y;
    z = 0.0f;
    c = 1.0f;
}

Point_4D::Point_4D(Point_3D point_3d)
{
    x = point_3d.x;
    y = point_3d.y;
    z = point_3d.z;
    c = 1.0f;
}

Point_3D::Point_3D()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

Point_3D::Point_3D(Point_2D point_2d)
{
    x = point_2d.x;
    y = point_2d.y;
    z = 0.0f;
}

Point_3D::Point_3D(Point_4D point_4d)
{
    x = point_4d.x;
    y = point_4d.y;
    z = point_4d.z;
}

Point_2D::Point_2D()
{
    x = 0.0f;
    y = 0.0f;
}

Point_2D::Point_2D(Point_3D point_3d)
{
    x = point_3d.x;
    y = point_3d.y;
}

Point_2D::Point_2D(Point_4D point_4d)
{
    x = point_4d.x;
    y = point_4d.y;
}
