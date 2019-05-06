/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"




//==============================================================================
CtrlkAudioProcessor::CtrlkAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),parameters(*this,nullptr)
#endif
{
   
    
    parameters.createAndAddParameter("startFreq", "Start Frequency",{}, NormalisableRange<float> (100.0f,10020.0f), 6000.0f, nullptr, nullptr);
    parameters.createAndAddParameter("endFreq", "End Frequency",{}, NormalisableRange<float> (20.0f,440.0f), 60.0f, nullptr, nullptr);
    parameters.createAndAddParameter("ratio", "Ratio",{}, NormalisableRange<float> (0.0f,24.0f), 1.0f, nullptr, nullptr);
   
    parameters.createAndAddParameter("offset", "offset",{}, NormalisableRange<float> (0.0f,1000.0f), 0.0f, nullptr, nullptr);
    
     parameters.createAndAddParameter("pitchDecay1", "Pitch Decay 1",{}, NormalisableRange<float> (0.001f,2.0f), 0.5f, nullptr, nullptr);
     parameters.createAndAddParameter("pitchDecay2", "Pitch Decay 2",{}, NormalisableRange<float> (0.001f,2.0f), 0.5f, nullptr, nullptr);

    parameters.createAndAddParameter("pitchExpLin1", "Pitch Exp. Lin. 1",{}, NormalisableRange<float> (0.5f,1.0f), 0.63f, nullptr, nullptr);
    
    parameters.createAndAddParameter("pitchExpLin2", "Pitch Exp. Lin. 2",{}, NormalisableRange<float> (0.5f,1.0f), 0.63f, nullptr, nullptr);

    
    
    parameters.createAndAddParameter("ampDecay1", "Amp Decay 1",{}, NormalisableRange<float> (0.002f,2.0f), 0.63f, nullptr, nullptr);
    
    parameters.createAndAddParameter("ampDecay2", "Amp Decay 2",{}, NormalisableRange<float> (0.002f,2.0f), 0.63f, nullptr, nullptr);

    
    
    parameters.createAndAddParameter("ampAmount1", "Amp Amount 1",{}, NormalisableRange<float> (0.0f,1.0f), 1.0f, nullptr, nullptr);
    parameters.createAndAddParameter("ampAmount2", "Amp Amount 2",{}, NormalisableRange<float> (0.0f,1.0f),0.0,nullptr,nullptr);

    parameters.createAndAddParameter("waveselect1", "waveselect1", {}, NormalisableRange<float>(1.0,4.0), 1.0, nullptr, nullptr);
    parameters.createAndAddParameter("waveselect2", "waveselect2", {}, NormalisableRange<float>(1.0,4.0), 1.0, nullptr, nullptr);

    
    
    
    
    parameters.state = ValueTree(Identifier("CTRLKParameters"));
    
    
    
    startFreq = parameters.getRawParameterValue("startFreq");
    endFreq = parameters.getRawParameterValue("endFreq");
    
    ratio = parameters.getRawParameterValue("ratio");
    offset = parameters.getRawParameterValue("offset");
    
    
    pitchDecay1 = parameters.getRawParameterValue("pitchDecay1");
    pitchExpLin1 = parameters.getRawParameterValue("pitchExpLin1");
    
    pitchDecay2 = parameters.getRawParameterValue("pitchDecay2");
    pitchExpLin2 = parameters.getRawParameterValue("pitchExpLin2");


    ampAmount1 = parameters.getRawParameterValue("ampAmount1");
    ampAmount2 = parameters.getRawParameterValue("ampAmount2");

    ampDecay1 = parameters.getRawParameterValue("ampDecay1");
    ampDecay2 = parameters.getRawParameterValue("ampDecay2");

    waveselect1 = parameters.getRawParameterValue("waveselect1");
    waveselect2 = parameters.getRawParameterValue("waveselect2");

    
    synth = new MySynthesiser();
    
   
}

CtrlkAudioProcessor::~CtrlkAudioProcessor()
{
    delete synth;
}

//==============================================================================
const String CtrlkAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CtrlkAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CtrlkAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CtrlkAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CtrlkAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CtrlkAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CtrlkAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CtrlkAudioProcessor::setCurrentProgram (int index)
{
}

const String CtrlkAudioProcessor::getProgramName (int index)
{
    return {};
}

void CtrlkAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CtrlkAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth->setCurrentPlaybackSampleRate(sampleRate*2.0);
    update();
}

void CtrlkAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CtrlkAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CtrlkAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming spread
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    update();
    synth->myRenderBlock(buffer, midiMessages, 0, numSamples);
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
   
}

void CtrlkAudioProcessor::update()
{
    
    
    synth->setFundamentalAndRatios(*startFreq,*endFreq,*ratio,*offset);
    synth->setWaveForm((int)*waveselect1, (int)*waveselect2);
    synth->setAmpAmount(*ampAmount1, *ampAmount2);
    synth->setAmpDecay(*ampDecay1,*ampDecay2);
    synth->setPitchDecay(*pitchDecay1,*pitchDecay2);
    synth->setPitchExpLin(*pitchExpLin1,*pitchExpLin2);
    

    
    
}

//==============================================================================
bool CtrlkAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CtrlkAudioProcessor::createEditor()
{
    return new CtrlkAudioProcessorEditor (*this, parameters);
}



//==============================================================================
void CtrlkAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void CtrlkAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CtrlkAudioProcessor();
}
