//
//  pitch-estimator-plugin
//
//  Created by Jacob Møller on 9/06/15.
//
//

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
PitchestimatorpluginAudioProcessorEditor::PitchestimatorpluginAudioProcessorEditor (PitchestimatorpluginAudioProcessor& p)
    : AudioProcessorEditor (&p),
      processor (p),
      labelHarmonicsKnob("","Number Of Harmonics"),
      labelGain ("", "Throughput level:")

{
    setSize (400, 300);

    sliderHarmonics.setSliderStyle (Slider::Rotary);
    sliderHarmonics.setRange(1, 15, 1);
    sliderHarmonics.setValue(5);
    
    sliderGain.setSliderStyle (Slider::Rotary);
    sliderGain.setRange (0.0f, 1.0f, 0.01f);
    sliderGain.setValue(1.0f);
    
    labelHarmonicsKnob.attachToComponent (&sliderHarmonics, false);
    labelHarmonicsKnob.setFont (Font (15.0f));
    
    labelPitch.setBounds(120, 120, 200, 200);
    labelPitch.setText(processor.pitchText, sendNotification);
    labelPitch.setFont(60.0f);
    
    labelGain.attachToComponent (&sliderGain, false);
    labelGain.setFont (Font (15.0f));
    
    sliderHarmonics.addListener(this);
    sliderGain.addListener (this);

    // this function adds the slider to the editor
    addAndMakeVisible (&sliderHarmonics);
    addAndMakeVisible(&labelPitch);
    addAndMakeVisible (&sliderGain);
    
    startTimer (150);
}

PitchestimatorpluginAudioProcessorEditor::~PitchestimatorpluginAudioProcessorEditor()
{
}

//==============================================================================
void PitchestimatorpluginAudioProcessorEditor::paint (Graphics& g)
{
    g.setGradientFill (ColourGradient (Colours::white, 0, 0,
                                       Colours::grey, 0, (float) getHeight(), false));
    g.fillAll();
    sliderHarmonics.setBounds (200, 60, 150, 40);

}

void PitchestimatorpluginAudioProcessorEditor::resized()
{
    // Lay out the positions of any subcomponents in the editor.
    sliderHarmonics.setBounds (180, 30, 20, getHeight() - 200);
    sliderGain.setBounds (20, 60, 150, 40);


}
void PitchestimatorpluginAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    processor.numberOfHarmonics = sliderHarmonics.getValue();
    processor.gain = sliderGain.getValue();
}
void PitchestimatorpluginAudioProcessorEditor::timerCallback()
{
    labelPitch.setText(processor.pitchText, sendNotification);
}