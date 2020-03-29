
// This must be before OpenGL and SDL2 includes
#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

int main()
{
    // Initialize SDL2
    SDL_Init(SDL_INIT_VIDEO);

    // Initialize attributes for OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Create the window and OpenGL context
    SDL_Window *window = SDL_CreateWindow("OpenGL Test", 100, 100, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Use Vertex Array Object to save links between attributes and VBOs
    GLuint vao;
    glGenVertexArrays(1, &vao);

    // Enable this Vertex Array Object
    glBindVertexArray(vao);

    // Set background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
    glClear(GL_COLOR_BUFFER_BIT);

 // Define vertex shader to interprate vertex array
    const char* vertexSource = R"glsl(
        #version 150 core

        in vec2 position;
        in vec3 color;

        out vec3 Color;

        void main()
        {
            gl_Position = vec4(position, 0.0, 1.0);
            Color = color;
        }
    )glsl";

    // Create vertexshader object
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Copy vertextShader source into object
    glShaderSource(vertexShader, 1, &vertexSource, NULL);

    // Compile vertexShader
    glCompileShader(vertexShader);


    // Define fragment shader to decide color for each fragment/pixel
    const char* fragmentSource = R"glsl(
        #version 150 core

        in vec3 Color;

        uniform float alpha;
        uniform vec3 triangleColor;
        out vec4 outColor;

        void main()
        {
            outColor = vec4(0.1569, 0.6314, 0.5294, 0.5);
            //outColor = vec4(Color, 0.5f);
        }
    )glsl";

    // Create fragmentShader object
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Copy fragmentShader source into object
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

    // Compile fragmentShader
    glCompileShader(fragmentShader);



    // Create program to link shaders together
    GLuint shaderProgram = glCreateProgram();

    // Add shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Specify what output data is written to what buffer. In this program there is one output
    // data and the default buffer is 0 so the following line is not necessary.
    //glBindFragDataLocation(shaderProgram, 0, "outColor");

    // Link the program, necessary to register shader changes
    glLinkProgram(shaderProgram);

    // Set program to be active, only one can be active at a time
    glUseProgram(shaderProgram);




    // Now to upload memory to graphics card, where it is much faster
    // Create Vertex Buffer Object
    GLuint vbo;
    glGenBuffers(1, &vbo); // Create 1 buffer, make OpenGL unsigned int (GLuint) that is reference to buffer

    // Set this vbo as the active array buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);








    // Get position attribute of vertexShader
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    std::cout << "pos: " << posAttrib << std::endl;

    // Specify how this attribute is parsed from input data
    // This also binds using the current active VBO bount to GL_ARRAY_BUFFER
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);

    // Same for color
    GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
    std::cout << "color: " << colorAttrib << std::endl;
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));

    // Enable the attribute
    glEnableVertexAttribArray(posAttrib);
    glEnableVertexAttribArray(colorAttrib);











    /// Basic Drawing

    // Create vertices
    float vertices[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,     // Vertex 1 (X, Y, r, g, b)
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f,     // Vertex 2 (X, Y, r, g, b)
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f,     // Vertex 3 (X, Y, r, g, b)
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f      // Vertex 4 (X, Y, r, g, b)
    };



    // Sent vertices to the active array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // Create elements
    //GLuint elements[] = {
    //    0, 1, 2,
    //    2, 3, 0
    //};

    //GLuint elements[] = {
    //    0, 1, 1, 2, 2, 3, 3, 0
    //};

    // Element Buffer Object
    //GLuint ebo;
    //glGenBuffers(1, &ebo);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


    


    // Need to tell OpenGL how input data from vertices are formatted and ordered

        // Now tell OpenGL where to get data for triangleColor uniform

    // Get uniform location for trangleColor
    //GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");

    // Write to triangleColor uniform
    //glUniform3f(uniColor, 0.2f, 0.4f, 0.6f);

    //GLint alphaValue = glGetUniformLocation(shaderProgram, "alpha");


    // Drawing
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    

    SDL_GL_SwapWindow(window);


    auto t_start = std::chrono::high_resolution_clock::now();

    // Event Loop
    SDL_Event windowEvent;
    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
            {
                break;
            }
        }

        //auto t_now = std::chrono::high_resolution_clock::now();
        //float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
        //glUniform3f(uniColor, 0.2f, 0.4f, sin(time*3.0f + 1.0f) / 2.0f);
        //glUniform1f(alphaValue, 0.5f);
        //cout << (sin(time*3.0f) + 1.0f) / 2.0f << endl;

        

        //SDL_GL_SwapWindow(window);
    }

    // Delete the OpenGL context, destroy the window, and quit
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}
