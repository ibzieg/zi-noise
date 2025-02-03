/*
 * Copyright 2021, Ian Zieg
 *
 * This file is part of a program called "imgspec"
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "NoiseSource.h"
#include "../support/MathSupport.h"

void NoiseSource::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    // Initialize wave table
    _waveTableSize = WAVE_TABLE_SIZE;
    for (int i = 0; i < _waveTableSize; i++) {
        // Sin
        _waveTableSin[i] = (float) sin(2.0 * double_Pi * i / _waveTableSize);

        // Saw
        _waveTableSaw[i] = ((float) i / (float) _waveTableSize) * 2.0f - 1.0f;

        // Sqr
        if (i < (_waveTableSize / 2)) {
            _waveTableSqr[i] = -1.0f;
        } else {
            _waveTableSqr[i] = 1.0f;
        }
    }

    // Initialize oscillators
    _sampleRate = sampleRate;

    _smoothFreq = 261.65;
    _smoothFreq.reset(_sampleRate, PITCH_SMOOTH_SECONDS);

    _modOscRatioSmooth.reset(_sampleRate, PITCH_SMOOTH_SECONDS);
    _lfoOscRatioSmooth.reset(_sampleRate, PITCH_SMOOTH_SECONDS);

    _phaseModAmountSmooth.reset(_sampleRate, PHASE_MOD_SMOOTH_SECONDS);
    _phaseModAmountSmooth = synthOptions.phaseModAmount;

    _carrierOscFreq = 261.65;
    _carrierOscPhase = 0.0;

    _modOscFreq = 523.22;
    _modOscPhase = 0.0;

    _lfoOscFreq = 0.01;
    _lfoOscPhase = 0.0;

    _modEnv.setSampleRate(sampleRate);

    _isPreparedToPlay = true;
}

void NoiseSource::releaseResources() {
    // Do nothing ?
}

void NoiseSource::resetPhase() {
    _carrierOscPhase = 0.0;
    _modOscPhase = 0.0;
    _lfoOscPhase = 0.0;
}

void NoiseSource::updateFreqFactors() {
    //    for (int i = 0; i < _spectraCount; i++)
    //    {
    //        double factor = MathSupport::getFactorAtIndex(_patternType, i, _ratio, _spectraCount);
    //        _factorTable[i] = factor;
    //    }
}

float *NoiseSource::getWaveTable(int type) {
    switch (type) {
        case 2:
            return _waveTableSqr;
        case 1:
            return _waveTableSaw;
        default:
            return _waveTableSin;
    }
}

void NoiseSource::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) {
    jassert (_isPreparedToPlay);

    dsp::AudioBlock<float> block{*bufferToFill.buffer};

    double phaseIndexScalar = (double) _waveTableSize / MathSupport::PI2;

    _modEnv.setParameters(ADSR::Parameters{
            synthOptions.modEnvAttack,
            synthOptions.modEnvDecay,
            synthOptions.modEnvSustain,
            synthOptions.modEnvRelease});

    //    float *waveTable = getWaveTable(0 /* sin */);
    float *waveTable = _waveTableSin;

    _modOscRatioSmooth.setTargetValue(synthOptions.modFreqRatio);
    _lfoOscRatioSmooth.setTargetValue(synthOptions.lfoFreqRatio);
    float phaseModAmount;

    for (size_t s = 0; s < block.getNumSamples(); ++s) {
        // Get Frequency
        double smoothFreq = _smoothFreq.getNextValue();
        double freq = smoothFreq;
        _modOscFreq = _modOscRatioSmooth.getNextValue() * freq;
        //        _lfoOscFreq = _lfoOscRatioSmooth.getNextValue() * freq;
        _lfoOscFreq = _lfoOscRatioSmooth.getNextValue() * 261.61f;

        // Modulator
        const double cyclesPerSampleMod = _modOscFreq / _sampleRate;
        const double phaseDeltaMod = MathSupport::PI2 * cyclesPerSampleMod;
        const double modPhase = std::fmod(_modOscPhase + phaseDeltaMod, MathSupport::PI2);
        _modOscPhase = modPhase;

        const int modWaveIndex = abs((int) (modPhase * phaseIndexScalar) % (_waveTableSize - 1));
        jassert (modWaveIndex < _waveTableSize - 1);
        jassert (modWaveIndex >= 0);
        const float modWaveValue = waveTable[modWaveIndex];

        // LFO
        const double cyclesPerSampleLFO = _lfoOscFreq / _sampleRate;
        const double phaseDeltaLFO = MathSupport::PI2 * cyclesPerSampleLFO;
        const double lfoPhase = std::fmod(_lfoOscPhase + phaseDeltaLFO, MathSupport::PI2);
        _lfoOscPhase = lfoPhase;

        const int lfoWaveIndex = abs((int) (lfoPhase * phaseIndexScalar) % (_waveTableSize - 1));
        jassert (lfoWaveIndex < _waveTableSize - 1);
        jassert (lfoWaveIndex >= 0);
        const float lfoWaveValue = waveTable[lfoWaveIndex];

        // TODO Best way to mix these?
        const auto modEnvValue = _modEnv.getNextSample();
        _phaseModAmountSmooth.setTargetValue((1.0f * synthOptions.phaseModAmount) + (1.0f * modEnvValue) +
                                             (synthOptions.lfoModAmount * lfoWaveValue * modEnvValue));
        phaseModAmount = _phaseModAmountSmooth.getNextValue();

        // Carrier wave table value
        const double carrierFreqModulated = freq + (phaseModAmount * modWaveValue * freq);
        const double cyclesPerSampleCarrier = carrierFreqModulated / _sampleRate;
        //        const double cyclesPerSampleCarrier = freq / _sampleRate;
        const double phaseDeltaCarrier = MathSupport::PI2 * cyclesPerSampleCarrier;
        const double carrierPhase = std::fmod(_carrierOscPhase + phaseDeltaCarrier, MathSupport::PI2);
        _carrierOscPhase = carrierPhase;

        const int waveIndex = abs((int) (carrierPhase * phaseIndexScalar) % (_waveTableSize - 1));

        // Phase Modulation:
        //        const auto modulatedPhase = static_cast<const float> ((carrierPhase * modWaveValue * phaseModAmount) + (carrierPhase * (1.0f - phaseModAmount)));
        //        const int waveIndex = abs ((int) (modulatedPhase * phaseIndexScalar) % (_waveTableSize - 1));

        jassert (waveIndex < _waveTableSize - 1);
        jassert (waveIndex >= 0);
        const float carrierWaveValue = waveTable[waveIndex];

        float sampleValue = carrierWaveValue;


        // Noise Source vvv

        const auto samplesPerMillisecond = static_cast<const float>(_sampleRate / 1000.0f);

        int minRandomInt = std::max(
                static_cast<int>((synthOptions.grainLengthMin + (modEnvValue * 1.0f)) * samplesPerMillisecond), 1);
        int maxRandomInt = std::max(
                static_cast<int>((synthOptions.grainLengthMax - (modEnvValue * 1.0f)) * samplesPerMillisecond),
                minRandomInt + 1);

        if (_samplesUntilNextRandom <= 0) {
            _noiseValue = _rng.nextFloat() * 2.0f - 1.0f;
            _noiseDelta = _rng.nextFloat() * 2.0f - 1.0f;

            _samplesUntilNextRandom = _rng.nextInt(Range<int>{minRandomInt, maxRandomInt});
        } else {
            _samplesUntilNextRandom -= 1;
        }

        _noiseValue += _noiseDelta;
        if (_noiseValue > 1.0f) {
            _noiseValue = 1.0f;
            _noiseDelta = -_noiseDelta;
        } else if (_noiseValue < -1.0f) {
            _noiseValue = -1.0f;
            _noiseDelta = -_noiseDelta;
        }

        sampleValue = _noiseValue;

        // Noise Source ^^^





        // Apply stereo tilt and set channel sample values
        for (size_t ch = 0; ch < block.getNumChannels(); ++ch) {
            float newSample = 0.0f;

            newSample = sampleValue * (float) _velocity;
            block.setSample((int) ch, s, newSample);
        }
    }
}

void NoiseSource::startNote(const int midiNoteNumber, float velocity) {
    _velocity = velocity;
    _carrierOscFreq = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    _smoothFreq.setTargetValue(_carrierOscFreq);

    _phaseModAmountSmooth.setTargetValue(synthOptions.phaseModAmount);

    _modEnv.noteOn();

    resetPhase();
}

void NoiseSource::stopNote(float velocity) {
    _modEnv.noteOff();
}
