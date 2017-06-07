//modified by: Alexander Nguyen
//date:06/03/2017
//purpose: To make the particle bounce off the platform
//
//cs3350 Spring 2017 Lab-1
//author: Gordon Griesel
//date: 2014 to present
//This program demonstrates the use of OpenGL and XWindows
//
//Assignment is to modify this program.
//You will follow along with your instructor.
//
//Elements to be learned in this lab...
//
//. general animation framework
//. animation loop
//. object definition and movement
//. collision detection
//. mouse/keyboard interaction
//. object constructor
//. coding style
//. defined constants
//. use of static variables
//. dynamic memory allocation
//. simple opengl components
//. git
//
//elements we will add to program...
//. Game constructor
//. multiple particles
//. gravity
//. collision detection
//. more objects
//
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include "hw1.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define GRAVITY 1.2

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;


int main(void)
{
    int done=0;
    srand(time(NULL));
    initXWindows();
    init_opengl();
    //declare game object
    Game game;
    //declare a box shape
    for (int i = 0; i < 5; i++) {
	game.box[i].width = 100;
	game.box[i].height = 12;
	game.box[i].center.x = 120 + i*65;//box
	game.box[i].center.y = 500 - i*60;
    }

    game.circle.radius = 200;
    game.circle.center.x = 350 + 5*65;
    game.circle.center.y = 250 - 5*60;

    //start animation
    while (!done) {
	while (XPending(dpy)) {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    check_mouse(&e, &game);
	    done = check_keys(&e, &game);
	}
	movement(&game);
	render(&game);
	glXSwapBuffers(dpy, win);
    }
    cleanupXWindows();
    cleanup_fonts();
    return 0;
}

void set_title(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "335 Lab1   LMB for particle");
}

void cleanupXWindows(void)
{
    //do not change
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

void initXWindows(void)
{
    //do not change
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    int w=WINDOW_WIDTH, h=WINDOW_HEIGHT;
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
	std::cout << "\n\tcannot connect to X server\n" << std::endl;
	exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if (vi == NULL) {
	std::cout << "\n\tno appropriate visual found\n" << std::endl;
	exit(EXIT_FAILURE);
    } 
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
	ButtonPress | ButtonReleaseMask | PointerMotionMask |
	StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
	    InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
}

void init_opengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //Set 2D mode (no perspective)
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
    //Set the screen background color
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
}

#define rnd() (float)rand() / (float)RAND_MAX


extern void setVelocity(Particle *p);

void makeParticle(Game *game, int x, int y)
{
    if (game->n >= MAX_PARTICLES)
	return;
    //std::cout << "makeParticle() " << x << " " << y << std::endl;
    //position of particle
    Particle *p = &game->particle[game->n];
    p->s.center.x = x;
    p->s.center.y = y;
    //p->velocity.y = rnd() * 1.0 - 0.5;
    //p->velocity.x = rnd() * 1.0 - 0.5;
    setVelocity(p);
    game->n++;
}

void check_mouse(XEvent *e, Game *game)
{
    static int savex = 0;
    static int savey = 0;
    static int n = 0;

    if (e->type == ButtonRelease) {
	return;
    }
    if (e->type == ButtonPress) {
	if (e->xbutton.button==1) {
	    //Left button was pressed
	    int y = WINDOW_HEIGHT - e->xbutton.y;
	    for (int i = 0; i < 10; i++) {
		makeParticle(game, e->xbutton.x, y);
	    }
	    return;
	}
	if (e->xbutton.button==3) {
	    //Right button was pressed
	    return;
	}
    }
    //Did the mouse move?
    if (savex != e->xbutton.x || savey != e->xbutton.y) {
	savex = e->xbutton.x;
	savey = e->xbutton.y;
	if (++n < 10)
	    return;
	int y = WINDOW_HEIGHT - e->xbutton.y;
	if(game-> bubbler == 0) {
	    game->mouse[0] = savex;
	    game->mouse[1] = WINDOW_HEIGHT - savey;
	}
	for (int i = 0; i < 10; i++) {
	    makeParticle(game, e->xbutton.x,y);
	}


    }
}

int check_keys(XEvent *e, Game *game)
{
    //Was there input from the keyboard?
    if (e->type == KeyPress) {
	int key = XLookupKeysym(&e->xkey, 0);
	if (key == XK_Escape) {
	    return 1;
	}
	//You may check other keys here
	if (key == XK_b) {
	    game->bubbler ^= 1;
	}

    }
    return 0;
}

