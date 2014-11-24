//
//  ___FILENAME___
//  ___PACKAGENAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

// This is a test implementation of a sin wave synth using
// AUInstrumentBase classes
//
//  It illustrates a basic usage of these classes
//
//  It artificially limits the number of notes at one time to 12, so
//  the note-stealing algorithm is used - you should know how this
//  works!
//
//  Most of the work you need to do is defining a Note class (see
//  ___PACKAGENAMEASIDENTIFIER___Note). AUInstrument manages the
//  creation and destruction of notes, the various stages of a note's
//  lifetime.
//
//  Alot of printfs have been left in (but are if'def out) These can
//  be useful as you figure out how this all fits together. This is
//  true in the AUInstrumentBase classes as well; simply define
//  DEBUG_PRINT to 1 and this turns all this on
//
//  The project also defines CA_AUTO_MIDI_MAP (OTHER_C_FLAGS). This
//  adds all the code that is needed to map MIDI messages to specific
//  parameter changes. This can be seen in AU Lab's MIDI Editor window
//  CA_AUTO_MIDI_MAP is implemented in AUMIDIBase.cpp/.h

#include "___FILEBASENAME___.h"
 
static const UInt32 kMaxActiveNotes = 8;

// Define parameter name strings here.
// static CFStringRef kParamName_One = CFSTR("First Parameter");
// static CFStringRef kParamName_Two = CFSTR("Second Parameter");

////////////////////////////////////////////////////////////////////////////////

#pragma mark ___PACKAGENAMEASIDENTIFIER___ Methods

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUMusicDeviceFactory, ___PACKAGENAMEASIDENTIFIER___)

