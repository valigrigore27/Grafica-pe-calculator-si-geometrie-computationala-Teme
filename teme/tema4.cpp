#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <map>


#define PI 3.1415926535897932384626433832795
using namespace std;
unsigned char prevKey;

struct x_point {
    int rows_x, cols_x;
};


class CartesianGrid {
public:
    int rows, cols, rows2, cols2;
    double init_x, init_y, row_distance, col_distance, c, e, eps, circle, rad, x_og, y_og;
    bool ok;

    CartesianGrid(int rows, int cols, double eps) {
        this->rows = rows;
        this->cols = cols;
        this->eps = 0.1;
        this->circle = circle;
        this->init_x = -1 + this->eps;
        this->init_y = -1 + this->eps;
        this->ok = ok;
        this->c = (2 - 2 * this->eps) / (cols - 1);
        this->e = (2 - 2 * this->eps) / (rows - 1);

        this->rad = this->c / 3;
        this->drawCols();
        this->drawLines();
    }

    CartesianGrid(int l, int c, bool pc = true) {
        this->rows2 = l;
        this->cols2 = c;
        this->eps = eps;
        this->rad = rad;
        this->row_distance = 1.6 / rows2;
        this->col_distance = 1.6 / cols2;
        this->c = (2 - 2 * this->eps) / (cols - 1);
        this->e = (2 - 2 * this->eps) / (rows - 1);
        this->ok = pc;

        this->x_og = (!ok) ? (-0.8 + rows2 / 2 * row_distance) : -0.8;
        this->y_og = (!ok) ? (-0.8 + cols2 / 2 * row_distance) : -0.8;
    }

    void drawLines() {
        double d_lines = 0.0;
        double d_cols = 0.0;
        glColor3f(0.0, 0.0, 0.0);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        double pixel1_x = this->init_x;
        d_lines = this->init_x;
        double pixel_y = this->init_y;
        d_cols = this->init_y;
        double pixel2_x = -this->init_x;
        int j = 0;

        for (int i = 1; i <= this->rows; i++) {
            j++;
            glVertex2f(pixel1_x, pixel_y);
            glVertex2f(pixel2_x, pixel_y);
            pixel_y += this->e;
            d_cols += this->e;
        }

        glEnd();
    }


    void drawCols() {
        glColor3f(0.0, 0.0, 0.0);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        double pixel_x = this->init_x;
        double pixel1_y = this->init_y;
        double pixel2_y = 1 - this->eps;

        for (int i = 1; i <= this->cols; i++) {
            glVertex2f(pixel_x, pixel1_y);
            glVertex2f(pixel_x, pixel2_y);
            pixel_x += this->c;
        }

        glEnd();
    }

    void writePixel(int i, int j) {
        double x = this->init_x + i * this->c;
        double y = this->init_y + j * this->e;
        drawCircle(x, y, rad, 10000);
    }


    void showRightSegment(int startX, int startY, int endX, int endY, bool okk) {
        int deltaX = abs(endX - startX), deltaY = abs(endY - startY);
        int stepX = (startX < endX) ? 1 : -1;
        int stepY = (startY < endY) ? 1 : -1;
        int error = deltaX - deltaY;

        while (true) {
            if (startX >= 0 && startX < this->cols && startY >= 0 && startY < this->rows) {
                writePixel(startX, startY);
            }

            if (startX == endX && startY == endY) {
                break;
            }

            int error2 = 2 * error;

            if (error2 > -deltaY) {
                error -= deltaY;
                startX += stepX;
            }

            if (error2 < deltaX) {
                error += deltaX;
                startY += stepY;
            }
        }
    }

    void drawCircle(double centerX, double centerY, double radius, int numSegments) {
        glColor3ub(71, 71, 71);
        double rows_x, rows_y;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(centerX, centerY);

        for (int i = 0; i <= numSegments; i++) {
            float angle = i * 2.0f * PI / numSegments;
            float offsetX = radius * cos(angle);
            float offsetY = radius * sin(angle);

            glVertex2f(centerX + offsetX, centerY + offsetY);
        }

        glEnd();
    }

    void showLeftSegment(int x0, int y0, int x_param, int y_param) {
        glColor3f(1.0, 0.1, 0.1);
        glLineWidth(1.0f);
        glBegin(GL_LINES);

        double startX = init_x + x0 * c;
        double endX = init_x + x_param * c;
        double param_x = init_x + x_param;
        double param_y = init_y + y_param;
        double startY = init_y + y0 * e;
        double endY = init_y + y_param * e;

        glVertex2f(startX, startY);
        glVertex2f(endX, endY);

        glEnd();
    }



