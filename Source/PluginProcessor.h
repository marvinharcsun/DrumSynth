/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include "stk/"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Filters.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include "Tables.h"


/*
static float blit(double x)
{
    return cos(M_PI*x)/cos((M_PI*x)/127);
}

static double bipolarblit(double x)
{
    return blit(x+M_PI)+blit(x);
}
static const double a = .9;
static double lastOutput = 0.0;
static double leak(double x)
{
    return lastOutput = x + a*lastOutput;
}
*/

class Decimator9
{
private:
    const float h0,h1,h3,h5,h7,h9;
    float R1,R2,R3,R4,R5,R6,R7,R8,R9;
public:
    Decimator9() : h0(8192/16384.0f),h1(5042/16384.0f),h3(-1277/16384.0f),h5(429/16384.0f),h7(-116/16384.0f),h9(18/16384.0f)
    {
        R1=R2=R3=R4=R5=R6=R7=R8=R9=0.0f;
    }
    inline float Calc(const float x0,const float x1)
    {
        float h9x0=h9*x0;
        float h7x0=h7*x0;
        float h5x0=h5*x0;
        float h3x0=h3*x0;
        float h1x0=h1*x0;
        float R10=R9+h9x0;
        R9=R8+h7x0;
        R8=R7+h5x0;
        R7=R6+h3x0;
        R6=R5+h1x0;
        R5=R4+h1x0+h0*x1;
        R4=R3+h3x0;
        R3=R2+h5x0;
        R2=R1+h7x0;
        R1=h9x0;
        return R10;
    }
};

const int STAGE_OFF = -1;
const int NUM_STAGES = 1;
const int DEFAULT_STAGE = 0;
struct Stage{
    double time;
    double curve;
    double rate;
};
class Env{
public:
    Env()
    {
        setStage(STAGE_OFF);
        setSampleRate(44100.0f);
        setStageTime(DEFAULT_STAGE,0.001f);
        setStageTimeCurve(DEFAULT_STAGE,1.0f);
    }
    
    ~Env()
    {
        
    }
    
    void setStage(int stage)
    {
        mStage = stage;
    }
    int getStage()
    {
        return mStage;
    }
    void setStageTime(int stage, double rate)
    {
        mStages[stage].rate = rate;
        mStages[stage].time = 1/(mSampleRate*mStages[stage].rate);
    }
    
    void setStageTimeCurve(int stage, double curve)
    {
        mStages[stage].curve = curve;
    }
    void setSampleRate(double sampleRate)
    {
        mSampleRate = sampleRate;
    }
    void reset()
    {
        
        setStage(DEFAULT_STAGE);
        mEnvValue = 1.0;
        mEnvValue2 = 0.0;
    }
    double process()
    {
        double lastValue = 0.0;
        switch(mStage)
        {
            case DEFAULT_STAGE:
                if(mEnvValue > 0.0f)
                {
                    lastValue = mEnvValue;
                    mEnvValue -= pow(mStages[DEFAULT_STAGE].time, mStages[DEFAULT_STAGE].curve);
                }else{
                    mStage = STAGE_OFF;
                    mEnvValue = 0.0;
                }
                break;
            case STAGE_OFF:
                return 0.0f;
                break;
        }
        
        return lastValue;
    }
    
    double process2()
    {
        double lastValue = 0.0;
        switch(mStage)
        {
            case DEFAULT_STAGE:
                if(mEnvValue2 < 1.0f)
                {
                    lastValue = mEnvValue2;
                    mEnvValue2 += pow(mStages[DEFAULT_STAGE].time, mStages[DEFAULT_STAGE].curve);
                }else{
                    mStage = STAGE_OFF;
                    mEnvValue2 = 1.0;
                    lastValue = 1.0;
                }
                break;
            case STAGE_OFF:
                return 1.0f;
                break;
        }
        
        return lastValue;
    }
    
    
    
private:
    int mStage;
    double mSampleRate;
    Stage mStages[NUM_STAGES];
    double mEnvValue = 0.0;
    double mEnvValue2 = 0.0;
    
};

