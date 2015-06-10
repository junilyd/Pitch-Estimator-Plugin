//
//  HarmonicSummation.h
//  midi-demo-plugin
//
//  Created by Jacob Møller on 19/05/15.
//
//

#ifndef __midi_demo_plugin__HarmonicSummation__
#define __midi_demo_plugin__HarmonicSummation__

#include <stdio.h>

#endif /* defined(__midi_demo_plugin__HarmonicSummation__) */

class HarmonicSummation {
    
public:
    
    HarmonicSummation (int f0AreaSize);
    ~HarmonicSummation();
    
    // Find Arg Max
    float estimatePitch     (float* f0Area, int f0AreaSize);
    void generateCost       (double* X, float* f0Area, int L, int bufsize, int f0AreaSize, int fs, int nFFT);

private:
    float* Cost;
    float *HS;
};