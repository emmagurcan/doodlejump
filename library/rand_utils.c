#include "rand_utils.h"
#include <time.h>

// random generator: 
double rand_range(double min, double max) {
    return (min + (double) rand()/ ((double) RAND_MAX) * (max-min));
}