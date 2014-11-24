//
//  ___FILENAME___
//  ___PACKAGENAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

#ifndef _____FILEBASENAME_____
#define _____FILEBASENAME_____

#include "AUInstrumentBase.h"
#include "___PACKAGENAMEASIDENTIFIER___Version.h"

static const UInt32 kNumNotes = 12;

// Define constants to identify the parameters.
enum Parameter {
    // kParameter_Zero = 0,
    // kParameter_One  = 1,
    kNumberOfParameters
};

// Define constants to identify factory presets.
enum Preset {
    kPreset_Default = 0,
    // kPreset_One  = 1,
    // kPreset_Two  = 2,
    kNumberOfPresets
};

struct ___PACKAGENAMEASIDENTIFIER___Note : public SynthNote {

 public:
    virtual         ~___PACKAGENAMEASIDENTIFIER___Note() {}

    virtual bool     Attack(const MusicDeviceNoteParams &inParams);
    virtual void     Kill(UInt32 inFrame); // voice is being stolen.
    virtual void     Release(UInt32 inFrame);
    virtual void     FastRelease(UInt32 inFrame);
    virtual Float32  Amplitude();
    virtual OSStatus Render(UInt64            inAbsoluteSampleFrame,
                            UInt32            inNumFrames,
                            AudioBufferList** inBufferList,
                            UInt32            inOutBusCount);

 private:
        double phase, amp, maxamp;
        double up_slope, dn_slope, fast_dn_slope;

};

class ___PACKAGENAMEASIDENTIFIER___ : public AUMonotimbralInstrumentBase
{
public:
                       ___PACKAGENAMEASIDENTIFIER___(AudioUnit inComponentInstance);
    virtual           ~___PACKAGENAMEASIDENTIFIER___();
                                
    virtual OSStatus   Initialize();
    virtual void       Cleanup();
    virtual OSStatus   Version() { return k___PACKAGENAMEASIDENTIFIER___Version; }

    virtual AUElement *CreateElement(AudioUnitScope   scope,
                                     AudioUnitElement element);

    virtual OSStatus   GetParameterInfo(AudioUnitScope          inScope,
                                    AudioUnitParameterID    inParameterID,
                                    AudioUnitParameterInfo &outParameterInfo);

private:
    ___PACKAGENAMEASIDENTIFIER___Note mNotes[kNumNotes];

};

#endif /* !_____FILEBASENAME_____ */
