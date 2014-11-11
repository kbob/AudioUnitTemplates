//
//  ___FILENAME___
//  ___PACKAGENAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

#include "AUEffectBase.h"
#include "___PACKAGENAMEASIDENTIFIER___Version.h"

#if AU_DEBUG_DISPATCHER
    #include "AUDebugDispatcher.h"
#endif

#ifndef _____FILEBASENAME_____
#define _____FILEBASENAME_____

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constants for parameters and factory presets
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark _______PACKAGENAMEASIDENTIFIER___ Parameter Constants

// Define constants to identify the parameters;
// define the total number of parameters.
enum {
    // kParameter_Example = 0,
    kNumberOfParameters
};

// Define name, default, min, and max for each parameter.
// static CFStringRef kParamName_Example    = CFSTR("Example");
// static const float kDefaultValue_Example = 2.0;
// static const float kMinimumValue_Example = 0.5;
// static const float kMaximumValue_Example = 20.0;

#pragma mark _______PACKAGENAMEASIDENTIFIER___ Factory Preset Constants

// Define constants to identify factory presets.
enum {
    kPreset_Default = 0,
    // kPreset_Example2 = 1,
    kNumberOfPresets
};

// Define values for each preset/parameter pair.
// static const float kParameter_Preset_Default_Example  = 2.0;
// static const float kParameter_Preset_Example2_Example = 3.0;

// Define the presets.
static AUPreset kPresets [kNumberOfPresets] = {
    { kPreset_Default, CFSTR("Factory Default") },
    // { kPreset_Example2, CFSTR("Example 2") },
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ___PACKAGENAMEASIDENTIFIER___ class
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark _______PACKAGENAMEASIDENTIFIER___

class ___PACKAGENAMEASIDENTIFIER___ : public AUEffectBase {

public:
    ___PACKAGENAMEASIDENTIFIER___ (AudioUnit component);

#if AU_DEBUG_DISPATCHER
    virtual ~___PACKAGENAMEASIDENTIFIER___ () {delete mDebugDispatcher;}
#endif

    virtual AUKernelBase *NewKernel ();

    virtual ComponentResult GetParameterInfo (
            AudioUnitScope          inScope,
            AudioUnitParameterID    inParameterID,
            AudioUnitParameterInfo &outParameterInfo
    );

    virtual ComponentResult GetParameterValueStrings (
            AudioUnitScope          inScope,
            AudioUnitParameterID    inParameterID,
            CFArrayRef             *outStrings
    );

    virtual ComponentResult GetPropertyInfo (
            AudioUnitPropertyID     inID,
            AudioUnitScope          inScope,
            AudioUnitElement        inElement,
            UInt32                 &outDataSize,
            Boolean                &outWritable
    );

    virtual ComponentResult GetProperty (
            AudioUnitPropertyID     inID,
            AudioUnitScope          inScope,
            AudioUnitElement        inElement,
            void                   *outData
    );

    // Report that the audio unit supports the
    // kAudioUnitProperty_TailTime property.
    virtual bool SupportsTail ()
    {
        return true;
    }

    // Provide the audio unit version information.
    virtual ComponentResult Version ()
    {
        return k___PACKAGENAMEASIDENTIFIER___Version;
    }

    // Declare the GetPresets method (for setting up the factory
    // presets), overriding the method from the AUBase superclass.
    virtual ComponentResult GetPresets (
            CFArrayRef             *outData
    ) const;

    // Declare the NewFactoryPresetSet method (for setting a factory
    // preset when requested by the host application), overriding the
    // method from the AUBase superclass.
    virtual OSStatus NewFactoryPresetSet (
            const AUPreset         &inNewFactoryPreset
    );

protected:
    class Kernel;

};

#endif
