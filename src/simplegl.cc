#define GLEW_STATIC

#include "GL/glew.h"
#include "SDL2/SDL.h"

#include "simplegl.h"

#include <vector>
#include <iostream>

SimpleGL::SimpleGL(std::string title, int loc_x, int loc_y, int width, int height, SimpleGL::Color background_color) :
        window_title_(title), window_loc_x_(loc_x), window_loc_y_(loc_y),
        window_width_(width), window_height_(height), background_color_(background_color)
{
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

    // Set Background Color
    glClearColor(background_color_.red, background_color_.green, background_color_.blue, background_color_.alpha);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window_);
    glClear(GL_COLOR_BUFFER_BIT);

    // Initialize shaders
    vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_, 1, &kVertexShaderSource, NULL);
    glCompileShader(vertex_shader_);

    fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_, 1, &kFragmentShaderSource, NULL);
    glCompileShader(fragment_shader_);

    // Create program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader_);
    glAttachShader(program, fragment_shader_);
    //glBindFragDataLocation(program, 0, "fragment_color_out");
    glLinkProgram(program);
    glUseProgram(program);

    GLint Result;
    int InfoLogLength;

    glGetProgramiv(program, GL_LINK_STATUS, &Result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::cout << InfoLogLength << std::endl;
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    GLint status;
    glGetShaderiv(vertex_shader_, GL_COMPILE_STATUS, &status);
    char buffer[512];
    glGetShaderInfoLog(fragment_shader_, 512, NULL, buffer);
    std::cout << buffer << std::endl;

    // Initialize virtual buffer objects
    glGenBuffers(1, &triangles_vbo_);
    glGenBuffers(1, &lines_vbo_);

    // Initialize element buffer objects
    glGenBuffers(1, &triangles_ebo_);
    glGenBuffers(1, &lines_ebo_);


    // Bind the buffer that the below attributes pertain to
    glBindBuffer(GL_ARRAY_BUFFER, triangles_vbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangles_ebo_);

    GLint pos_attrib = glGetAttribLocation(program, "vertex_position_in");
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);

    GLint color_attrib = glGetAttribLocation(program, "vertex_color_in");
    glVertexAttribPointer(color_attrib, 4, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(2*sizeof(float)));

    glEnableVertexAttribArray(pos_attrib);
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

    SimpleGL::Draw();
    SDL_GL_SwapWindow(window_);

    return true;
}

void SimpleGL::Draw()
{
    // Update virtual buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, triangles_vbo_);

    
    float vertices[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f,    // Vertex 1 (X, Y, r, g, b, a)
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,    // Vertex 2 (X, Y, r, g, b, a)
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,    // Vertex 3 (X, Y, r, g, b, a)
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f     // Vertex 4 (X, Y, r, g, b, a)
    };

    float *d = triangle_vertices_.data();
    std::cout << sizeof(d) / sizeof(d[0]) << std::endl;
    std::cout << typeid(vertices).name() << std::endl;
    std::cout << sizeof(triangle_vertices_.data()) << std::endl;
    std::cout << triangle_vertices_.size() << std::endl;


    glBufferData(GL_ARRAY_BUFFER, triangle_vertices_.size()*sizeof(float), triangle_vertices_.data(), GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ARRAY_BUFFER, lines_vbo_);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices_.data()), line_vertices_.data(), GL_STATIC_DRAW);

    // Update element buffer objects
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangles_ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangle_elements_.size()*sizeof(int), triangle_elements_.data(), GL_STATIC_DRAW);
    
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lines_ebo_);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(line_elements_.data()), line_elements_.data(), GL_STATIC_DRAW);

    // Draw elements
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawElements(GL_TRIANGLES, triangle_elements_.size(), GL_UNSIGNED_INT, 0);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    std::cout << triangle_elements_.size() / 3 << std::endl;
    //glDrawElements(GL_LINES, line_elements_.size() / 2, GL_UNSIGNED_INT, 0);

    // Clear vertex and element arrays
    //triangle_vertices_.clear();
    //line_vertices_.clear();
    //triangle_elements_.clear();
    //line_elements_.clear();
}

void SimpleGL::do_stuff()
{
    std::cout << "Doing Stuff" << std::endl;
    float vertices[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f,    // Vertex 1 (X, Y, r, g, b, a)
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,    // Vertex 2 (X, Y, r, g, b, a)
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,    // Vertex 3 (X, Y, r, g, b, a)
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f     // Vertex 4 (X, Y, r, g, b, a)
    };

    glBindBuffer(GL_ARRAY_BUFFER, triangles_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    SDL_GL_SwapWindow(window_);
}

void SimpleGL::DrawTriangle(SimpleGL::Point p1, SimpleGL::Point p2, SimpleGL::Point p3, SimpleGL::Color color)
{
    float x1f, y1f, x2f, y2f, x3f, y3f;
    SimpleGL::NormalizePointToGLCoords(p1, x1f, y1f);
    SimpleGL::NormalizePointToGLCoords(p2, x2f, y2f);
    SimpleGL::NormalizePointToGLCoords(p3, x3f, y3f);
    
    triangle_vertices_.push_back(x1f);
    triangle_vertices_.push_back(y1f);
    SimpleGL::AppendColor(triangle_vertices_, color);

    triangle_vertices_.push_back(x2f);
    triangle_vertices_.push_back(y2f);
    SimpleGL::AppendColor(triangle_vertices_, color);

    triangle_vertices_.push_back(x3f);
    triangle_vertices_.push_back(y3f);
    SimpleGL::AppendColor(triangle_vertices_, color);

    triangle_elements_.push_back(triangle_elements_.size());
    triangle_elements_.push_back(triangle_elements_.size());
    triangle_elements_.push_back(triangle_elements_.size());
}

void SimpleGL::AppendColor(std::vector<float>& vec, SimpleGL::Color color)
{
    vec.push_back(color.red);
    vec.push_back(color.green);
    vec.push_back(color.blue);
    vec.push_back(color.alpha);
}

void SimpleGL::NormalizePointToGLCoords(SimpleGL::Point point, float& x_gl, float& y_gl)
{
    x_gl = (point.x / (float)window_width_) * 2 - 1;
    y_gl = (point.y / (float)window_height_) * 2 - 1;
}

void SimpleGL::print_vector(std::vector<float> vec)
{
    for (int i = 0; i < vec.size(); i++)
        std::cout << vec.at(i) << " ";
    std::cout << std::endl;
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