struct OscState{
    double mPhi;
    double mPhiIncrement;
};

class SampleAndHold{
private:
    OscState lfo;
    double mSampleRate;
    double mValue;
public:
    
    SampleAndHold(){
        mSampleRate = 44100.0;
        mValue = 0.0;
    }
    
    void setSampleRate(double sampleRate)
    {
        mSampleRate = sampleRate;
    }
    
    void setFrequency(double freq)
    {
        lfo.mPhiIncrement = freq/mSampleRate;
    }
    
    double process(){
        
        if(lfo.mPhi == 0.0f)
        {
            mValue = WhiteNoise::process();
        }
        lfo.mPhi += lfo.mPhiIncrement;
        
        if(lfo.mPhi > 1.0)
        {
            lfo.mPhi = 0.0f;
        }
        return mValue;
    }
    
};



class ChirpOsc
{
private:
    void wrap()
    {
        if(mState.mPhi >= 1.0f)
        {
            mState.mPhi -= 1.0f;
        }
        else if(mState.mPhi < 0.0f)
        {
            mState.mPhi += 1.0f;
        }
    }

    
    double mStartFreq;
    double mEndFreq;
    double mSampleRate;
    OscState mState;
    Env pitchEnv;
    double inputs;
    
  
    
    double lastOutput = 0.0;
public:
    
    enum OscillatorMode {
        OSCILLATOR_MODE_SINE=1,
        OSCILLATOR_MODE_SAW,
        OSCILLATOR_MODE_SQUARE,
        OSCILLATOR_MODE_TRIANGLE
    };
    
     OscillatorMode mOscillatorMode;
    
    ChirpOsc()
    {
        
        mState.mPhi = 0.0f;
        mState.mPhiIncrement = 0.0f;
        setSampleRate(44100.0);
        setStartFrequency(6000.0);
        setEndFrequency(60.0);
        mOscillatorMode = OSCILLATOR_MODE_SINE;
        
        
    }
    ~ChirpOsc()
    {
        
    }
    
    double naiveWaveformForMode(OscillatorMode mode) {
        double value;
        switch (mode) {
            case OSCILLATOR_MODE_SINE:
                value = sin(mState.mPhi+inputs);
                break;
            case OSCILLATOR_MODE_SAW:
                value = (mState.mPhi+inputs)/M_PI - 1.0;
                break;
            case OSCILLATOR_MODE_SQUARE:
                if (mState.mPhi+inputs < M_PI) {
                    value = 1.0;
                } else {
                    value = -1.0;
                }
                break;
            case OSCILLATOR_MODE_TRIANGLE:
                //value = -1.0 + (2.0 * (mState.mPhi+inputs) / 2.0*M_PI);
                //value = 2.0 * (fabs(value) - 0.5);
                value = (2/M_PI)*asin(sin((2*M_PI)*(mState.mPhi+inputs)));
                break;
            default:
                break;
        }
        return value;
    }
    
    double poly_blep(double t)
    {
        double dt = mState.mPhiIncrement;
        
        // 0 <= t < 1
        if (t < dt) {
            t /= dt;
            return t+t - t*t - 1.0;
        }
        // -1 < t < 0
        else if (t > 1.0 - dt) {
            t = (t - 1.0) / dt;
            return t*t + t+t + 1.0;
        }
        // 0 otherwise
        else return 0.0;
    }
    
