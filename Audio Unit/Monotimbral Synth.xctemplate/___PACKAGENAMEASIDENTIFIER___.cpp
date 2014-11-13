//
//  ___FILENAME___
//  ___PACKAGENAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

/*
	This is a test implementation of a sin wave synth using AUInstrumentBase classes
	
	It illustrates a basic usage of these classes
	
	It artificially limits the number of notes at one time to 12, so the note-stealing 
	algorithm is used - you should know how this works!
	
	Most of the work you need to do is defining a Note class (see ___PACKAGENAMEASIDENTIFIER___Note). AUInstrument manages the
	creation and destruction of notes, the various stages of a note's lifetime.

	Alot of printfs have been left in (but are if'def out)
	These can be useful as you figure out how this all fits together. This is true in the AUInstrumentBase
	classes as well; simply define DEBUG_PRINT to 1 and this turns all this on
	
	The project also defines CA_AUTO_MIDI_MAP (OTHER_C_FLAGS). This adds all the code that is needed
	to map MIDI messages to specific parameter changes. This can be seen in AU Lab's MIDI Editor window
	CA_AUTO_MIDI_MAP is implemented in AUMIDIBase.cpp/.h
*/

#include "___FILEBASENAME___.h"
 
static const UInt32 kMaxActiveNotes = 8;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const double twopi = 2.0 * 3.14159265358979;

inline double pow5(double x) { double x2 = x*x; return x2*x2*x; }

#pragma mark ___PACKAGENAMEASIDENTIFIER___ Methods

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUMusicDeviceFactory, ___PACKAGENAMEASIDENTIFIER___)

