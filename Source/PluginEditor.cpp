/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
CtrlkAudioProcessorEditor::CtrlkAudioProcessorEditor (CtrlkAudioProcessor& p, AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), processor (p), valueTreeState(vts)
{
    
    pitchLabel.reset (new Label ("pitchLabel",
                             TRANS("Main")));
    addAndMakeVisible(pitchLabel.get());
    pitchLabel->setFont (Font (Font::getDefaultMonospacedFontName(), 24.00f, Font::plain).withTypefaceStyle ("Bold").withExtraKerningFactor (0.015f));
    pitchLabel->setJustificationType (Justification::centred);
    pitchLabel->setEditable (false, false, false);
    pitchLabel->setColour (Label::backgroundColourId, Colour (0xffdedede));
    pitchLabel->setColour (Label::textColourId, Colour (0xff233034));
    pitchLabel->setColour (TextEditor::textColourId, Colours::black);
    pitchLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    
    mainOsc.reset(new MainOscComponent(p,vts));
    addAndMakeVisible(mainOsc.get());
    
  
    
    
    tapButton.reset(new TextButton("Tap"));
    addAndMakeVisible(tapButton.get());
    tapButton->setLookAndFeel(&blf);
    tapButton->addListener(this);
    

    setSize (600, 400);
    
   

}

CtrlkAudioProcessorEditor::~CtrlkAudioProcessorEditor()
{
    
    
}

//==============================================================================
void CtrlkAudioProcessorEditor::paint (Graphics& g)
{
    
    g.fillAll(Colour::fromRGB(23,30,34));
   
}


void CtrlkAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
  
    if(buttonThatWasClicked == tapButton.get())
    {
        processor.synth->reset();
    }
}


void CtrlkAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto offsetW = proportionOfWidth(0.05);
    auto offsetH = proportionOfHeight(0.1f);
    auto mainOscW = proportionOfWidth(0.5f);
    auto mainOscH = proportionOfHeight(0.75);
    mainOsc.get()->setBounds(offsetW,offsetH,mainOscW,mainOscH);
 
    tapButton->setBounds(proportionOfWidth(0.7f), proportionOfHeight(0.2f), proportionOfWidth(0.1f), proportionOfHeight(0.1f));
  
   

    
    
}
