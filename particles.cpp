#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "hw1.h"

#define rnd() (float)rand() / (float)RAND_MAX

extern void setVelocity(Particle *p);

void setVelocity(Particle *p) {
	p->velocity.y = rnd() * 1.0 - 0.5;
	p->velocity.x = rnd() * 1.0 - 0.5;
}