static const AudioUnitParameterID kGlobalVolumeParam = 0;
static const CFStringRef kGlobalVolumeName = CFSTR("global volume");

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	___PACKAGENAMEASIDENTIFIER___::___PACKAGENAMEASIDENTIFIER___
//
// This synth has No inputs, One output
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
___PACKAGENAMEASIDENTIFIER___::___PACKAGENAMEASIDENTIFIER___(AudioUnit inComponentInstance)
	: AUMonotimbralInstrumentBase(inComponentInstance, 0, 1)
{
	CreateElements();
	
	Globals()->UseIndexedParameters(1); // we're only defining one param
	Globals()->SetParameter (kGlobalVolumeParam, 1.0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	___PACKAGENAMEASIDENTIFIER___::~___PACKAGENAMEASIDENTIFIER___
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
___PACKAGENAMEASIDENTIFIER___::~___PACKAGENAMEASIDENTIFIER___()
{
}


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

AUElement* ___PACKAGENAMEASIDENTIFIER___::CreateElement(	AudioUnitScope					scope,
									AudioUnitElement				element)
{
	switch (scope)
	{
		case kAudioUnitScope_Group :
			return new SynthGroupElement(this, element, new MidiControls);
		case kAudioUnitScope_Part :
			return new SynthPartElement(this, element);
		default :
			return AUBase::CreateElement(scope, element);
	}
}

OSStatus			___PACKAGENAMEASIDENTIFIER___::GetParameterInfo(		AudioUnitScope					inScope,
														AudioUnitParameterID			inParameterID,
														AudioUnitParameterInfo &		outParameterInfo)
{
	if (inParameterID != kGlobalVolumeParam) return kAudioUnitErr_InvalidParameter;
	if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;

	outParameterInfo.flags = SetAudioUnitParameterDisplayType (0, kAudioUnitParameterFlag_DisplaySquareRoot);
    outParameterInfo.flags += kAudioUnitParameterFlag_IsWritable;
	outParameterInfo.flags += kAudioUnitParameterFlag_IsReadable;

	AUBase::FillInParameterName (outParameterInfo, kGlobalVolumeName, false);
	outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
	outParameterInfo.minValue = 0;
	outParameterInfo.maxValue = 1.0;
	outParameterInfo.defaultValue = 1.0;
	return noErr;
}


#pragma mark ___PACKAGENAMEASIDENTIFIER___Note Methods

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void			___PACKAGENAMEASIDENTIFIER___Note::Release(UInt32 inFrame)
{
	SynthNote::Release(inFrame);
#if DEBUG_PRINT
	printf("___PACKAGENAMEASIDENTIFIER___Note::Release %p %d\n", this, GetState());
#endif
}

void			___PACKAGENAMEASIDENTIFIER___Note::FastRelease(UInt32 inFrame) // voice is being stolen.
{
	SynthNote::Release(inFrame);
#if DEBUG_PRINT
	printf("___PACKAGENAMEASIDENTIFIER___Note::Release %p %d\n", this, GetState());
#endif
}

void			___PACKAGENAMEASIDENTIFIER___Note::Kill(UInt32 inFrame) // voice is being stolen.
{
	SynthNote::Kill(inFrame);
#if DEBUG_PRINT
	printf("___PACKAGENAMEASIDENTIFIER___Note::Kill %p %d\n", this, GetState());
#endif
}

OSStatus		___PACKAGENAMEASIDENTIFIER___Note::Render(UInt64 inAbsoluteSampleFrame, UInt32 inNumFrames, AudioBufferList** inBufferList, UInt32 inOutBusCount)
{
	float *left, *right;
/* ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~	
	Changes to this parameter (kGlobalVolumeParam) are not being de-zippered; 
	Left as an exercise for the reader
 ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ */
	float globalVol = GetGlobalParameter(kGlobalVolumeParam);
	
	// ___PACKAGENAMEASIDENTIFIER___Note only writes into the first bus regardless of what is handed to us.
	const int bus0 = 0;
	int numChans = inBufferList[bus0]->mNumberBuffers;
	if (numChans > 2) return -1;
	
	left = (float*)inBufferList[bus0]->mBuffers[0].mData;
	right = numChans == 2 ? (float*)inBufferList[bus0]->mBuffers[1].mData : 0;

	double sampleRate = SampleRate();
	double freq = Frequency() * (twopi/sampleRate);

	
#if DEBUG_PRINT_RENDER
	printf("___PACKAGENAMEASIDENTIFIER___Note::Render %p %d %g %g\n", this, GetState(), phase, amp);
#endif
	switch (GetState())
	{
		case kNoteState_Attacked :
		case kNoteState_Sostenutoed :
		case kNoteState_ReleasedButSostenutoed :
		case kNoteState_ReleasedButSustained :
			{
				for (UInt32 frame=0; frame<inNumFrames; ++frame)
				{
					if (amp < maxamp) amp += up_slope;
					float out = pow5(sin(phase)) * amp * globalVol;
					phase += freq;
					if (phase > twopi) phase -= twopi;
					left[frame] += out;
					if (right) right[frame] += out;
				}
			}
			break;
			
		case kNoteState_Released :
			{
				UInt32 endFrame = 0xFFFFFFFF;
				for (UInt32 frame=0; frame<inNumFrames; ++frame)
				{
					if (amp > 0.0) amp += dn_slope;
					else if (endFrame == 0xFFFFFFFF) endFrame = frame;
					float out = pow5(sin(phase)) * amp * globalVol;
					phase += freq;
					left[frame] += out;
					if (right) right[frame] += out;
				}
				if (endFrame != 0xFFFFFFFF) {
#if DEBUG_PRINT
					printf("___PACKAGENAMEASIDENTIFIER___Note::NoteEnded  %p %d %g %g\n", this, GetState(), phase, amp);
#endif
					NoteEnded(endFrame);
				}
			}
			break;
			
		case kNoteState_FastReleased :
			{
				UInt32 endFrame = 0xFFFFFFFF;
				for (UInt32 frame=0; frame<inNumFrames; ++frame)
				{
					if (amp > 0.0) amp += fast_dn_slope;
					else if (endFrame == 0xFFFFFFFF) endFrame = frame;
					float out = pow5(sin(phase)) * amp * globalVol;
					phase += freq;
					left[frame] += out;
					if (right) right[frame] += out;
				}
				if (endFrame != 0xFFFFFFFF) {
#if DEBUG_PRINT
					printf("___PACKAGENAMEASIDENTIFIER___Note::NoteEnded  %p %d %g %g\n", this, GetState(), phase, amp);
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

