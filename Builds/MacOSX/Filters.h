//
//  Filters.h
//  CTRLK
//
//  Created by Marvin Harootoonyan on 8/20/18.
//  Copyright © 2018 Bzarvin-Records. All rights reserved.
//

#pragma once

//[Headers]     -- You can add your own extra header files here --


#include <complex>
#include <cmath>
#include <xmmintrin.h>
#include <emmintrin.h>


enum MultiModeLadderFilterTypes
{
    OFF=0,
    LPF1,
    LPF2,
    LPF3,
    LPF4,
    HPF1,
    HPF2,
    HPF3,
    HPF4,
    BPF2,
    BPF4,
    NF2,
    NF4,
    PF2,
    PF4,
    NumberOfFilterTypes
};
class Lowpass1{
public:
    Lowpass1(){
        m_cutoff = 10.0;
        m_sampleRate = 44100.0;
        m_z1_state = 0.0;
    }
    inline const float process(const float x){
        const float v = (x - m_z1_state)*m_G;
        const float y = v + m_z1_state;
        m_z1_state = y + v;
        return y;
    }
    inline void setCutoff(const float cutoff){
        m_cutoff = cutoff;
        setInstantaneousResponseGain();
    }
    inline void setSampleRate(const float sampleRate){
        m_sampleRate = sampleRate;
        setInstantaneousResponseGain();
    }
    inline void reset(){
        m_z1_state = 0.0;
    }
    inline const float getFilterState(){
        return m_z1_state;
    }
    inline const float getInstantaneousResponseGain(){
        return m_g;
    }
    float m_z1_state;
private:
    inline void setInstantaneousResponseGain(){
        m_g = tan(M_PI*m_cutoff/m_sampleRate);
        m_G = m_g/(1.0+m_g);
    }
    float m_cutoff;
    float m_sampleRate;
    float m_g;
    float m_G;
};


class MultiModeLadderFilter{
public:
    
    MultiModeLadderFilter(){
        setCutoff(1000.0);
        setResonance(0.0);
        setFilterType(LPF4);
        setBoost(false);
        setSampleRate(44100.0);
    }
    ~MultiModeLadderFilter(){
        
    }
    inline const float process(float x){
        if(m_filterType==OFF) return x;
        if(m_boost==true) x=x*(m_k+1.0);
        return calculateCoefficients(x);
    }
    inline void setCutoff(const float cutoff){
        m_f1.setCutoff(cutoff);
        m_f2.setCutoff(cutoff);
        m_f3.setCutoff(cutoff);
        m_f4.setCutoff(cutoff);
        m_cutoff = cutoff;
        setCoefficients();
    }
    inline void setResonance(const float res){
        m_k = res*4.0;
        m_alpha = 1.0/(1.0 + m_k*m_GAMMA);
    }
    inline void setBoost(const bool boost){
        m_boost = boost;
    }
    void setFilterType(const int filterType){
        m_filterType = filterType;
        switch (m_filterType) {
            case LPF1:
                m_a0 = 0.0;m_a1 = 1.0;m_a2 = 0.0;m_a3 = 0.0;m_a4 = 0.0;
                break;
            case LPF2:
                m_a0 = 0.0;m_a1 = 0.0;m_a2 = 1.0;m_a3 = 0.0;m_a4 = 0.0;
                break;
            case LPF3:
                m_a0 = 0.0;m_a1 = 0.0;m_a2 = 0.0;m_a3 = 1.0;m_a4 = 0.0;
                break;
            case LPF4:
                m_a0 = 0.0;m_a1 = 0.0;m_a2 = 0.0;m_a3 = 0.0;m_a4 = 1.0;
                break;
            case HPF1:
                m_a0 = 1.0; m_a1 = -1.0;m_a2 = 0.0;m_a3 = 0.0;m_a4 = 0.0;
                break;
            case HPF2:
                m_a0 = 1.0; m_a1 = -2.0;m_a2 = 1.0;m_a3 = 0.0;m_a4 = 0.0;
                break;
            case HPF3:
                m_a0 = 1.0; m_a1 = -3.0;m_a2 = 3.0;m_a3 = -1.0;m_a4 = 0.0;
                break;
            case HPF4:
                m_a0 = 1.0;m_a1 = -4.0;m_a2 = 6.0;m_a3 = -4.0;m_a4 = 1.0;
                break;
            case BPF2:
                m_a0 = 0.0;m_a1 = 2.0;m_a2 = -2.0;m_a3 = 0.0;m_a4 = 0.0;
                break;
            case BPF4:
                m_a0 = 0.0;m_a1 = 0.0;m_a2 = 4.0;m_a3 = -8.0;m_a4 = 4.0;
                break;
            case NF2:
                m_a0 = 1.0; m_a1 = -2.0;m_a2 = 2.0;m_a3 = 0.0;m_a4 = 0.0;
                break;
            case NF4:
                m_a0 = 1.0; m_a1 = -4.0;m_a2 = 8.0;m_a3 = -8.0;m_a4 = 4.0;
                break;
            case PF2:
                m_a0 = 1.0; m_a1 = -1.0;m_a2 = 1.0;m_a3 = 0.0;m_a4 = 0.0;
                break;
            case PF4:
                m_a0 = 1.0; m_a1 = -2.0;m_a2 = 3.0;m_a3 = -2.0;m_a4 = 1.0;
                break;
            default:
                break;
        }
    }
    inline void setSampleRate(const float sampleRate){
        m_f1.setSampleRate(sampleRate);
        m_f2.setSampleRate(sampleRate);
        m_f3.setSampleRate(sampleRate);
        m_f4.setSampleRate(sampleRate);
        m_sampleRate = sampleRate;
        setCoefficients();
    }
    inline const float getSampleRate(){
        return m_sampleRate;
    }
    inline float getCutoff()
    {
        return m_cutoff;
    }
    
