
#include "SDL2/SDL.h"
#include "GL/glew.h"

#include <string>
#include <vector>

class SimpleGL {

public:

    /**
     * Represent a color. Values are between 0 and 1.
     */
    struct Color {
        float red;
        float green;
        float blue;
        float alpha;

        Color (float r, float g, float b, float a) :
            red(r), green(g), blue(b), alpha(a)
        {
        }
    };

    /**
     * Create a SimpleGL object
     *
     * @param title The title of the window
     * @param loc_x The x location of the window
     * @param loc_y The y location of the window
     * @param height The window height in pixels
     * @param width The window width in pixels
     */
    SimpleGL(std::string title, int loc_x, int loc_y, int width, int height, SimpleGL::Color background_color);

    /**
     * Initialize the window
     */
    void Init();

    /**
     * Update the window
     *
     * @returns true if still running, false if window has been quit
     */
    bool Update();

    /**
     * Draw a triangle
     * @param x1, y1 The location of the first point
     * @param x2, y2 The location of the second point
     * @param x3, y3 The location of the third point
     * @param color The color of the triangle
     */
    void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, SimpleGL::Color color);

    /**
     * Draw a rectangle
     *
     * @param x, y The location of the top left corner of the rectangle
     * @param width The width of the rectangle
     * @param height The height of the rectangle
     */
    void DrawRect(int x, int y, int width, int height, SimpleGL::Color color);

    /**
     * Draw a line
     *
     * @param x1, y1 Location of first point of line
     * @param x2, y2 Location of second point of line
     */
    void DrawLine(int x1, int y1, int x2, int y2, SimpleGL::Color color);

    /**
     * Destroy the object, and clean up
     */
    void Destroy();

private:

    void Draw();

    std::string window_title_;
    int window_loc_x_, window_loc_y_, window_width_, window_height_;
    SimpleGL::Color background_color_;

    SDL_Window *window_;
    SDL_GLContext gl_context_;

    GLuint vertex_shader_;
    GLuint fragment_shader_;

    std::vector<float> triangle_vertices_;
    std::vector<float> line_vertices_;
    GLuint triangles_vbo_;
    GLuint lines_vbo_;
    std::vector<float> triangle_elements_;
    std::vector<float> line_elements_;
    GLuint triangles_ebo_;
    GLuint lines_ebo_;

    const char* kVertexShaderSource = R"glsl(
        #version 150 core

        in vec2 position;
        in vec3 color;

        out vec3 color_out;
    
        void main()
        {
            gl_Position = vec4(position, 0.0, 1.0);
            color_out = color;
        }
    )glsl";

    const char* kFragmentShaderSource = R"glsl(
        #version 150 core

        in vec3 color;
        
        out vec4 color_out;

        void main()
        {
            color_out = color;
        }
    )glsl";

};
