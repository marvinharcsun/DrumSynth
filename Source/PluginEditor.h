/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
#include "PluginProcessor.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class MyKnobLF : public LookAndFeel_V3
{
public:
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
    {
        const float radius = jmin (width / 2, height / 2) - 4.0f;
        const float centreX = x + width * 0.5f;
        const float centreY = y + height * 0.5f;
        const float rx = centreX - radius;
        const float ry = centreY - radius;
        const float rw = radius * 2.0f;
        const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        
        // fill
        

        g.setColour (Colour::fromRGB(23,30,34));
        g.fillEllipse (rx, ry, rw, rw);
        
       
        
        // outline
        g.setColour (Colour::fromRGB(222,222,222));
        g.drawEllipse (rx, ry, rw, rw, 4.0f);
        
        Path p;
        const float pointerLength = radius * 0.5f;
        const float pointerThickness = 4.0f;
        p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform (AffineTransform::rotation (angle).translated (centreX, centreY));
        
        // pointer
        g.setColour (Colour::fromRGB(222,222,222));
        g.fillPath (p);
        
        g.setColour(Colour::fromRGB(222,222,222));
        g.drawText(textLabel, *new Rectangle<float>(rx,y,rw,rw), Justification::centred);
        
        
        
        
    }
    
    juce::String textLabel;
    
};

class MyButtonLF : public LookAndFeel_V3
{
public:
    
    
   
    
    void drawButtonText (Graphics& g, TextButton& button, bool isMouseOverButton, bool isButtonDown) override
    {
    
        auto font = getTextButtonFont (button, button.getHeight());
        g.setFont (Font (Font::getDefaultMonospacedFontName(), 18.00f, Font::plain).withTypefaceStyle ("Bold").withExtraKerningFactor (0.0f));
        g.setColour (button.findColour (button.getToggleState() ? TextButton::textColourOnId
                                        : TextButton::textColourOffId)
                     .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));
        auto yIndent = jmin (4, button.proportionOfHeight (0.3f));
        auto cornerSize = jmin (button.getHeight(), button.getWidth()) / 2;
        auto fontHeight = roundToInt (font.getHeight() * 0.6f);
        auto leftIndent  = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft()  ? 4 : 2));
        auto rightIndent = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
        auto textWidth = button.getWidth() - leftIndent - rightIndent;
        if (textWidth > 0)
            g.drawFittedText (button.getButtonText(),
                              leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
                              Justification::centred, 2);
    }
    
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override
    {
        auto buttonArea = button.getLocalBounds();
        g.setColour (Colour(0xffdedede));
        if(isButtonDown)
        {
            g.setColour(Colour(0xffDF4C51));
        }
        int dif = abs(buttonArea.getHeight() - buttonArea.getWidth())*.5;
        g.fillRect (dif+buttonArea.getX(),buttonArea.getY(),buttonArea.getHeight(),buttonArea.getHeight());
        
    }
};
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
struct SliderWithAttachment
{
    ScopedPointer <Slider> slider;
    ScopedPointer <SliderAttachment> attachment;
};


