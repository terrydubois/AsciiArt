#include <iostream>
#include <cstring>
#include <cmath>
#include <chrono>
#include <thread>
#include <vector>
#include <cstdlib>
#include <random>

using namespace std;

const int width = 100, height = 60;
const int area = width * height;
char buffer[area];
int bgASCIICode = ' ';
float rayTwist = 0;
int birdFrame = 0;
int particleFrame = 0;

// create random seed
random_device rd;
mt19937 gen(rd());

// distribution for random range
uniform_int_distribution<> distrib(0, 1);

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

// set char in buffer
void setChar(int x, int y, char ch) {
    int idx = (y * width) + x;
    if (idx >= 0 && idx < area) {
        buffer[idx] = ch;
    }
}

// convert degrees to radians
float degToRad(float deg) {
    return deg * (M_PI / 180.0);
}

void drawCircle(int x, int y, int radius, char ch1, char ch2) {

    int idx = (y * width) + x;
    buffer[idx] = '$';

    // fill every cell with circle if is in the area
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int xDist = pow(x - i, 2);
            float yDist = pow(y - j, 2) * 4;
            float dist = sqrt(xDist + yDist) * 0.6;
            if (dist <= radius) {
                int rando = distrib(gen);
                setChar(i, j, rando == 0 ? ch1 : ch2);
            }
        }
    }
}

void drawLine(int x1, int y1, int x2, int y2, char ch1, char ch2) {
    for (float percent = 0; percent < 1; percent += 0.05) {
        int _x = (int) x1 + (x2 - x1) * percent * 2;
        int _y = (int) y1 + (y2 - y1) * percent;
        int rando = distrib(gen);
        setChar(_x, _y, rando == 0 ? ch1 : ch2);
    }
}


void drawArc(int xCenter, int yCenter, float radius, float radiusRatio, float deg1, float deg2, char ch1, char ch2) {

    // draw points around circle
    for (float i = deg1; i < deg2; i++) {
        float radians = degToRad(i);
        float _x = xCenter + radius * radiusRatio * cos(radians);
        float _y = yCenter + radius * sin(radians);
        int rando = distrib(gen);
        setChar(_x, _y, rando == 0 ? ch1 : ch2);
    }

}


class Raindrop {

public:

    // Raindrop will have x1 and y1, and a tail that extends
    // backwards with length len
    float x, y;
    float xOrigin, yOrigin;
    float len, spd;

    // constructor
    Raindrop(float _x, float _y, float _len, float _spd)
        : x(_x), y(_y), xOrigin(_x), yOrigin(_y),
        len(_len), spd(_spd) {}

    void updatePos() {
        x -= spd;
        y += spd;

        // reset position when going off-screen
        if (y - len > height) {
            x = xOrigin;
            y = yOrigin;
        }
    }

    void drawSelf() {
        for (int i = 0; i < len; i++) {
            setChar(x + i, y - i, '/');
        }
    }
};

class Bird {

public:

    float x, y;
    float radius;
    float spd;

    // constructor
    Bird(float _x, float _y, float _radius, float _spd)
        : x(_x), y(_y), radius(_radius), spd(_spd) {}

    void updatePos() {
        x += spd;
    }

    void drawSelf() {
        float radiusRatio = 3;
        if (birdFrame == 0) {
            drawArc(x - radius * 2, y, radius, radiusRatio, 180, 360, 'V', 'V');
            drawArc(x + radius * 2, y, radius, radiusRatio, 180, 360, 'V', 'V');
        }
        else if (birdFrame == 1 || birdFrame == 3) {
            drawArc(x - radius * 2 * 1.5, y, radius * 1.5, radiusRatio, 260, 360, 'V', 'V');
            drawArc(x + radius * 2 * 1.5, y, radius * 1.5, radiusRatio, 180, 280, 'V', 'V');
        }
        else if (birdFrame == 2) {
            drawArc(x - radius * 2 * 2.5, y, radius * 2.5, radiusRatio, 300, 360, 'V', 'V');
            drawArc(x + radius * 2 * 2.5, y, radius * 2.5, radiusRatio, 180, 240, 'V', 'V');
        }
    }

};

class Particle {

public:
    float deg, len;

    // constructor
    Particle()
        : deg(0), len(0) {}

    void drawSelf(int sunX, int sunY) {
        int rando = distrib(gen);
        float radians = degToRad(deg);
        float x = sunX + len * 1.5 * cos(radians);
        float y = sunY + len * sin(radians);
        setChar(x, y, rando == 0 ? '*': '%');
    }

};