    double process()
    {
        double value = 0.0;
        double t = mState.mPhi/(2.0*M_PI);
        
        if (mOscillatorMode == OSCILLATOR_MODE_SINE) {
            value = naiveWaveformForMode(OSCILLATOR_MODE_SINE);
        } else if (mOscillatorMode == OSCILLATOR_MODE_SAW) {
            value = naiveWaveformForMode(OSCILLATOR_MODE_SAW);
            value -= poly_blep(t);
        } else {
            value = naiveWaveformForMode(OSCILLATOR_MODE_SQUARE);
            value += poly_blep(t);
            value -= poly_blep(fmod(t + 0.5, 1.0));
            
            if (mOscillatorMode == OSCILLATOR_MODE_TRIANGLE) {
                // Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1]
                value = mState.mPhiIncrement * value + (1 - mState.mPhiIncrement) * lastOutput;
                lastOutput = value;
            }
        }
        
        
        
        mState.mPhiIncrement = incrementPhase();
        
        mState.mPhi += mState.mPhiIncrement*(2.0*M_PI);
        while (mState.mPhi >= (2.0*M_PI)) {
            mState.mPhi -= (2.0*M_PI);
        }
        return value;
    }
    
    void setInputs(double input)
    {
        inputs = input;
    }
    
    void setStartFrequency(double freq)
    {
        mStartFreq = freq;
    }
    
    void setEndFrequency(double freq)
    {
        mEndFreq = freq;
    }
    
    
    void setSampleRate(double sampleRate)
    {
        mSampleRate = sampleRate;
        pitchEnv.setSampleRate(sampleRate);
    }
    
    void setPitchDecay(double rate)
    {
        pitchEnv.setStageTime(DEFAULT_STAGE, rate);
    }
    
    void setPitchDecayCurve(double curve)
    {
        pitchEnv.setStageTimeCurve(DEFAULT_STAGE, curve);
    }
    
    void reset()
    {
        pitchEnv.reset();
        mState.mPhi = 0.0f;
    }
    
    double getVariedFrequency()
    {
        return mEndFreq;
    }
    
    double incrementPhase()
    {
        const double time = pitchEnv.process2();
        const double start = mStartFreq;
        const double end = mEndFreq;
        return (end*start / ((end - start)*(1.0-time) + start))/mSampleRate;
    }
    
};




class MySynthesiser : public Synthesiser
{
private:
    double mSampleRate;
    bool held;
    int time;
    
    Env noiseEnv;
    LinearSmoothedValue<double> noiseAmount;
    
    MultiModeLadderFilter noiseFilter;
    
    //Oversampling
    MultiModeLadderFilter lowpass;
    Decimator9 decimator;
    
    
public:
    
    
   
    ChirpOsc osc[6];
    Env ampEnv[6];
    LinearSmoothedValue<double> ampAmount[6];
    double ratios[5];
    LinearSmoothedValue<double> amModAmount;
    bool amMod;
    
    
    
    
    MySynthesiser()
    {
        mSampleRate = 44100.0;
        held = false;
        time = 0 ;
       

        
        
        lowpass.setSampleRate(44100.0);
        lowpass.setCutoff(20000.0);
        lowpass.setResonance(0.0);
        
        

        
    }
    
    
    void setCurrentPlaybackSampleRate (double sampleRate) override
    {
        mSampleRate = sampleRate;
        const double time = 0.01;
        for(int i = 0; i < 2; i++)
        {
            osc[i].setSampleRate(sampleRate);
            ampEnv[i].setSampleRate(sampleRate);
            ampAmount[i].reset(sampleRate,time);
            
        }
        
        amModAmount.reset(sampleRate,time);
        
        noiseAmount.reset(sampleRate,time);
        
        noiseEnv.setSampleRate(sampleRate);
        
        
        
        
        noiseFilter.setSampleRate(sampleRate);
        noiseFilter.setFilterType(HPF4);
        noiseFilter.setCutoff(1000.0f);
        
        lowpass.setSampleRate(sampleRate);
        lowpass.setCutoff(20000.0);
        
       
        
        
    }
    void reset()
    {
        
        osc[0].reset();
        osc[1].reset();
        
        
        ampEnv[0].reset();
        ampEnv[1].reset();
        
        
        noiseEnv.reset();
        lowpass.reset();
        noiseFilter.reset();
    }
  
