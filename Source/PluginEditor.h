//
//  pitch-estimator-plugin
//
//  Created by Jacob Møller on 9/06/15.
//
//

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class PitchestimatorpluginAudioProcessorEditor  : public AudioProcessorEditor,
                                                  private Slider::Listener,
                                                  public Timer
{
public:
    PitchestimatorpluginAudioProcessorEditor (PitchestimatorpluginAudioProcessor&);
    ~PitchestimatorpluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider* slider) override;
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PitchestimatorpluginAudioProcessor& processor;
    
    Slider sliderHarmonics, sliderGain;
    Label labelPitch, labelHarmonicsKnob, labelGain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchestimatorpluginAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
