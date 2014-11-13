//
//  ___FILENAME___
//  ___PACKAGENAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

// This file defines the ___PACKAGENAMEASIDENTIFIER___ class.

#include "___FILEBASENAME___.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AUDIOCOMPONENT_ENTRY(AUBaseFactory, ___PACKAGENAMEASIDENTIFIER___)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// The COMPONENT_ENTRY macro is required for the Mac OS X Component
// Manager to recognize and use the audio unit

// The constructor for new ___PACKAGENAMEASIDENTIFIER___ audio units
___PACKAGENAMEASIDENTIFIER___::___PACKAGENAMEASIDENTIFIER___ (AudioUnit component)
: AUEffectBase (component)
{

    // CreateElements, defined in the AUBase superclass, ensures that
    // the required audio unit elements are created and initialized.
    CreateElements ();

    // Invokes the use of an STL vector for parameter access.  See
    // AUBase/AUScopeElement.cpp
    Globals () -> UseIndexedParameters (kNumberOfParameters);

    // During instantiation, sets up the parameters according to
    // their defaults.  The parameter defaults should correspond
    // to the settings for the default preset.
    // SetParameter (
    //     kParameter_Example,
    //     kDefaultValue_Example
    // );

    // Also during instantiation, sets the preset menu to indicate
    // the default preset, which corresponds to the default
    // parameters. It's possible to set this so a fresh audio unit
    // indicates the wrong preset, so be careful to get it right.
    SetAFactoryPresetAsCurrent (
        kPresets[kPreset_Default]
    );

    #if AU_DEBUG_DISPATCHER
        mDebugDispatcher = new AUDebugDispatcher (this);
    #endif
}

#pragma mark ____Parameters

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      ___PACKAGENAMEASIDENTIFIER___::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Called by the audio unit's view; provides information needed for
// the view to display the audio unit's parameters

