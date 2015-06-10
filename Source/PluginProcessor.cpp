//
//  pitch-estimator-plugin
//
//  Created by Jacob Møller on 9/06/15.
//
//

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "math.h"
//==============================================================================
PitchestimatorpluginAudioProcessor::PitchestimatorpluginAudioProcessor()
{
    fft = NULL;
    HS = NULL;
}

PitchestimatorpluginAudioProcessor::~PitchestimatorpluginAudioProcessor()
{
    deleteAndZero(fft);
    deleteAndZero(HS);
}

//==============================================================================
const String PitchestimatorpluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int PitchestimatorpluginAudioProcessor::getNumParameters()
{
    return totalNumParams-1;
}

float PitchestimatorpluginAudioProcessor::getParameter (int index)
{
    switch (index)
    {
        case gainParam:     return gain;
        case harmonicParam: return numberOfHarmonics;
        default:            return 0.0f;
    }
}

void PitchestimatorpluginAudioProcessor::setParameter (int index, float newValue)
{
    switch (index)
    {
        case gainParam:     gain = newValue;  break;
        case harmonicParam: numberOfHarmonics = newValue;  break;
        default:            break;
    }
}
float PitchestimatorpluginAudioProcessor::getParameterDefaultValue (int index)
{
    switch (index)
    {
        case gainParam:     return 1.0f;
        case harmonicParam: return 5;
        default:            break;
    }
    
    return 0.0f;
}

const String PitchestimatorpluginAudioProcessor::getParameterName (int index)
{
    switch (index)
    {
        case gainParam:     return "gain";
        case harmonicParam: return "numberOfHarmonics";
        default:            break;
    }
    return String::empty;
    //return String();
}

const String PitchestimatorpluginAudioProcessor::getParameterText (int index)
{
    return String (getParameter (index), 2);
}

const String PitchestimatorpluginAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String PitchestimatorpluginAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool PitchestimatorpluginAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool PitchestimatorpluginAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool PitchestimatorpluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PitchestimatorpluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PitchestimatorpluginAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double PitchestimatorpluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PitchestimatorpluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
}

int PitchestimatorpluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PitchestimatorpluginAudioProcessor::setCurrentProgram (int index)
{
}

const String PitchestimatorpluginAudioProcessor::getProgramName (int index)
{
    return String();
}

void PitchestimatorpluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PitchestimatorpluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    nFFT = pow(2,14);
    numberOfHarmonics = 5;
    gain = 1.0f;
    fftData = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * nFFT);
    X = (double *)malloc(sizeof(double)*nFFT);

    // Create Search Grid
    f0Res=0.05f;
    f0Max=670.0f;
    f0Min=80.0f;
    
    f0AreaSize = round((f0Max-f0Min)/f0Res+1);
    f0Area = (float *)malloc(f0AreaSize*sizeof(float));
    f0Area[0] = f0Min;
    for (int i = 1; i < f0AreaSize; i++) {
        f0Area[i] = f0Area[i-1] + f0Res;
    }
    
    if(fft == NULL)
    {
        fft = new FastFourierTransformer(nFFT);
        HS = new HarmonicSummation(f0AreaSize);
    }
}

void PitchestimatorpluginAudioProcessor::releaseResources()
{
    deleteAndZero(fft);
    deleteAndZero(HS);
    free(X);
    free(f0Area);
    fftw_free(fftData);
}

void PitchestimatorpluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
 
    int bufsize = buffer.getNumSamples();
    
    //main process loop
    for (int channel = 0; channel < getNumInputChannels(); ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
        fft->processForward(channelData, fftData, bufsize, nFFT);
        buffer.applyGain (channel, 0, bufsize, gain);
    }
    for (int i=0; i<bufsize; i++) {
        X[i] = fft->cartopolRadius(fftData[i][0], fftData[i][1]);
    }
    
    HS->generateCost(X, f0Area, numberOfHarmonics, bufsize, f0AreaSize, getSampleRate(), nFFT);
    pitchEstimate = HS->estimatePitch(f0Area, f0AreaSize);
    
    pitchText = String (pitchEstimate, 1);

}

//==============================================================================
bool PitchestimatorpluginAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* PitchestimatorpluginAudioProcessor::createEditor()
{
    return new PitchestimatorpluginAudioProcessorEditor (*this);
}

//==============================================================================
void PitchestimatorpluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
}

void PitchestimatorpluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PitchestimatorpluginAudioProcessor();
}