// Define the presets.
static AUPreset kPresets[kNumberOfPresets] = {
    { kPreset_Default, CFSTR("Factory Default") },
    // { kPreset_One, CFSTR("Preset One") },
    // { kPreset_Two, CFSTR("Preset Two") },
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      ___PACKAGENAMEASIDENTIFIER___::___PACKAGENAMEASIDENTIFIER___
//
// This synth has No inputs, One output
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
___PACKAGENAMEASIDENTIFIER___::___PACKAGENAMEASIDENTIFIER___(AudioUnit inComponentInstance)
: AUMonotimbralInstrumentBase(inComponentInstance, 0, 1)
{
    CreateElements();
        
    Globals()->UseIndexedParameters(kNumberOfParameters);
    // Globals()->SetParameter(kParameter_One, 1.0);
    // Globals()->SetParameter(kParameter_Two, 1.0);

    SetAFactoryPresetAsCurrent(kPresets[kPreset_Default]);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      ___PACKAGENAMEASIDENTIFIER___::~___PACKAGENAMEASIDENTIFIER___
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
___PACKAGENAMEASIDENTIFIER___::~___PACKAGENAMEASIDENTIFIER___()
{}

void ___PACKAGENAMEASIDENTIFIER___::Cleanup()
{
#if DEBUG_PRINT
    printf("___PACKAGENAMEASIDENTIFIER___::Cleanup\n");
#endif
}

OSStatus ___PACKAGENAMEASIDENTIFIER___::Initialize()
{       
#if DEBUG_PRINT
    printf("->___PACKAGENAMEASIDENTIFIER___::Initialize\n");
#endif
    AUMonotimbralInstrumentBase::Initialize();
        
    SetNotes(kNumNotes, kMaxActiveNotes, mNotes, sizeof(___PACKAGENAMEASIDENTIFIER___Note));
#if DEBUG_PRINT
    printf("<-___PACKAGENAMEASIDENTIFIER___::Initialize\n");
#endif

    return noErr;
}

AUElement* ___PACKAGENAMEASIDENTIFIER___::CreateElement(AudioUnitScope   scope,
                                                        AudioUnitElement element)
{
    switch (scope) {

    case kAudioUnitScope_Group:
        return new SynthGroupElement(this, element, new MidiControls);

    case kAudioUnitScope_Part:
        return new SynthPartElement(this, element);

    default:
        return AUBase::CreateElement(scope, element);
    }
}

OSStatus ___PACKAGENAMEASIDENTIFIER___::GetParameterInfo(AudioUnitScope           inScope,
                                    AudioUnitParameterID     inParameterID,
                                    AudioUnitParameterInfo & outParameterInfo)
{
    AudioUnitParameterInfo& info = outParameterInfo;
    info.flags = (kAudioUnitParameterFlag_IsWritable |
                  kAudioUnitParameterFlag_IsReadable);

    if (inScope != kAudioUnitScope_Global)
        return kAudioUnitErr_InvalidScope;

    switch (inParameterID) {

    // case kParam_One:
    //     info.flags = kAudioUnitParameterFlag_DisplaySquareRoot;
    //     info.flags |= kAudioUnitParameterFlag_IsWritable;
    //     info.flags |= kAudioUnitParameterFlag_IsReadable;

    //     AUBase::FillInParameterName (info, kGlobalVolumeName, false);
    //     info.unit = kAudioUnitParameterUnit_LinearGain;
    //     info.minValue = 0;
    //     info.maxValue = 1.0;
    //     info.defaultValue = 1.0;
    //     break;

    // case kParam_Two:
    //     info.flags = kAudioUnitParameterFlag_DisplaySquareRoot;
    //     info.flags |= kAudioUnitParameterFlag_IsWritable;
    //     info.flags |= kAudioUnitParameterFlag_IsReadable;

    //     AUBase::FillInParameterName (info, kGlobalVolumeName, false);
    //     info.unit = kAudioUnitParameterUnit_LinearGain;
    //     info.minValue = 0;
    //     info.maxValue = 1.0;
    //     info.defaultValue = 1.0;
    //     break;

    default:
        return kAudioUnitErr_InvalidParameter;
    }

    return noErr;
}


#pragma mark ___PACKAGENAMEASIDENTIFIER___Note Methods

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool ___PACKAGENAMEASIDENTIFIER___Note::Attack(const MusicDeviceNoteParams &inParams)
{ 
#if DEBUG_PRINT
    printf("___PACKAGENAMEASIDENTIFIER___Note::Attack %p %d\n",
           this, GetState());
#endif
    double sampleRate = SampleRate();
    phase = 0.;
    amp = 0.;
    maxamp = 0.4 * pow(inParams.mVelocity/127., 3.); 
    up_slope = maxamp / (0.1 * sampleRate);
    dn_slope = -maxamp / (0.9 * sampleRate);
    fast_dn_slope = -maxamp / (0.005 * sampleRate);
    return true;
}

void ___PACKAGENAMEASIDENTIFIER___Note::Release(UInt32 inFrame)
{
    SynthNote::Release(inFrame);
#if DEBUG_PRINT
    printf("___PACKAGENAMEASIDENTIFIER___Note::Release %p %d\n",
           this, GetState());
#endif
}

void ___PACKAGENAMEASIDENTIFIER___Note::FastRelease(UInt32 inFrame) // voice is being stolen.
{
    SynthNote::Release(inFrame);
#if DEBUG_PRINT
    printf("___PACKAGENAMEASIDENTIFIER___Note::Release %p %d\n",
           this, GetState());
#endif
}

Float32 ___PACKAGENAMEASIDENTIFIER___Note::Amplitude()
{
    return amp;
}

void ___PACKAGENAMEASIDENTIFIER___Note::Kill(UInt32 inFrame) // voice is being stolen.
{
    SynthNote::Kill(inFrame);
#if DEBUG_PRINT
    printf("___PACKAGENAMEASIDENTIFIER___Note::Kill %p %d\n",
           this, GetState());
#endif
}

OSStatus ___PACKAGENAMEASIDENTIFIER___Note::Render(UInt64           inAbsoluteSampleFrame,
                                                UInt32            inNumFrames,
                                                AudioBufferList **inBufferList,
                                                UInt32            inOutBusCount)
{
    float *left, *right;
        
    // ___PACKAGENAMEASIDENTIFIER___Note only writes into the first bus regardless of what is handed to us.
    const int bus0 = 0;
    int numChans = inBufferList[bus0]->mNumberBuffers;
    if (numChans > 2)
        return -1;
        
    left = (float*)inBufferList[bus0]->mBuffers[0].mData;
    right = numChans == 2 ? (float*)inBufferList[bus0]->mBuffers[1].mData : 0;

    double sampleRate = SampleRate();
    double freq = Frequency() * (2 * M_PI / sampleRate);

        
#if DEBUG_PRINT_RENDER
    printf("___PACKAGENAMEASIDENTIFIER___Note::Render %p %d %g %g\n",
           this, GetState(), phase, amp);
#endif
    switch (GetState()) {

    case kNoteState_Attacked :
    case kNoteState_Sostenutoed :
    case kNoteState_ReleasedButSostenutoed :
    case kNoteState_ReleasedButSustained :
        for (UInt32 frame=0; frame<inNumFrames; frame++) {
            if (amp < maxamp)
                amp += up_slope;
            float sp = sinf(phase);
            float sp2 = sp * sp;
            float sp5 = sp2 * sp2 * sp;
            float out = sp5 * amp;
            phase += freq;
            if (phase > 2 * M_PI)
                phase -= 2 * M_PI;
            left[frame] += out;
            if (right)
                right[frame] += out;
        }
        break;
                        
    case kNoteState_Released :
        {
            UInt32 endFrame = 0xFFFFFFFF;
            for (UInt32 frame=0; frame<inNumFrames; frame++) {
                if (amp > 0.0)
                    amp += dn_slope;
                else if (endFrame == 0xFFFFFFFF)
                    endFrame = frame;
                float sp = sinf(phase);
                float sp2 = sp * sp;
                float sp5 = sp2 * sp2 * sp;
                float out = sp5 * amp;
                phase += freq;
                left[frame] += out;
                if (right)
                    right[frame] += out;
            }
            if (endFrame != 0xFFFFFFFF) {
#if DEBUG_PRINT
                printf("___PACKAGENAMEASIDENTIFIER___Note::NoteEnded  %p %d %g %g\n",
                       this, GetState(), phase, amp);
#endif
                NoteEnded(endFrame);
            }
        }
        break;
                        
    case kNoteState_FastReleased :
        {
            UInt32 endFrame = 0xFFFFFFFF;
            for (UInt32 frame=0; frame<inNumFrames; ++frame) {
                if (amp > 0.0)
                    amp += fast_dn_slope;
                else if (endFrame == 0xFFFFFFFF)
                    endFrame = frame;
                float sp = sinf(phase);
                float sp2 = sp * sp;
                float sp5 = sp2 * sp2 * sp;
                float out = sp5 * amp;
                phase += freq;
                left[frame] += out;
                if (right)
                    right[frame] += out;
            }
            if (endFrame != 0xFFFFFFFF) {
#if DEBUG_PRINT
                printf("___PACKAGENAMEASIDENTIFIER___Note::NoteEnded  %p %d %g %g\n",
                       this, GetState(), phase, amp);
#endif
                NoteEnded(endFrame);
            }
        }
        break;

    default :
        break;
    }

    return noErr;
}
