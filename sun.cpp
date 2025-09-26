#include <iostream>
#include <cstring>
#include <cmath>
#include <chrono>
#include <thread>

using namespace std;

const int width = 100, height = 60;
const int area = width * height;
char buffer[width * height];
int bgASCIICode = ' ';

// formatting colors
// 30 - black
// 31 - red
// 32 - green
// 33 - yellow
// 34 - blue
// 35 - magenta
// 36 - cyan
// 37 - white
// 39 - default
// 0 - reset all formatting

int setChar(int x, int y, char ch) {
    return buffer[(y * width) + x] = ch;
}

void drawCircle(int x, int y, int radius, char ch) {

    int idx = (y * width) + x;
    buffer[idx] = '$';

    // fill every cell with circle if is in the area
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int xDist = pow(x - i, 2);
            float yDist = pow(y - j, 2) * 4;
            float dist = sqrt(xDist + yDist) * 0.6;
            if (dist <= radius) {
                setChar(i, j, ch);
            }
        }
    }
}

void drawLine(int x1, int y1, int x2, int y2, char ch) {
    for (float percent = 0; percent < 1; percent += 0.05) {
        int _x = (int) x1 + (x2 - x1) * percent * 2;
        int _y = (int) y1 + (y2 - y1) * percent;
        setChar(_x, _y, ch);
    }
}



int main() {

    // clear the screen & hide cursor
    cout << "\033[2J";
    cout << "\033[?25l";

    float rayTwist = 0;

    while (true) {

        // fill terminal canvas with BG char
        memset(buffer, bgASCIICode, area);

        // reset cursor to top-left corner
        cout << "\033[H";

        // draw circle
        int sunX = 30;
        int sunY = 15;
        int sunRadius = 7;
        drawCircle(sunX, sunY, sunRadius, 'O');

        // draw sun rays
        float rayLen = sunRadius * 1.5;
        float plusDegrees = sin(rayTwist / 2) * 20;
        for (int i = 0; i < 8; i++) {
            float degrees = ((360 / 8) * i) + plusDegrees;
            float radians = degrees * (M_PI / 180.0);
            float rayX1 = sunX + 1;
            float rayY1 = sunY + 1;
            float rayX2 = rayX1 + rayLen * cos(radians);
            float rayY2 = rayY1 + rayLen * sin(radians);
            drawLine((int) rayX1, (int) rayY1, (int) rayX2, (int) rayY2, ':');
        }

        // draw cloud
        drawCircle(60, 40, 4, ':');
        drawCircle(70, 35, 5, ':');
        drawCircle(73, 40, 5, ':');
        drawCircle(79, 33, 3, ':');
        drawCircle(80, 40, 6, ':');
        drawCircle(90, 37, 5, ':');
        drawCircle(93, 42, 3, ':');
        
        // print out all chars in buffer
        for (int k = 0; k < area; k++) {

            // change color based on char
            switch (buffer[k]) {
                case '$':
                    cout << "\033[37m"; // yellow
                    break;
                case 'O':
                    cout << "\033[33m"; // yellow
                    break;
                case '@':
                    cout << "\033[33m"; // yellow
                    break;
                default:
                    cout << "\033[0m"; // reset
                    break;
            }

            // print current char
            putchar(k % width ? buffer[k] : 10);
        }

        this_thread::sleep_for(chrono::microseconds(10000));
        rayTwist += 0.1;
    }
}