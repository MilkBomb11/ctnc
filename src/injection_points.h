#ifndef INJECTION_POINTS_H
#define INJECTION_POINTS_H

#include <stddef.h>

typedef struct InjectionPoints InjectionPoints;

struct InjectionPoints {
    size_t front;    
    size_t back;
};

#endif