    //based on transfer functions
    inline const float magnitudePlot(float x){
        const float w = x/m_cutoff;
        const std::complex<float> j =  std::complex<float>(0.0,1.0);
        const std::complex<float> s = w*j;
        const std::complex<float> c = std::complex<float>(1.0,0.0);
        const std::complex<float> n = m_a0*(s+c)*(s+c)*(s+c)*(s+c)+m_a1*(s+c)*(s+c)*(s+c)
        +m_a2*(s+c)*(s+c)+m_a3*(s+c)+m_a4;
        const std::complex<float> d = (s+c)*(s+c)*(s+c)*(s+c)+m_k;
        const std::complex<float> N = n.real()*n.real()+n.imag()*n.imag();
        const std::complex<float> D = d.real()*d.real()+d.imag()*d.imag();
        const std::complex<float> H = sqrt(N/D);
        
        if(m_boost==true){
            return 20.0*log10f((1.0+m_k)*H.real());
        }
        
        return 20.0*log10f(H.real());
    }
    inline void reset(){
        m_f1.reset();
        m_f2.reset();
        m_f3.reset();
        m_f4.reset();
    }
    
private:
    inline void setCoefficients(){
        const float g = m_f1.getInstantaneousResponseGain();
        const float G = g/(1+g);
        m_GAMMA = G*G*G*G;
        m_beta1 = G*G*G/(1.0+g);
        m_beta2 = G*G/(1.0+g);
        m_beta3 = G/(1.0+g);
        m_beta4 = 1.0/(1.0+g);
    }
    inline float calculateCoefficients(const float x){
        const float S = m_f1.m_z1_state*m_beta1+m_f2.m_z1_state*m_beta2+
        m_f3.m_z1_state*m_beta3+m_f4.m_z1_state*m_beta4;
        const float u = (x - m_k*S)*m_alpha;
        const float y1 = m_f1.process(u);
        const float y2 = m_f2.process(y1);
        const float y3 = m_f3.process(y2);
        const float y4 = m_f4.process(y3);
        return m_a0*u + m_a1*y1 + m_a2*y2 + m_a3*y3 + m_a4*y4;
    }
    int m_filterType;
    float m_cutoff;
    float m_k;
    float m_sampleRate;
    bool m_boost;
    
    float m_GAMMA;
    float m_beta1;
    float m_beta2;
    float m_beta3;
    float m_beta4;
    
    float m_alpha;
    
    float m_a0;
    float m_a1;
    float m_a2;
    float m_a3;
    float m_a4;
    