class MainOscComponent  : public Component
{
public:
    //==============================================================================
    MainOscComponent (CtrlkAudioProcessor& parent, AudioProcessorValueTreeState &vts)
    :processor(parent), valueTreeState(vts)
    {
        
        mainLabel.reset (new Label ("mainLabel",
                                     TRANS("Main")));
        addAndMakeVisible(mainLabel.get());
        mainLabel->setFont (Font (Font::getDefaultMonospacedFontName(), 18.00f, Font::plain).withTypefaceStyle ("Bold").withExtraKerningFactor (0.015f));
        mainLabel->setJustificationType (Justification::centred);
        mainLabel->setEditable (false, false, false);
        mainLabel->setColour (Label::backgroundColourId, Colour (0xffdedede));
        mainLabel->setColour (Label::textColourId, Colour (0xff233034));
        mainLabel->setColour (TextEditor::textColourId, Colours::black);
        mainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        
        startLabel.reset (new Label ("startLabel",
                                     TRANS("Start")));
        addAndMakeVisible(startLabel.get());
        startLabel->setFont (Font (Font::getDefaultMonospacedFontName(), 11.00f, Font::plain).withTypefaceStyle ("Bold").withExtraKerningFactor (0.015f));
        startLabel->setJustificationType (Justification::centred);
        startLabel->setEditable (false, false, false);
        startLabel->setColour (Label::backgroundColourId, Colour (0xffdedede));
        startLabel->setColour (Label::textColourId, Colour (0xff233034));
        startLabel->setColour (TextEditor::textColourId, Colours::black);
        startLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        
        
        startFreq.slider = new Slider();
        addAndMakeVisible (startFreq.slider);
        startFreq.slider.get()->setSliderStyle (Slider::RotaryVerticalDrag);
        startFreq.slider.get()->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
        startFreq.slider.get()->setLookAndFeel(&lf);
        startFreq.attachment.reset (new SliderAttachment(valueTreeState,"startFreq",*startFreq.slider));
        
        
        
        endFreq.slider = new Slider();
        addAndMakeVisible (endFreq.slider);
        endFreq.slider.get()->setSliderStyle (Slider::RotaryVerticalDrag);
        endFreq.slider.get()->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
        endFreq.slider.get()->setLookAndFeel(&lf);
        endFreq.attachment.reset (new SliderAttachment(valueTreeState,"endFreq",*endFreq.slider));
        
        endLabel.reset (new Label ("endLabel",
                                     TRANS("End")));
        addAndMakeVisible(endLabel.get());
        endLabel->setFont (Font (Font::getDefaultMonospacedFontName(), 11.00f, Font::plain).withTypefaceStyle ("Bold").withExtraKerningFactor (0.015f));
        endLabel->setJustificationType (Justification::centred);
        endLabel->setEditable (false, false, false);
        endLabel->setColour (Label::backgroundColourId, Colour (0xffdedede));
        endLabel->setColour (Label::textColourId, Colour (0xff233034));
        endLabel->setColour (TextEditor::textColourId, Colours::black);
        endLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        
        
        pitchDecay.slider = new Slider();
        addAndMakeVisible (pitchDecay.slider);
        pitchDecay.slider.get()->setSliderStyle (Slider::RotaryVerticalDrag);
        pitchDecay.slider.get()->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
        pitchDecay.slider.get()->setLookAndFeel(&lf);
        pitchDecay.attachment.reset (new SliderAttachment(valueTreeState,"pitchDecay",*pitchDecay.slider));
        
        
        pitchDecayLabel.reset (new Label ("pitchDecayLabel",
                                     TRANS("P Dec.")));
        addAndMakeVisible(pitchDecayLabel.get());
        pitchDecayLabel->setFont (Font (Font::getDefaultMonospacedFontName(), 11.00f, Font::plain).withTypefaceStyle ("Bold").withExtraKerningFactor (0.015f));
        pitchDecayLabel->setJustificationType (Justification::centred);
        pitchDecayLabel->setEditable (false, false, false);
        pitchDecayLabel->setColour (Label::backgroundColourId, Colour (0xffdedede));
        pitchDecayLabel->setColour (Label::textColourId, Colour (0xff233034));
        pitchDecayLabel->setColour (TextEditor::textColourId, Colours::black);
        pitchDecayLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        
        pitchExpLin.slider = new Slider();
        addAndMakeVisible (pitchExpLin.slider);
        pitchExpLin.slider.get()->setSliderStyle (Slider::RotaryVerticalDrag);
        pitchExpLin.slider.get()->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
        pitchExpLin.slider.get()->setLookAndFeel(&lf);
        pitchExpLin.attachment.reset (new SliderAttachment(valueTreeState,"pitchExpLin",*pitchExpLin.slider));
        
        pitchExpLinLabel.reset (new Label ("pitchExpLinLabel",
                                     TRANS("P Vel.")));
        addAndMakeVisible(pitchExpLinLabel.get());
        pitchExpLinLabel->setFont (Font (Font::getDefaultMonospacedFontName(), 11.00f, Font::plain).withTypefaceStyle ("Bold").withExtraKerningFactor (0.015f));
        pitchExpLinLabel->setJustificationType (Justification::centred);
        pitchExpLinLabel->setEditable (false, false, false);
        pitchExpLinLabel->setColour (Label::backgroundColourId, Colour (0xffdedede));
        pitchExpLinLabel->setColour (Label::textColourId, Colour (0xff233034));
        pitchExpLinLabel->setColour (TextEditor::textColourId, Colours::black);
        pitchExpLinLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        
        
        ampDecay.slider = new Slider();
        addAndMakeVisible (ampDecay.slider);
        ampDecay.slider.get()->setSliderStyle (Slider::RotaryVerticalDrag);
        ampDecay.slider.get()->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
        ampDecay.slider.get()->setLookAndFeel(&lf);
        ampDecay.attachment.reset (new SliderAttachment(valueTreeState,"ampDecay",*ampDecay.slider));
        
        ampDecayLabel.reset (new Label ("ampDecayLabel",
                                          TRANS("A Dec.")));
        addAndMakeVisible(ampDecayLabel.get());
        ampDecayLabel->setFont (Font (Font::getDefaultMonospacedFontName(), 11.00f, Font::plain).withTypefaceStyle ("Bold").withExtraKerningFactor (0.015f));
        ampDecayLabel->setJustificationType (Justification::centred);
        ampDecayLabel->setEditable (false, false, false);
        ampDecayLabel->setColour (Label::backgroundColourId, Colour (0xffdedede));
        ampDecayLabel->setColour (Label::textColourId, Colour (0xff233034));
        ampDecayLabel->setColour (TextEditor::textColourId, Colours::black);
        ampDecayLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        
        
        ampAmount.slider = new Slider();
        addAndMakeVisible (ampAmount.slider);
        ampAmount.slider.get()->setSliderStyle (Slider::RotaryVerticalDrag);
        ampAmount.slider.get()->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
        ampAmount.slider.get()->setLookAndFeel(&lf);
        ampAmount.attachment.reset (new SliderAttachment(valueTreeState,"ampAmount",*ampAmount.slider));
        
        ampAmountLabel.reset (new Label ("ampAmountLabel",
                                        TRANS("A Level")));
        addAndMakeVisible(ampAmountLabel.get());
        ampAmountLabel->setFont (Font (Font::getDefaultMonospacedFontName(), 11.00f, Font::plain).withTypefaceStyle ("Bold").withExtraKerningFactor (0.015f));
        ampAmountLabel->setJustificationType (Justification::centred);
        ampAmountLabel->setEditable (false, false, false);
        ampAmountLabel->setColour (Label::backgroundColourId, Colour (0xffdedede));
        ampAmountLabel->setColour (Label::textColourId, Colour (0xff233034));
        ampAmountLabel->setColour (TextEditor::textColourId, Colours::black);
        ampAmountLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        
        
        foldAmountLabel.reset (new Label ("foldAmountLabel",
                                           TRANS("Fold")));
        addAndMakeVisible(foldAmountLabel.get());
        foldAmountLabel->setFont (Font (Font::getDefaultMonospacedFontName(), 11.00f, Font::plain).withTypefaceStyle ("Bold").withExtraKerningFactor (0.015f));
        foldAmountLabel->setJustificationType (Justification::centred);
        foldAmountLabel->setEditable (false, false, false);
        foldAmountLabel->setColour (Label::backgroundColourId, Colour (0xffdedede));
        foldAmountLabel->setColour (Label::textColourId, Colour (0xff233034));
        foldAmountLabel->setColour (TextEditor::textColourId, Colours::black);
        foldAmountLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        
        foldAmount.slider = new Slider();
        addAndMakeVisible (foldAmount.slider);
        foldAmount.slider.get()->setSliderStyle (Slider::RotaryVerticalDrag);
        foldAmount.slider.get()->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
        foldAmount.slider.get()->setLookAndFeel(&lf);
        foldAmount.attachment.reset (new SliderAttachment(valueTreeState,"foldAmount",*foldAmount.slider));
        
        
        
        
        
        delayAmountLabel.reset (new Label ("delayAmountLabel",
                                         TRANS("S Delay")));
        addAndMakeVisible(delayAmountLabel.get());
        delayAmountLabel->setFont (Font (Font::getDefaultMonospacedFontName(), 11.00f, Font::plain).withTypefaceStyle ("Bold").withExtraKerningFactor (0.015f));
        delayAmountLabel->setJustificationType (Justification::centred);
        delayAmountLabel->setEditable (false, false, false);
        delayAmountLabel->setColour (Label::backgroundColourId, Colour (0xffdedede));
        delayAmountLabel->setColour (Label::textColourId, Colour (0xff233034));
        delayAmountLabel->setColour (TextEditor::textColourId, Colours::black);
        delayAmountLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
        
        
        delayAmount.slider = new Slider();
        addAndMakeVisible (delayAmount.slider);
        delayAmount.slider.get()->setSliderStyle (Slider::RotaryVerticalDrag);
        delayAmount.slider.get()->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
        delayAmount.slider.get()->setLookAndFeel(&lf);
        delayAmount.attachment.reset (new SliderAttachment(valueTreeState,"delayAmount",*delayAmount.slider));
        
        
        setSize (400, 300);
    }
    ~MainOscComponent()
    {
        
    }
    
    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]
    
    void paint (Graphics& g)
    {
         g.fillAll(Colour::fromRGB(23,30,34));
    }
    void resized()
    {
        
        auto kh = MainOscComponent::proportionOfHeight(.2);
        auto klh = MainOscComponent::proportionOfHeight(.05);
        auto klw = MainOscComponent::proportionOfWidth(.2);
        auto offset = MainOscComponent::proportionOfWidth(.05);
        
        auto mlo = kh*.5+offset;
        mainLabel.get()->setBounds(0,0,kh*3+offset*2,kh*.5);
        
        startFreq.slider.get()->setBounds(0,mlo,kh,kh);
        startLabel.get()->setBounds(0,mlo+kh,kh,klh);
        
        pitchDecay.slider.get()->setBounds(kh+offset,mlo,kh,kh);
        pitchDecayLabel.get()->setBounds(kh+offset,mlo+kh,kh,klh);
        
        ampDecay.slider.get()->setBounds((kh+offset)*2,mlo,kh,kh);
        ampDecayLabel.get()->setBounds((kh+offset)*2,mlo+kh,kh,klh);
        
        endFreq.slider.get()->setBounds(0,(kh+klh)+mlo,kh,kh);
        endLabel.get()->setBounds(0,(kh*2+klh)+mlo,kh,klh);
        
        pitchExpLin.slider.get()->setBounds(kh+offset,(kh+klh)+mlo,kh,kh);
        pitchExpLinLabel.get()->setBounds(kh+offset,(kh*2+klh)+mlo,kh,klh);
        
        ampAmount.slider.get()->setBounds((kh+offset)*2,(kh+klh)+mlo,kh,kh);
        ampAmountLabel.get()->setBounds((kh+offset)*2,(kh*2+klh)+mlo,kh,klh);
        
        delayAmount.slider.get()->setBounds(kh*.5,(kh+klh)*2+mlo,kh,kh);
        delayAmountLabel.get()->setBounds(kh*.5,(kh+klh)*2+kh+mlo,kh,klh);
        
        foldAmount.slider.get()->setBounds((kh+offset)+kh,(kh+klh)*2+mlo,kh,kh);
        foldAmountLabel.get()->setBounds((kh+offset)+kh,(kh+klh)*2+kh+mlo,kh,klh);
    }
    
    
    
