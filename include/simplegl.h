

class SimpleGL {

public:

    /**
     * Create a SimpleGL object
     *
     * @param height The window height in pixels
     * @param width The window width in pixels
     */
    SimpleGL(int width, int height);

    /**
     * Initialize the window
     */
    void init();

    /**
     * Update the window
     */
    void update();

    /**
     * Draw a rectangle
     *
     * @param x, y The location of the top right corner of the rectangle
     * @param width The width of the rectangle
     * @param height The height of the rectangle
     */
    void drawRect(int x, int y, int width, int height);

    /**
     * Draw a line
     *
     * @param x1, y1 Location of first point of line
     * @param x2, y2 Location of second point of line
     */
    void drawLine(int x1, int y1, int x2, int y2);

private:

    int windowWidth_, windowHeight_;


};