    void drawGrid_2() {
        glLineWidth(1);
        glColor3f(0.1, 0.1, 0.1);
        double row_x = 0.2 + row_distance;
        double row_y = 0.1 + row_distance;

        for (int i = 0; i <= rows2; ++i) {
            glBegin(GL_LINES);
            row_x++;
            row_y++;
            glVertex2d(-0.8, -0.8 + row_distance * i);
            glVertex2d(0.8, -0.8 + row_distance * i);
            glEnd();
        }

        for (int i = 0; i <= cols2; ++i) {
            glBegin(GL_LINES);
            glVertex2d(-0.8 + row_distance * i, -0.8);
            glVertex2d(-0.8 + row_distance * i, 0.8);
            row_x++;
            row_y++;
            glEnd();

        }
    }

    bool isPixelOk(int i, int j) {
        if (ok) { // se afla in primul cadran
            return (i >= 0 && i < rows2&& j >= 0 && j < cols2);
        }
        else { // se afla in al doilea cadran
            int xmin = -rows2 / 2;
            int xmax = rows2 / 2;
            int ymin = -cols2 / 2;
            int ymax = cols2 / 2;
            return (i >= xmin && i <= xmax && j >= ymin && j <= ymax);
        }
    }

    void addPixel(int i, int j) {
        if (isPixelOk(i, j) == false) {
            return;
        }

        float _i = x_og + row_distance * i;
        float _j = y_og + col_distance * j;

        float radius = 0.02;

        glPushMatrix(); // salvăm matricea curentă
        glTranslatef(_i, _j, 0.0); // translatăm la poziția pixelului
        glColor3f(0.1, 0.1, 0.1);
        glutSolidSphere(radius, 20, 20); // desenăm o sferă cu raza și detaliile date
        glPopMatrix(); // revenim la matricea anterioară
    }


    void drawCircle2(int radiusX, int radiusY) {
        const float DEG2RAD = 3.14159 / 180;
        glColor3f(1.0, 0.1, 0.1);
        bool isOk;
        glLineWidth(4);
        glBegin(GL_LINE_STRIP);
        int rad_c;
        isOk = true;

        rad_c = 0;
        for (int i = 0; i <= 360; i += 10) {
            float degInRad = i * DEG2RAD;
            double ii = cos(degInRad) * row_distance * radiusX;
            rad_c += radiusX;
            double jj = sin(degInRad) * col_distance * radiusY;
            rad_c += radiusY;
            if (ok && (ii < 0 || jj < 0)) {
                glEnd();
                isOk = false;
                rad_c = 0;
                return;
            }
            glVertex2f(ii + x_og, jj + y_og);
        }
        glEnd();
    }



    void Orizontally(int i, int j, int width) {
        int start = i - width / 2;
        int c_width;
        if (width % 2 == 0) {
            start += 1;
            c_width = 0;
        }
        int end = start + width - 1;

        for (int x = start; x <= end; x++) {
            addPixel(x, j);
        }
    }

    void printCircle2(int raza, int big) {
        int x = 0, y = raza;
        int d = 3 - 2 * raza;

        Orizontally(y, x, 3);

        while (x < y) {
            if (d < 0) {
                d += 4 * x + 6;
            }
            else {
                d += 4 * (x - y) + 10;
                y--;
            }
            x++;
            Orizontally(y, x, 3);
        }
    }
};

void Init(void) {
    int x_point;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLineWidth(1);
    glPointSize(4);
    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display1() {
    
    CartesianGrid* cartesianGrid = new CartesianGrid(16, 16, 0.1);

    cartesianGrid->showRightSegment(0, 15, 15, 10, true);
    cartesianGrid->showRightSegment(0, 14, 15, 9, true);
    cartesianGrid->showRightSegment(0, 16, 15, 11, true);
    cartesianGrid->showLeftSegment(0, 15, 15, 10);



    cartesianGrid->showRightSegment(0, 0, 15, 7, true);
    cartesianGrid->showLeftSegment(0, 15, 15, 10);
    cartesianGrid->showLeftSegment(0, 0, 15, 7);
}

void Display2() {

    CartesianGrid* cartesianGrid2 = new CartesianGrid(16, 16, true);
    glPushMatrix();
    glLoadIdentity();
    cartesianGrid2->drawGrid_2();
    cartesianGrid2->addPixel(0, 0);
    cartesianGrid2->drawCircle2(13, 13);
    cartesianGrid2->addPixel(-1, -1);
    cartesianGrid2->printCircle2(13, 3);

    glPopMatrix();
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (prevKey) {
    case '1':
        Display1();
        break;
    case '2':
        Display2();
        break;
    default:
        break;
    }

    glFlush();
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27) // escape
        exit(0);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutCreateWindow(argv[0]);

    Init();
    glutKeyboardFunc(KeyboardFunc);
    glutDisplayFunc(Display);
    glutMainLoop();

    return 0;
}