    void setAmpDecay(double rate1,double rate2)
    {
        ampEnv[0].setStageTime(DEFAULT_STAGE, rate1);
        ampEnv[1].setStageTime(DEFAULT_STAGE, rate2);
    }
    
    void setAmpAmount(double amount1, double amount2)
    {
        ampAmount[0].setValue(amount1);
        ampAmount[1].setValue(amount2);
    }
    
    
    void setWaveForm(int form1, int form2)
    {
        osc[0].mOscillatorMode = (ChirpOsc::OscillatorMode)form1;
        osc[1].mOscillatorMode = (ChirpOsc::OscillatorMode)form2;
    }
    
    void setFundamentalAndRatios(double start, double end, double ratio, double offset)
    {
        
     
        osc[0].setStartFrequency(start);
        osc[0].setEndFrequency(end);
        
        osc[1].setStartFrequency(start);
        osc[1].setEndFrequency(end*ratio+offset);
        
    }
    
    
    void setPitchDecay(double rate1,double rate2)
    {
        osc[0].setPitchDecay(rate1);
        osc[1].setPitchDecay(rate2);
        
    }
    
    void setPitchExpLin(double explin1, double explin2)
    {
        osc[0].setPitchDecayCurve(explin1);
        osc[1].setPitchDecayCurve(explin2);
    }
   
    
    void setNoiseDecay(double rate)
    {
        noiseEnv.setStageTime(DEFAULT_STAGE, rate);
    }
    
    void setNoiseAmount(double amount)
    {
        noiseAmount.setValue(amount);
    }
    
    
    void myRenderBlock(AudioBuffer<float>& outputBuffer, MidiBuffer& midiMessages,int startSample, int numSamples)
    {
        auto *channelData0 = outputBuffer.getWritePointer(0);
        auto *channelData1 = outputBuffer.getWritePointer(1);
        
        double firstValue = 0.0;
        double secondValue = 0.0;
        
        MidiBuffer processedMidi;
       
        MidiMessage m;
        startSample = 0;
        int time = 0;
        MidiBuffer::Iterator it (midiMessages);
        it.getNextEvent(m,time);
        if(m.isNoteOff())
        {
            held = false;
        }
        
        while(startSample < numSamples)
        {
           
            if(m.isNoteOn() && !held)
            {
                reset();
                held = true;
            }
          
            for(int i = 0; i < 2; i++)
            {
               
                
                double oscillators = osc[0].process()*ampEnv[0].process()*ampAmount[0].getNextValue();
                                osc[0].setInputs(osc[1].process()*ampEnv[1].process()*ampAmount[1].getNextValue()*2.0*M_PI);
                
                
                if(i%2==0)
                {
                    firstValue = lowpass.process(oscillators);
                }else{
                    secondValue = lowpass.process(oscillators);
                }
            }
           
            const double finalOutput = decimator.Calc(firstValue,secondValue);
        
            channelData0[startSample] = finalOutput*0.5011872336272722;//+whitenoise;
            channelData1[startSample] = finalOutput*0.5011872336272722;//+whitenoise;
            
            startSample++;
            
        }
        
        midiMessages.swapWith (processedMidi);
    }
    
   
   
    
};
class CtrlkAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    CtrlkAudioProcessor();
    ~CtrlkAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
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
    float mSampleRate;
    
    MySynthesiser *synth;
    
    AudioProcessorValueTreeState parameters;
    
    void update();
    
    float *startFreq;
    float *endFreq;
    
    float *ratio;
    float *offset;
    
    
    float *pitchDecay1;
    float *pitchDecay2;
    
    float *pitchExpLin1;
    float *pitchExpLin2;

    
    float *ampAmount1;
    float *ampAmount2;

    
    float *ampDecay1;
    float *ampDecay2;

    
    
    
    float *ring1;

    float *waveselect1;
    float *waveselect2;

    
   
    
    
private:
    

    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CtrlkAudioProcessor)
};