void movement(Game *game)
{
    Particle *p;
    if (game->bubbler != 0) {
	makeParticle(game, game->mouse[0], game->mouse[1]);
    }

    if (game->n <= 0)
	return;
    if(game->bubbler != 0) {
	makeParticle(game, game->mouse[0], game->mouse[1]);
    }

    for (int i = 0; i < game->n; i++) {
	p = &game->particle[i];
	p->velocity.y -= GRAVITY;
	p->s.center.x += p->velocity.x;
	p->s.center.y += p->velocity.y;

	Shape *s;
	//check for collision with shapes...
	for (int x = 0; x < 5; x++) {
	    s = &game->box[x]; // make shape = to game->box)
	    if (p->s.center.y < s->center.y + s->height &&
		    p->s.center.y > s->center.y - s->height &&
		    p->s.center.x > s->center.x - s->width &&
		    p->s.center.x < s->center.x + s->width ) {
		p->s.center.y = s->center.y + s->height;
		p->velocity.y = -p->velocity.y;// * 0.125; 
		p->velocity.y *= ((rnd()*1.0)*0.125);
		p->velocity.x += ((rnd()*1.0)*0.125);
	    }
	}
	//check for off-screen
	if (p->s.center.y < 0.0) {
	    //std::cout << "off screen" << std::endl;
	    game->particle[i] = game->particle[game->n-1];
	    game->n--;
	}


	float d0, d1, dist;
	d0 = p->s.center.x - game->circle.center.x;
	d1 = p->s.center.y - game->circle.center.y;
	dist = sqrt((d0*d0) + (d1*d1));

	if (dist <= game->circle.radius) {
	    p->s.center.x = game->circle.center.x + 
		(d0/dist)*game->circle.radius*1.01;
	    p->s.center.y = game->circle.center.y + 
		(d1/dist)*game->circle.radius*1.01;

	    p->velocity.x += (d0/dist)*2.25;
	    p->velocity.y += (d1/dist)*2.25;

	}

	if (p->s.center.y < 0.0) {
	    game->particle[i] = game->particle[game->n-1];
	    game->n--;
	}
    }
}


void render(Game *game)
{
    float w, h;
    glClear(GL_COLOR_BUFFER_BIT);
    //Draw shapes...
    const int n = 50;
    static Vec vert[n];
    static bool first = true;
    if(first) {
	float angle = 0.0, inc = (3.14159 * 2.0)/(float)n;
	for (int i = 0; i < n; i++) {
	    vert[i].x=cos(angle)*game->circle.radius;
	    vert[i].y=sin(angle)*game->circle.radius;
	    angle += inc;
	}
	first = false;
    }
    glBegin(GL_LINE_LOOP);
    glColor3ub(90,140,70);
    for (int i = 0; i < n; i++) {
	glVertex2i(game->circle.center.x + vert[i].x, 
		game->circle.center.y + vert[i].y);
    }
    glEnd();
    //draw box
    Shape *s;

    for (int i = 0; i < 5; i++) {
	glColor3ub(70,140,90);
	s = &game->box[i];
	glPushMatrix();
	glTranslatef(s->center.x, s->center.y, s->center.z);
	w = s->width;
	h = s->height;
	glBegin(GL_QUADS);
	glVertex2i(-w,-h);
	glVertex2i(-w, h);
	glVertex2i( w, h);
	glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
    }
    //draw all particles here
    for (int i = 0; i < game->n; i++) {
	glPushMatrix();
	glColor3ub(rand()%250,rand()%250,rand()%250);
	Vec *c = &game->particle[i].s.center;
	w = 2;
	h = 2;
	glBegin(GL_QUADS);
	glVertex2i(c->x-w, c->y-h);
	glVertex2i(c->x-w, c->y+h);
	glVertex2i(c->x+w, c->y+h);
	glVertex2i(c->x+w, c->y-h);
	glEnd();
	glPopMatrix();
    }

    glEnable(GL_TEXTURE_2D);
    Rect rect;

    rect.center = 0;
    rect.left = 0;
    rect.bot = WINDOW_HEIGHT - 20;
    ggprint16(&rect, 16, 0x00ffffff, "Waterfall Model");

    rect.center = 0;
    rect.left = 60;
    rect.bot = 490;
    ggprint16(&rect, 16, 0x00ffffff, "Requirements");

    rect.center = 0;
    rect.left = 150;
    rect.bot = 430;
    ggprint16(&rect, 16, 0x00ffffff, "Design");

    rect.center = 0;
    rect.left = 220;
    rect.bot = 370;
    ggprint16(&rect, 16, 0x00ffffff, "Coding");

    rect.center = 0;
    rect.left = 290;
    rect.bot = 310;
    ggprint16(&rect, 16, 0x00ffffff, "Testing");

    rect.center = 0;
    rect.left = 350;
    rect.bot = 250;
    ggprint16(&rect, 16, 0x00ffffff, "Deliver");

}


