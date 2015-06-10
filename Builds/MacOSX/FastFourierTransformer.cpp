//
//    FastFourierTransformer.h
//    Created by Jacob Møller 18 May 2015
//
//

#include "fftw3.h"
#include "FastFourierTransformer.h"

//--------------------------------------------------------------
//        fftw3 class implementation
//--------------------------------------------------------------

// constructor

FastFourierTransformer::FastFourierTransformer (int nFFT) {
	
	data          = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * nFFT);
	fft_result    = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * nFFT);
	ifft_result   = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * nFFT);
	plan_forward  = fftw_plan_dft_1d(nFFT, data, fft_result, FFTW_FORWARD, FFTW_MEASURE);
	plan_backward = fftw_plan_dft_1d(nFFT, data, ifft_result, FFTW_BACKWARD, FFTW_MEASURE);	
	
}

//-------------------------------------------------------------

// destructor

FastFourierTransformer::~FastFourierTransformer() {
	
	fftw_free(data);
	fftw_free(fft_result);
	fftw_free(ifft_result);
		
	fftw_destroy_plan(plan_forward);	
	fftw_destroy_plan(plan_backward);
	
}

//--------------------------------------------------------------

//polar to cartesian conversion functions

double FastFourierTransformer::poltocarX (double angle, double radius) { 
	
	return cos(angle) * radius;
}

//--------------------------------------------------------------

double FastFourierTransformer::poltocarY (double angle, double radius) {
	
	return sin(angle) * radius ;	
}

//--------------------------------------------------------------

//cartesian to polar conversion functions

double FastFourierTransformer::cartopolRadius (double x, double y) {
	
	return sqrt(y * y + x * x);	
}

//--------------------------------------------------------------

double FastFourierTransformer::cartopolAngle (double x, double y)  { 
	
    if (x > 0) { return atan(y/x); }
	if (x < 0 && y >= 0) {return atan(y/x) + M_PI; }
	if (x < 0 && y < 0) {return atan(y/x) - M_PI; }
	if (y > 0) { return M_PI / 2; }
	if (y < 0) { return -M_PI / 2; }
	
	return 0;	
}

//--------------------------------------------------------------

// fft conversion method

void FastFourierTransformer::processForward (const float* channelData, fftw_complex* fftData, int bufSize, int nFFT) {
	
	for(i = 0; i < bufSize; i++) {
		
		data[i][0] = channelData[i];
		data[i][1] = 0.0;
	}
	fftw_execute(plan_forward);

    for (i = 0; i < nFFT; i++) {
		fftData[i][0] = fft_result[i][0];
		fftData[i][1] = fft_result[i][1];				
	}	
}

//--------------------------------------------------------------

// inverse fft conversion method

void FastFourierTransformer::processBackward (fftw_complex* fftData, float* channelData, int bufSize, int nFFT) {
			
	for(i = 0; i < bufSize; i++) {
		
		data[i][0] = fftData[i][0];
        data[i][1] = fftData[i][1];
	}
	fftw_execute(plan_backward);
		
	for(i = 0; i < nFFT; i++) {
	
		channelData[i] = ifft_result[i][0] / nFFT;
	}		
}

//--------------------------------------------------------------

// fft gain control function

void FastFourierTransformer::gainFFT (fftw_complex* fftData, float oldGain, float gainStep, int bufSize) {
		
	float Pmagnitude[bufSize];			//local arrays for storing polar coordinates
	float Pangle[bufSize];
		
	for(int i = 0; i < bufSize; i++) {
		
		//cartesian to polar conversion
		
		Pmagnitude[i] = this->cartopolRadius(fftData[i][0], fftData[i][1]);
		Pangle[i]	  =	this->cartopolAngle(fftData[i][0], fftData[i][1]);
		
		//change polar magnitude
		
		oldGain += gainStep;
		Pmagnitude[i] = Pmagnitude[i] * oldGain ;	
		
		//polar to cartesian conversion
		
		fftData[i][0] = this->poltocarX(Pangle[i], Pmagnitude[i]);
		fftData[i][1] = this->poltocarY(Pangle[i], Pmagnitude[i]);
		
	}	
	
}