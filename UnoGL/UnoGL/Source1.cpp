#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings, not mentioned but useful later prolly
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//A temporary(hopefully) representation of a GLSL shader, bit ugly
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.xyz, 1.0);\n" //'swizzling' is cool
"   vertexColor = aColor;\n" //sets vertex color to the 'other' vec3 defined in the vertices data 
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 vertexColor;\n" //the input from the above vertexShader, matching name and type
"uniform vec4 gloColor;\n" //global uniform color
"void main()\n"
"{\n"
"   FragColor = vec4(vertexColor, 1.0);\n"
//"   if(vertexColor.x < 0.6f)\n" //Interestingly overwrites every fragment that has a Red value over 0.6f, even interpolated ones; didn't expect that somehow.
//"   {\n"
//"      FragColor = gloColor;\n"
//"   }\n"
"}\n\0";


int main()
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //Register Callbacks before the render loop and after the window's creation
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //BasicVertexShaderCreation
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //Check if the vertex compilation broke
    /*
    int  vsuccess;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vsuccess);
    if (!vsuccess)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    */

    //BasicFragmentShaderCreation
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    //Check if the fragment compilation broke
    /*
    int  fsuccess;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fsuccess);
    if (!fsuccess)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    */

    //Link shaders to one shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //Check if the shader program blew up
    /*
    int  psuccess;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &psuccess);
    if (!psuccess)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    */

    //Delete the shaders now since they're in the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Now a Rectangle
    float vertices[] = {
    //positions         //colors
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,//bot left
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,//bot right
     0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,//top right
     -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f //top left
    };
    unsigned int indices[] = {
        2, 1, 0, //triangle1
        0, 3, 2  //triangle2
    };

    //creating buffer/array objects
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Bind order: Vertex Array, Vertex buffer(s), Vertex attribute(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /*Tells OpenGL how to interpret vertex data :
    0 matchs the layout location specified in the vertex shader source
    3 is the size of each attribute, it's a vector3.
    GL_FLOAT is the type of each coordinate.
    GL_FALSE specifies that we don't want the data normalized.
    6 * sizeof(float) is the stride, or the total space between vertex attributes. There are 6 floats between attributes now.
    (void*)0 is the offset of where the data begins, it is 0 because we're handing it the data plainly.
        (void*)(3* sizeof(float)) is the new offset for the color value, since it starts 3 floats into the array.
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //^-position color-v
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    //how to unbind for safety if needed
    //glBindVertexArray(0);

    //Render loop, each iteration a frame
    while (!glfwWindowShouldClose(window))
    {
        //input func
        processInput(window);

        //rendering commands-- (tells the program to clear old frames with a certain color)
        glClearColor(0.1f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Color value that changes over time
        float timeV = glfwGetTime();
        float oscColorV = (sin(timeV) / 2.0f) + 0.5f;
        //Locating the now implanted uniform global value
        int vertexColorLoc = glGetUniformLocation(shaderProgram, "gloColor");

        //Draw--
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLoc, 0.0f, oscColorV, 0.0f, 1.0f); //sending the oscillating value while the program is running
        glBindVertexArray(VAO); //Technically not needed without a second VAO
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //check+call events, swapping buffers prevents users from seeing the image while its being drawn(?)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

