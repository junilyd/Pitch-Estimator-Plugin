//
//    FastFourierTransformer.h
//    Created by Jacob Møller 02 May 2015
//
//

#define USE_MATH_DEFINES

#include "fftw3.h"
#include "math.h"

#ifndef __FASTFOURIERTRANSFORMER_H_9F2E8A4F__
#define __FASTFOURIERTRANSFORMER_H_9F2E8A4F__

// fft class

class FastFourierTransformer {

// class methods
	
public:
	
	FastFourierTransformer (int bufSize);	//constructor
	~FastFourierTransformer();				//destructor
	
	//convert coordinates 
	
	double poltocarX	   (double angle, double radius);		
	double poltocarY	   (double angle, double radius);
	double cartopolAngle   (double Cx, double Cy);
	double cartopolRadius  (double Cx, double Cy);
	
	//fft conversion
	
	void processForward	   (const float* channelData, fftw_complex* fftData, int bufSize, int nFFT);
	void processBackward   (fftw_complex* fftData, float* channelData, int bufSize, int nFFT);
	
	//fft gain
	
	void gainFFT		   (fftw_complex* fftData, float oldGain, float gainStep, int bufSize);
	
						
// member variables
	
private:
	
	fftw_complex    *data, *fft_result, *ifft_result;	
	fftw_plan       plan_forward, plan_backward;
	float			*Pangle, *Pmagnitude;
	int             i;
	
};


#endif  // __FASTFOURIERTRANSFORMER_H_9F2E8A4F__
