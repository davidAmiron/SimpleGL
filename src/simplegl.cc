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

    // Set Background Color
    glClearColor(background_color_.red, background_color_.green, background_color_.blue, background_color_.alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(window_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    // Initialize virtual buffer objects
    glGenBuffers(1, &triangles_vbo_);
    glGenBuffers(1, &lines_vbo_);

    // Initialize element buffer objects
    glGenBuffers(1, &triangles_ebo_);
    glGenBuffers(1, &lines_ebo_);

    // Initialize vertex array objects
    glGenVertexArrays(1, &triangles_vao_);
    glGenVertexArrays(1, &lines_vao_);


    // Bind triangles buffer and set vertex attributes
    glBindVertexArray(triangles_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, triangles_vbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangles_ebo_);

    GLint pos_attrib = glGetAttribLocation(program, "vertex_position_in");
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);

    GLint color_attrib = glGetAttribLocation(program, "vertex_color_in");
    glVertexAttribPointer(color_attrib, 4, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(2*sizeof(float)));

    glEnableVertexAttribArray(pos_attrib);
    glEnableVertexAttribArray(color_attrib);

    // Bind lines buffer and set vertex attributes
    glBindVertexArray(lines_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, lines_vbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lines_ebo_);

    //GLint pos_attrib_2 = glGetAttribLocation(program, "vertex_position_in");
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);

    //GLint color_attrib_2 = glGetAttribLocation(program, "vertex_color_in");
    glVertexAttribPointer(color_attrib, 4, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(2*sizeof(float)));

    glEnableVertexAttribArray(pos_attrib);
    glEnableVertexAttribArray(color_attrib);
}

void SimpleGL::Update()
{
    SimpleGL::Draw();
    SDL_GL_SwapWindow(window_);
}

void SimpleGL::Draw()
{
    // Clear screen
    glClearColor(background_color_.red, background_color_.green, background_color_.blue, background_color_.alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw triangles
    glBindVertexArray(triangles_vao_);

    glBindBuffer(GL_ARRAY_BUFFER, triangles_vbo_);
    glBufferData(GL_ARRAY_BUFFER, triangle_vertices_.size()*sizeof(float), triangle_vertices_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangles_ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangle_elements_.size()*sizeof(int), triangle_elements_.data(), GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, triangle_elements_.size(), GL_UNSIGNED_INT, 0);

    // Draw lines
    glBindVertexArray(lines_vao_);

    glBindBuffer(GL_ARRAY_BUFFER, lines_vbo_);
    glBufferData(GL_ARRAY_BUFFER, line_vertices_.size()*sizeof(float), line_vertices_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lines_ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, line_elements_.size()*sizeof(int), line_elements_.data(), GL_STATIC_DRAW);

    glDrawElements(GL_LINES, line_elements_.size(), GL_UNSIGNED_INT, 0);

    // Clear vertex and element arrays
    triangle_vertices_.clear();
    line_vertices_.clear();
    triangle_elements_.clear();
    line_elements_.clear();
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
    y_gl = -(point.y / (float)window_height_) * 2 + 1;
}

void SimpleGL::DrawRect(int x, int y, int width, int height, SimpleGL::Color color)
{
    SimpleGL::DrawTriangle(SimpleGL::Point(x, y),
                           SimpleGL::Point(x + width, y),
                           SimpleGL::Point(x + width, y + height),
                           color);

    SimpleGL::DrawTriangle(SimpleGL::Point(x + width, y + height),
                           SimpleGL::Point(x, y + height),
                           SimpleGL::Point(x, y),
                           color);

}

void SimpleGL::DrawPoint(SimpleGL::Point point, int radius, SimpleGL::Color color)
{
    int x = point.x;
    int y = point.y;
    SimpleGL::DrawTriangle(SimpleGL::Point(x, y + radius),
                           SimpleGL::Point(x + radius, y),
                           SimpleGL::Point(x, y - radius),
                           color);

    SimpleGL::DrawTriangle(SimpleGL::Point(x, y + radius),
                           SimpleGL::Point(x - radius, y),
                           SimpleGL::Point(x, y - radius),
                           color);
}

void SimpleGL::DrawLine(SimpleGL::Point p1, SimpleGL::Point p2, SimpleGL::Color color)
{
    float x1f, y1f, x2f, y2f;
    SimpleGL::NormalizePointToGLCoords(p1, x1f, y1f);
    SimpleGL::NormalizePointToGLCoords(p2, x2f, y2f);
    
    line_vertices_.push_back(x1f);
    line_vertices_.push_back(y1f);
    SimpleGL::AppendColor(line_vertices_, color);

    line_vertices_.push_back(x2f);
    line_vertices_.push_back(y2f);
    SimpleGL::AppendColor(line_vertices_, color);

    line_elements_.push_back(line_elements_.size());
    line_elements_.push_back(line_elements_.size());
}

bool SimpleGL::HasQuit()
{
    SDL_Event window_event;
    return SDL_PollEvent(&window_event) && window_event.type == SDL_QUIT;
}

void SimpleGL::Destroy()
{
    SDL_GL_DeleteContext(gl_context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}
