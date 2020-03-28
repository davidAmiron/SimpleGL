#define GLEW_STATIC

#include "GL/glew.h"
#include "SDL2/SDL.h"

#include "simplegl.h"

#include <vector>
#include <iostream>

SimpleGL::SimpleGL(std::string title, int loc_x, int loc_y, int width, int height) :
        window_title_(title), window_loc_x_(loc_x), window_loc_y_(loc_y),
        window_width_(width), window_height_(height)
{
    std::cout << "Created" << std::endl;
}

void SimpleGL::Init()
{
    // Initialize SDL2
    SDL_Init(SDL_INIT_VIDEO);

    // Initialize attributes for OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Create window and OpenGL context
    window_ = SDL_CreateWindow(window_title_.c_str(), window_loc_x_, window_loc_y_,
                    window_width_, window_height_, SDL_WINDOW_OPENGL);
    gl_context_ = SDL_GL_CreateContext(window_);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Use Vertex Array Object to save links between attributes and VBOs
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Initialize virtual buffer objects
    glGenBuffers(1, &triangles_vbo_);
    glGenBuffers(1, &lines_vbo_);

    // Initialize element buffer objects
    glGenBuffers(1, &triangles_ebo_);
    glGenBuffers(1, &lines_ebo_);

    // Initialize shaders
    vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_, 1, &kVertexShaderSource, NULL);
    glCompileShader(vertex_shader_);

    fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_, 1, &kFragmentShaderSource, NULL);
    glCompileShader(fragment_shader_);

    // Create program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glUseProgram(program);

    GLint pos_attrib = glGetAttribLocation(program, "position");
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
    glEnableVertexAttribArray(pos_attrib);

    GLint color_attrib = glGetAttribLocation(program, "color");
    glVertexAttribPointer(color_attrib, 4, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)2*sizeof(float));
    glEnableVertexAttribArray(color_attrib);

}

bool SimpleGL::Update()
{
    SDL_Event window_event;
    if (SDL_PollEvent(&window_event))
    {
        if (window_event.type == SDL_QUIT) {
            SimpleGL::Destroy();
            return false;
        }
    }

    return true;
}

void SimpleGL::Draw()
{
    // Update virtual buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, triangles_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices_.data()), triangle_vertices_.data(), GL_STATIC_DRAW);
    triangle_vertices_.clear();

    glBindBuffer(GL_ARRAY_BUFFER, lines_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices_.data()), line_vertices_.data(), GL_STATIC_DRAW);
    line_vertices_.clear();

    // Update element buffer objects
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangles_ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_elements_.data()), triangle_elements_.data(), GL_STATIC_DRAW);
    triangle_elements_.clear();
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lines_ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(line_elements_.data()), line_elements_.data(), GL_STATIC_DRAW);
    line_elements_.clear();
}

void SimpleGL::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, SimpleGL::Color color)
{
    
}

void SimpleGL::DrawRect(int x, int y, int width, int height, SimpleGL::Color color)
{

}

void SimpleGL::DrawLine(int x1, int y1, int x2, int y2, SimpleGL::Color color)
{
}

void SimpleGL::Destroy()
{
    SDL_GL_DeleteContext(gl_context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}
