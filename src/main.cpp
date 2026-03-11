#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";
const char *yellowfragmentShaderSource = "#version 330 core\n"
                                         "out vec4 FragColor;\n"
                                         "void main()\n"
                                         "{\n"
                                         "    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                         "}\0";

void framebuffer_size_callback(GLFWwindow *, int width, int height);
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    int screen_width = 800;
    int screen_height = 600;
    GLFWwindow *window =
        glfwCreateWindow(screen_width, screen_height, "GLFW Window on Fedora", NULL, NULL);
    if (window == nullptr)
    {
        std::println("Failed to create GLFW window");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initializing GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::println("Failed to initialize GLAD");
        return 1;
    }
    // Fix for retina display, try on desktop or comment if doesn't work the same
    glfwGetFramebufferSize(window, &screen_width, &screen_height);
    glViewport(0, 0, screen_width, screen_height);
    std::println("Window successfully created");

    // build and compile shader program
    // --------------------------------
    // Vertex shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertex_shader);
    // check for compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
        std::println("Error::Shader::vertex::compilation_failed: {}", infoLog);
        return 1;
    }
    // Fragment shaders
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragment_shader);
    // check for compile errors
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
        std::println("Error::Shader::fragment::compilation_failed: {}", infoLog);
        return 1;
    }
    // yellow fragment shader
    unsigned int yellow_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(yellow_fragment_shader, 1, &yellowfragmentShaderSource, nullptr);
    glCompileShader(yellow_fragment_shader);
    // check for compile errors
    glGetShaderiv(yellow_fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(yellow_fragment_shader, 512, nullptr, infoLog);
        std::println("Error::Shader::fragment::compilation_failed: {}", infoLog);
        return 1;
    }

    // Create Shader Program and link shaders
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    // check for linking errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, nullptr, infoLog);
        std::println("Error::Shader::program::linking_failed: {}", infoLog);
        return 1;
    }
    // Delete first fragment shader since we've linked it and no longer need it
    glDeleteShader(fragment_shader);

    // Create second Shader Program and link shaders
    unsigned int yellow_shader_program = glCreateProgram();
    glAttachShader(yellow_shader_program, vertex_shader);
    glAttachShader(yellow_shader_program, yellow_fragment_shader);
    glLinkProgram(yellow_shader_program);
    // check for linking errors
    glGetProgramiv(yellow_shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(yellow_shader_program, 512, nullptr, infoLog);
        std::println("Error::Shader::program::linking_failed: {}", infoLog);
        return 1;
    }
    // delete vertex shader and second fragment shader since we've linked it and
    // no longer need it
    glDeleteShader(vertex_shader);
    glDeleteShader(yellow_fragment_shader);
    // Here onwards we can use both shader programs

    // Two arrays of vertices used from exercise 2 onwards
    float verticesA[] = {
        -0.5f, 0.5f, 0.0f, // A, top
        -0.9f, 0.0f, 0.0f, // A, left
        0.0f,  0.0f, 0.0f, // A right
    };
    float verticesB[] = {
        0.5f, 0.5f, 0.0f, // B top
        0.0f, 0.0f, 0.0f, // B left
        0.9f, 0.0f, 0.0f, // B right
    };

    // More clean solution for Exercise 2 onwards:
    unsigned int VAO[2] = {0};
    unsigned int VBO[2] = {0};
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    // bind the vertex array object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s)
    glBindVertexArray(VAO[0]);
    // 2. copy vertices array in a vertex buffer for OpenGL to use
    // First one, then the other
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesA), verticesA, GL_STATIC_DRAW);
    // then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // Now the other
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesB), verticesB, GL_STATIC_DRAW);
    // then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // process Input
        // -------------------
        processInput(window);

        // Rendering commands
        // -------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Actually draw shit
        // --------------------
        glUseProgram(shader_program);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Use the second shader program to draw the second triangle
        glUseProgram(yellow_shader_program);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
        // etc.)
        // ----------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}