private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]
    
    //==============================================================================
    MyKnobLF lf;
    
    std::unique_ptr<Label> mainLabel;
    
    std::unique_ptr<Label> startLabel;
    SliderWithAttachment startFreq;
    
    std::unique_ptr<Label> pitchDecayLabel;
    SliderWithAttachment pitchDecay;
    
    std::unique_ptr<Label> ampDecayLabel;
    SliderWithAttachment ampDecay;
    
    std::unique_ptr<Label> endLabel;
    SliderWithAttachment endFreq;
    
    std::unique_ptr<Label> pitchExpLinLabel;
    SliderWithAttachment pitchExpLin;
    
    std::unique_ptr<Label> ampAmountLabel;
    SliderWithAttachment ampAmount;

     std::unique_ptr<Label> foldAmountLabel;
    SliderWithAttachment foldAmount;
    
    std::unique_ptr<Label> delayAmountLabel;
    SliderWithAttachment delayAmount;
    
    AudioProcessorValueTreeState& valueTreeState;
    CtrlkAudioProcessor& processor;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainOscComponent)
};






class CtrlkAudioProcessorEditor  : public AudioProcessorEditor, public Button::Listener
{
public:
    CtrlkAudioProcessorEditor (CtrlkAudioProcessor& parent, AudioProcessorValueTreeState &vts);
    ~CtrlkAudioProcessorEditor();

    enum
    {
        paramControlHeight = 40,
        paramLabelWidth    = 40,
        paramSliderWidth   = 40
    };

    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    //void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CtrlkAudioProcessor& processor;
    
    std::unique_ptr<Label> pitchLabel;
    std::unique_ptr<TextButton> tapButton;
    
    std::unique_ptr<MainOscComponent> mainOsc;
    
    

    
    MyKnobLF lf[16];
    MyButtonLF blf;
    AudioProcessorValueTreeState& valueTreeState;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CtrlkAudioProcessorEditor)
};