ComponentResult ___PACKAGENAMEASIDENTIFIER___::GetParameterInfo (
                AudioUnitScope          inScope,
                AudioUnitParameterID    inParameterID,
                AudioUnitParameterInfo &outParameterInfo
)
{
    ComponentResult result = noErr;

    // Add two flags to all parameters for the audio unit,
    // indicating to the host application that it should consider
    // all the audio unit's parameters to be readable and
    // writable.
    outParameterInfo.flags =
        kAudioUnitParameterFlag_IsWritable | kAudioUnitParameterFlag_IsReadable;

    if (inScope == kAudioUnitScope_Global) {
        switch (inParameterID) {

//         case kParameter_Example:
//             AUBase::FillInParameterName (
//                  outParameterInfo,
//                  kParamName_Tremolo_Freq,
//                  false
//              );
//             outParameterInfo.unit         = kAudioUnitParameterUnit_Hertz;
//             outParameterInfo.minValue     = kMinimumValue_Example;
//             outParameterInfo.maxValue     = kMaximumValue_Example;
//             outParameterInfo.defaultValue = kDefaultValue_Example;
//             outParameterInfo.flags       |=
//                 kAudioUnitParameterFlag_DisplayLogarithmic;
//             return noErr;

        }
    }
    return kAudioUnitErr_InvalidParameter;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      ___PACKAGENAMEASIDENTIFIER___::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Provides the strings for the Waveform popup menu in the generic view
ComponentResult ___PACKAGENAMEASIDENTIFIER___::GetParameterValueStrings (
        AudioUnitScope          inScope,
        AudioUnitParameterID    inParameterID,
        CFArrayRef             *outStrings
)
{

//     if ((inScope == kAudioUnitScope_Global) &&
//         (inParameterID == kParameter_Example)) {
//         if (outStrings == NULL)
//             return noErr;
//
//         // Define an array that contains the pop-up menu item names.
//         CFStringRef strings [] = {
//             kMenuItem_Example_First,
//             kMenuItem_Example_Second,
//         };
//
//         // Create a new immutable array containing the menu
//         // item names and place the array into the outStrings
//         // output parameter.
//         *outStrings = CFArrayCreate (
//             NULL,
//             (const void **) strings,
//             (sizeof strings / sizeof strings [0]),
//             NULL
//         );
//         return noErr;
//     }

    return kAudioUnitErr_InvalidParameter;
}

#pragma mark ____Properties

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      ___PACKAGENAMEASIDENTIFIER___::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ___PACKAGENAMEASIDENTIFIER___::GetPropertyInfo (
        AudioUnitPropertyID     inID,
        AudioUnitScope          inScope,
        AudioUnitElement        inElement,
        UInt32                          &outDataSize,
        Boolean                         &outWritable
) {
    // This audio unit doesn't define any custom properties, so it
    // uses this generic code for this method.
    return AUEffectBase::GetPropertyInfo (
        inID,
        inScope,
        inElement,
        outDataSize,
        outWritable
    );
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      ___PACKAGENAMEASIDENTIFIER___::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ___PACKAGENAMEASIDENTIFIER___::GetProperty (
        AudioUnitPropertyID inID,
        AudioUnitScope      inScope,
        AudioUnitElement    inElement,
        void               *outData
) {
    // This audio unit doesn't define any custom properties, so it
    // uses this generic code for this method.
    return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

#pragma mark ____Factory Presets

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      ___PACKAGENAMEASIDENTIFIER___::GetPresets
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// For users to be able to use the factory presets you define, you
// must add a generic implementation of the GetPresets method. The
// code here works for any audio unit that can support factory
// presets.
//
// The GetPresets method accepts a single parameter, a pointer to a
// CFArrayRef object. This object holds the factory presets array
// generated by this method. The array contains just factory preset
// numbers and names. The host application uses this array to set up
// its factory presets menu and when calling the NewFactoryPresetSet
// method.

ComponentResult ___PACKAGENAMEASIDENTIFIER___::GetPresets (
        CFArrayRef      *outData
) const {

        // Checks whether factory presets are implemented for this audio unit.
        if (outData == NULL) return noErr;

        // Instantiates a mutable Core Foundation array to hold the
        // factory presets.
        CFMutableArrayRef presetsArray = CFArrayCreateMutable (
                NULL,
                kNumberOfPresets,
                NULL
        );

        // Fills the factory presets array with values from the
        // definitions in the ___PACKAGENAMEASIDENTIFIER___.h file.
        for (int i = 0; i < kNumberOfPresets; ++i) {
                CFArrayAppendValue (
                        presetsArray,
                        &kPresets [i]
                );
    }

        // Stores the factory presets array at the outData location.
        *outData = (CFArrayRef) presetsArray;
        return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      ___PACKAGENAMEASIDENTIFIER___::NewFactoryPresetSet
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// The NewFactoryPresetSet method defines all the factory presets for
// an audio unit. Basically, for each preset, it invokes a series of
// SetParameter calls.
//
// This method takes a single argument of type AUPreset, a structure
//  containing a factory preset name and number.
OSStatus ___PACKAGENAMEASIDENTIFIER___::NewFactoryPresetSet (
        const AUPreset &inNewFactoryPreset
) {
    // Gets the number of the desired factory preset.
    SInt32 chosenPreset = inNewFactoryPreset.presetNumber;

    if (
        // Tests whether the desired factory preset is defined.
        chosenPreset == kPreset_Default
        // || chosenPreset = kPreset_Example2
    ) {

        // This 'for' loop, and the 'if' statement that follows it,
        // allow for noncontiguous preset numbers.
        for (int i = 0; i < kNumberOfPresets; ++i) {
            if (chosenPreset == kPresets[i].presetNumber) {

                //Selects the appropriate case statement based on
                //the factory preset number.

                switch (chosenPreset) {

                case kPreset_Default:
//                     SetParameter (
//                                   kParameter_Example,
//                                   kParameter_Preset_Default_Example
//                                   );
                    break;

//                 case kPreset_Example2:
//                     ...
//                     break;

                }

                // Updates the preset menu in the generic view to
                // display the new factory preset.
                SetAFactoryPresetAsCurrent (
                                            kPresets [i]
                                            );
                return noErr;
            }
        }
    }
    return kAudioUnitErr_InvalidProperty;
}



#pragma mark _______PACKAGENAMEASIDENTIFIER___::Kernel

class ___PACKAGENAMEASIDENTIFIER___::Kernel : public AUKernelBase {

public:
    Kernel (AUEffectBase *inAudioUnit);

    // *Required* overides for the process method for this effect
    // processes one channel of interleaved samples
    virtual void Process (
            const Float32  *inSourceP,
            Float32        *inDestP,
            UInt32          inFramesToProcess,
            UInt32          inNumChannels, // always equal to 1
            bool           &ioSilence
    );

    virtual void Reset ();

private:
    // declare member variables here.

    Float64 mSampleFrequency;
};

AUKernelBase *___PACKAGENAMEASIDENTIFIER___::NewKernel()
{
    return new ___PACKAGENAMEASIDENTIFIER___::Kernel(this);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      ___PACKAGENAMEASIDENTIFIER___::Kernel::Kernel()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This is the constructor for the
//  ___PACKAGENAMEASIDENTIFIER___::Kernel helper class, which holds
//  the DSP code for the audio unit. ___PACKAGENAMEASIDENTIFIER___ is
//  an n-to-n audio unit; one kernel object gets built for each
//  channel in the audio unit.
//
// The first line of the method consists of the constructor method
//  declarator and constructor-initializer. In addition to calling
//  the appropriate superclasses, this code initializes two member
//  variables:

___PACKAGENAMEASIDENTIFIER___::Kernel::Kernel (AUEffectBase *inAudioUnit )
    : AUKernelBase (inAudioUnit)
{
    mSampleFrequency = GetSampleRate ();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      ___PACKAGENAMEASIDENTIFIER___::Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void ___PACKAGENAMEASIDENTIFIER___::Kernel::Reset()
{
    // Reset kernel's state.
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      ___PACKAGENAMEASIDENTIFIER___::Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This method contains the DSP code.
void ___PACKAGENAMEASIDENTIFIER___::Kernel::Process (
        const Float32   *inSourceP,          // The audio sample input buffer.
        Float32         *inDestP,            // The audio sample output buffer.
        UInt32           inSamplesToProcess, // The number of samples in the
                                             // input buffer.
        UInt32           inNumChannels,      // The number of input channels.
                                             // This is always equal to 1,
                                             // because there is always one
                                             // kernel object instantiated
                                             // per channel of audio.
        bool            &ioSilence           // A Boolean flag indicating
                                             // whether the input to the audio
                                             // unit consists of silence,
                                             // with a TRUE value indicating
                                             // silence.
)
{
    // Ignores the request to perform the Process method if the input
    // to the audio unit is silence.
    if (!ioSilence) {
        for (UInt32 i = 0; i < inSamplesToProcess; i++)
            inDestP[i] = inSourceP[i];
    }
}


