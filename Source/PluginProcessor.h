//
//  pitch-estimator-plugin
//
//  Created by Jacob Møller on 9/06/15.
//
//

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "FastFourierTransformer.h"
#include "HarmonicSummation.h"

//==============================================================================
/**
*/
class PitchestimatorpluginAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    PitchestimatorpluginAudioProcessor();
    ~PitchestimatorpluginAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    int getNumParameters() override;
    float getParameter (int index) override;
    float getParameterDefaultValue (int index) override;
    void setParameter (int index, float newValue) override;

    const String getParameterName (int index) override;
    const String getParameterText (int index) override;

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // =============================================================================
    // For Fourier Transform Class
    FastFourierTransformer* fft;
    fftw_complex*  fftData;
    
    // =============================================================================
    //
    HarmonicSummation* HS;
    float f0Min, f0Max, f0Res, pitchEstimate, gain;
    float* f0Area;
    double* X;
    int f0AreaSize, numberOfHarmonics, nFFT;
    String pitchText;
    
    // =============================================================================
    // List of controls parameters
    enum Parameters
    {
        gainParam,
        harmonicParam,
        totalNumParams
    };
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchestimatorpluginAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
