#ifndef _HW1_H
#define _HW1_H

const int MAX_PARTICLES = 100000;

struct Vec {
    float x, y, z;
};

struct Shape {
    float width, height;
    float radius;
    Vec center;
};

struct Particle {
    Shape s;
    Vec velocity;
};

class Game {

    public:
	Shape box[5];
	Particle particle[MAX_PARTICLES];
	int n, bubbler;
	int mouse[2];
	Game() {
	    n = 0, bubbler = 0;
	}
	Shape circle;
};


//Function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_mouse(XEvent *e, Game *game);
int check_keys(XEvent *e, Game *game);
void movement(Game *game);
void render(Game *game);
#endif