    Lowpass1 m_f1;
    Lowpass1 m_f2;
    Lowpass1 m_f3;
    Lowpass1 m_f4;
};

class SVF{
public:
    SVF(){
        ic1eq = 0.0;
        ic2eq = 0.0;
        A = 0.0;
        g = 0.0;
        a1 = 0.0;
        a2 = 0.0;
        a3 = 0.0;
        m0 = 0.0;
        m1 = 0.0;
        m2 = 0.0;
        k = 0.0;
        v3 = 0.0;
        v1 = 0.0;
        v2 = 0.0;
        output =0.0;
        drive = 1.0;
        sampleRate = 44100.0;
        Q = .71;
        cutoff = 10000.0;
        bellGain = -180.0;
        filterType = 0;
    }
    ~SVF(){}
    
    float cutoff;
    float Q;
    int filterType;
    float sampleRate;
    float bellGain;
    float drive;
    float ic1eq;
    float ic2eq;
    
    float process(float v0){
        switch (filterType) {
            case off:
                return v0;
                break;
            case low:
                g = tan(M_PI*cutoff/sampleRate);
                k = 1.0/Q;
                a1 = 1.0/(1.0 + g*(g + k));
                a2 = g*a1;
                a3 = g*a2;
                m0 = 0.0;
                m1 = 0.0;
                m2 = 1.0;
                break;
            case band:
                g = tan(M_PI*cutoff/sampleRate);
                k = 1.0/Q;
                a1 = 1.0/(1.0 + g*(g + k));
                a2 = g*a1;
                a3 = g*a2;
                m0 = 0.0;
                m1 = 1.0;
                m2 = 0.0;
                break;
            case high:
                g = tan(M_PI*cutoff/sampleRate);
                k = 1/Q;
                a1 = 1/(1 + g*(g + k));
                a2 = g*a1;
                a3 = g*a2;
                m0 = 1;
                m1 = -k;
                m2 = -1;
                break;
            case notch:
                g = tan(M_PI*cutoff/sampleRate);
                k = 1/Q;
                a1 = 1/(1 + g*(g + k));
                a2 = g*a1;
                a3 = g*a2;
                m0 = 1;
                m1 = -k;
                m2 = -2;
                break;
            case peak:
                g = tan(M_PI*cutoff/sampleRate);
                k = 1/Q;
                a1 = 1/(1 + g*(g + k));
                a2 = g*a1;
                a3 = g*a2;
                m0 = 1;
                m1 = -k;
                m2 = 0;
                break;
            case bell:
                A = pow(10, bellGain/40);
                g = tan(M_PI*cutoff/sampleRate);
                k = 1/(Q*A);
                a1 = 1/(1 + g*(g + k));
                a2 = g*a1;
                a3 = g*a2;
                m0 = 1;
                m1 = k*(A*A - 1);
                m2 = 0;
                break;
            case lowshelf:
                A = pow(10, bellGain/40);
                g = tan(M_PI*cutoff/sampleRate)/sqrt(A);
                k = 1/Q;
                a1 = 1/(1 + g*(g + k));
                a2 = g*a1;
                a3 = g*a2;
                m0 = 1;
                m1 = k*(A - 1);
                m2 = (A*A - 1);
                break;
            case highshelf:
                A = pow(10, bellGain/40);
                g = tan(M_PI*cutoff/sampleRate)/sqrt(A);
                k = 1/Q;
                a1 = 1/(1 + g*(g + k));
                a2 = g*a1;
                a3 = g*a2;
                m0 = A*A;
                m1 = k*(1 - A)*A;
                m2 = (1 - A*A);
                break;
            default:
                break;
        }
        v3 = v0 - ic2eq;
        v1 = a1*ic1eq + a2*v3;
        v2 = ic2eq + a2*ic1eq + a3*v3;
        ic1eq = 2.0*v1 - ic1eq;
        ic2eq = 2.0*v2 - ic2eq;
        output = m0*v0 + m1*v1 + m2*v2;
        return output;
    }
private:
    float A;
    float g;
    float a1;
    float a2;
    float a3;
    float m0;
    float m1;
    float m2;
    float k;
    float v3;
    float v1;
    float v2;
    float output;
};

#endif /* Filters_h */
