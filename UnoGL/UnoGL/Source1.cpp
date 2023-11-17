#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <UnoGL\stb_image.h>

#include <UnoGL\Shader.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings, not mentioned but useful later prolly
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//yay files
const char* vertexShaderPath = "vShader1.vs";
const char* fragmentShaderPath = "fShader1.fs";

//quick value to mess with via input
float inputV = 0.0f;


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

    //Use new shader class to make a shader
    Shader shaderProgram(vertexShaderPath, fragmentShaderPath);

    //Now a Rectangle
    float vertices[] = {
    //positions         //colors          //texture coords
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //bot left
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //bot right
     0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //top right
     -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f  //top left
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
    //Position Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Color Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    //Texture Coord Attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Examples of setting texture parameters, default is GL_REPEAT
    //  Set the mode for X and Y (S and T) separately, even Z (R) if using 3D textures.
    //      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    //      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //  If setting to GL_CLAMP_TO_BORDER, we should set a border color.
    //      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    //      float borderColor[] = { 0.4f, 0.7f, 1.0f, 1.0f };
    //      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //
    //  Setting Filter methods:
    //      GL_NEAREST is nearest neighbor and the default.
    //      GL_LINEAR is bilinear filtering.
    //      We can also specify which is used when upscaling or downscaling textures.
    //          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Mipmaps are basically a collection of textures generated from the inputted texture, each subsequent one at half the size of the previous.
    //Each is used in relation to how far the texture is from the viewer.
    //  Mipmap texture filtering can also be adjusted.
    //      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); for example would interpolate between the two closest mipmaps to a pixel size, then use nearest neighbor.

    //Load the textures
    //Using stbi_load, we give it the location of the file and it gives us the width, height, and number of color channels.
    int width, height, nrChannels;
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    //Generates a texture object. 1 is how many textures, any more and it'd give us an array.
    unsigned int texture;
    glGenTextures(1, &texture);
    //Bind to ensure subsequent commands affect this texture.
    glBindTexture(GL_TEXTURE_2D, texture);
    //Wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) //catches if i did a path wrong
    {
        //--todo:big explain--
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    //free image memory after we're done
    stbi_image_free(data);

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

        //Draw--
        shaderProgram.use(); //New swanky shaderprogram
        shaderProgram.setFloat("gloColor", oscColorV);
        shaderProgram.setFloat("texMove", inputV);
        glBindTexture(GL_TEXTURE_2D, texture); //new texture
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
    //swap between wireframe and full
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //push texture left and right
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        inputV += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        inputV -= 0.01f;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