int main() {

    // clear the screen & hide cursor
    cout << "\033[2J";
    cout << "\033[?25l";

    srandom(static_cast<unsigned int>(time(0)));

    // create raindrops
    vector<Raindrop*> drops = {
        new Raindrop{65, 40, 3, 2},
        new Raindrop{75, 36, 4, 2},
        new Raindrop{78, 38, 5, 2},
        new Raindrop{82, 38, 3, 2},
        new Raindrop{88, 36, 3, 2},
        new Raindrop{90, 40, 4, 2},
        new Raindrop{93, 39, 3, 2}
    };

    // create particles
    vector<Particle*> particles = {
        new Particle,
        new Particle,
        new Particle,
        new Particle,
        new Particle,
        new Particle,
        new Particle,
        new Particle,
        new Particle,
        new Particle,
        new Particle
    };


    while (true) {

        // fill terminal canvas with BG char
        memset(buffer, bgASCIICode, area);

        // reset cursor to top-left corner
        cout << "\033[H";

        // draw circle
        int sunX = 30;
        int sunY = 15;
        int sunRadius = 7;
        drawCircle(sunX, sunY, sunRadius, '*', '%');

        // draw sun rays
        float rayLen = sunRadius * 1.5;
        float plusDegrees = sin(rayTwist / 2) * 20;
        for (int i = 0; i < 8; i++) {
            float deg = ((360 / 8) * i) + plusDegrees;
            float radians = degToRad(deg);
            float rayX1 = sunX + 1;
            float rayY1 = sunY + 1;
            float rayX2 = rayX1 + rayLen * cos(radians);
            float rayY2 = rayY1 + rayLen * sin(radians);
            drawLine(rayX1, rayY1, rayX2, rayY2, '*', '%');
        }

        // draw particles
        for (Particle* particle : particles) {
            particle->drawSelf(sunX, sunY);
        }

        // draw raindrops
        for (Raindrop* drop : drops) {
            drop->updatePos();
            drop->drawSelf();
        }

        // draw cloud
        drawCircle(60, 40, 4, ':', '.');
        drawCircle(70, 35, 5, ':', '.');
        drawCircle(73, 40, 5, ':', '.');
        drawCircle(79, 33, 3, ':', '.');
        drawCircle(80, 40, 6, ':', '.');
        drawCircle(90, 37, 5, ':', '.');
        drawCircle(93, 42, 3, ':', '.');

        // draw rainbow
        int minRadius = 21;
        drawArc(width * 0.85, height * 0.75, minRadius, 2, 190, 280, '?', 'Z');
        drawArc(width * 0.85, height * 0.75, minRadius + 1, 2, 190, 280, '@', 'X');
        drawArc(width * 0.85, height * 0.75, minRadius + 2, 2, 190, 280, '$', 'C');
        drawArc(width * 0.85, height * 0.75, minRadius + 3, 2, 190, 280, '%', 'A');
        drawArc(width * 0.85, height * 0.75, minRadius + 4, 2, 190, 280, '&', 'B');

        // draw rainbow cloud top
        drawCircle(90, 20, 2, ':', ';');
        drawCircle(94, 22, 3, ':', ';');
        drawCircle(90, 23, 3, ':', ';');

        // draw rainbow cloud bottom
        drawCircle(34, 40, 2, ':', ';');
        drawCircle(38, 42, 3, ':', ';');
        drawCircle(42, 41, 2, ':', ';');
        
        // print out all chars in buffer
        for (int k = 0; k < area; k++) {

            // change color based on char
            switch (buffer[k]) {
                case '*':
                    cout << "\033[33m"; // yellow
                    break;
                case 'O':
                    cout << "\033[33m"; // yellow
                    break;
                case '/':
                    cout << "\033[36m"; // cyan
                    break;
                case '&':
                    cout << "\033[31m"; // red
                    break;
                case 'B':
                    cout << "\033[31m"; // red
                    break;
                case '%':
                    cout << "\033[33m"; // yellow
                    break;
                case 'A':
                    cout << "\033[33m"; // yellow
                    break;
                case '6':
                    cout << "\033[33m"; // yellow
                    break;
                case '$':
                    cout << "\033[32m"; // green
                    break;
                case 'C':
                    cout << "\033[32m"; // green
                    break;
                case '@':
                    cout << "\033[36m"; // cyan
                    break;
                case 'X':
                    cout << "\033[36m"; // cyan
                    break;
                case '?':
                    cout << "\033[35m"; // magenta
                    break;
                case 'Z':
                    cout << "\033[35m"; // magenta
                    break;
                default:
                    cout << "\033[0m"; // reset
                    break;
            }

            // print current char
            putchar(k % width ? buffer[k] : 10);
        }

        this_thread::sleep_for(chrono::microseconds(10000));
        birdFrame++;
        if (birdFrame > 3) birdFrame = 0;
        particleFrame++;
        if (particleFrame > 5) {
            particleFrame = 0;
            for (Particle* particle : particles) {
                particle->deg = rand() % 360;
                particle->len = sunRadius + (rand() % sunRadius * 1);
            }
        }
        rayTwist += 0.1;
    }
}