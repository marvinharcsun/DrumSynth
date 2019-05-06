//
//  Tables.h
//  CTRLK
//
//  Created by Marvin Harootoonyan on 8/28/18.
//  Copyright © 2018 Bzarvin-Records. All rights reserved.
//

#pragma once
#include <stdlib.h>     /* srand, rand */
#include <time.h>


inline float crossfade(double a, double b, double frac) {
    return a + frac * (b - a);
}

/** Linearly interpolate an array `p` with index `x`
 Assumes that the array at `p` is of length at least floor(x)+1.
 */
inline float interpolateLinear(const double *p, double x) {
    int xi = x;
    float xf = x - xi;
    return crossfade(p[xi], p[xi+1], xf);
}

inline void wrapPhase(double a, double b, double &c)
{
    if(c >= a)
    {
        c -= a;
    }
    else if(c <= b)
    {
        c += a;
    }
}

class WhiteNoise{
public:
    WhiteNoise()
    {
        srand(time(NULL));
    }
    
    static double process()
    {
        return (rand()%1000)/1000.0;
    }
    

};




