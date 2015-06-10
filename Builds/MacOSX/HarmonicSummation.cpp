//
//  HarmonicSummation.cpp
//  midi-demo-plugin
//
//  Created by Jacob Møller on 19/05/15.
//
//

#include "HarmonicSummation.h"
#include "math.h"
#include <stdlib.h>

// constructor
HarmonicSummation::HarmonicSummation (int f0AreaSize) {
    Cost = (float *)malloc(f0AreaSize*sizeof(float));
}

// destructor
HarmonicSummation::~HarmonicSummation () {
    free(Cost);
}

void HarmonicSummation::generateCost(double* X, float* f0Area, int L, int bufsize, int f0AreaSize, int fs, int nFFT){
    int fIndex;     // One sample of F0Area
    
    for (int n = 0; n < f0AreaSize; n++)
    {
        fIndex = (int)floor(f0Area[n]*(1.f*nFFT/fs)+1);
        Cost[n] = X[fIndex];

        for (int l = 2; l <= L; l++)
        {
            fIndex = (int)floor(f0Area[n]*l*(1.f*nFFT/fs)+1);
            Cost[n] += X[fIndex];
        }
    }
}

float HarmonicSummation::estimatePitch (float* f0Area, int f0AreaSize) {
    int argument = 0;
    float maxVal = -32000;
    float pitchEstimate;
    
    for (int i = 0; i < f0AreaSize; i++)
    {
        if (maxVal < Cost[i]) {
            maxVal = Cost[i];
            argument = i;
        }
    }
    pitchEstimate = f0Area[argument];

    return pitchEstimate;
}