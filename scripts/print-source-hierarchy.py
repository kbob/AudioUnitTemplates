import os

aubase_path = '''
AUPublic/AUBase/AUBase.cpp
AUPublic/AUBase/AUBase.h
AUPublic/AUBase/AUDispatch.cpp
AUPublic/AUBase/AUDispatch.h
AUPublic/AUBase/AUInputElement.cpp
AUPublic/AUBase/AUInputElement.h
AUPublic/AUBase/AUOutputElement.cpp
AUPublic/AUBase/AUOutputElement.h
AUPublic/AUBase/AUPluginDispatch.cpp
AUPublic/AUBase/AUPluginDispatch.h
AUPublic/AUBase/AUScopeElement.cpp
AUPublic/AUBase/AUScopeElement.h
AUPublic/AUBase/ComponentBase.cpp
AUPublic/AUBase/ComponentBase.h
AUPublic/Utility/AUBuffer.cpp
AUPublic/Utility/AUBuffer.h
'''.split()

aufx_paths = '''
AUPublic/OtherBases/AUEffectBase.cpp
AUPublic/OtherBases/AUEffectBase.h
AUPublic/Utility/AUBaseHelper.cpp
AUPublic/Utility/AUBaseHelper.h
AUPublic/Utility/AUSilentTimeout.h
PublicUtility/CAAtomic.h
PublicUtility/CAAtomicStack.h
PublicUtility/CAAudioChannelLayout.cpp
PublicUtility/CAAudioChannelLayout.h
PublicUtility/CAAutoDisposer.h
PublicUtility/CABufferList.cpp
PublicUtility/CABufferList.h
PublicUtility/CAByteOrder.h
PublicUtility/CADebugMacros.cpp
PublicUtility/CADebugMacros.h
PublicUtility/CADebugPrintf.h
PublicUtility/CADebugger.cpp
PublicUtility/CADebugger.h
PublicUtility/CAException.h
PublicUtility/CAGuard.cpp
PublicUtility/CAGuard.h
PublicUtility/CAHostTimeBase.cpp
PublicUtility/CAHostTimeBase.h
PublicUtility/CALogMacros.h
PublicUtility/CAMath.h
PublicUtility/CAMutex.cpp
PublicUtility/CAMutex.h
PublicUtility/CAReferenceCounted.h
PublicUtility/CAStreamBasicDescription.cpp
PublicUtility/CAStreamBasicDescription.h
PublicUtility/CAThreadSafeList.h
PublicUtility/CAVectorUnit.cpp
PublicUtility/CAVectorUnit.h
PublicUtility/CAVectorUnitTypes.h
PublicUtility/CAXException.cpp
PublicUtility/CAXException.h
'''.split()

aumf_paths = '''
AUPublic/AUInstrumentBase/AUInstrumentBase.cpp
AUPublic/AUInstrumentBase/AUInstrumentBase.h
AUPublic/AUInstrumentBase/LockFreeFIFO.h
AUPublic/AUInstrumentBase/MIDIControlHandler.h
AUPublic/AUInstrumentBase/SynthElement.cpp
AUPublic/AUInstrumentBase/SynthElement.h
AUPublic/AUInstrumentBase/SynthEvent.h
AUPublic/AUInstrumentBase/SynthNote.cpp
AUPublic/AUInstrumentBase/SynthNote.h
AUPublic/AUInstrumentBase/SynthNoteList.cpp
AUPublic/AUInstrumentBase/SynthNoteList.h
AUPublic/OtherBases/AUMIDIBase.cpp
AUPublic/OtherBases/AUMIDIBase.h
AUPublic/OtherBases/MusicDeviceBase.cpp
AUPublic/OtherBases/MusicDeviceBase.h
AUPublic/Utility/AUBaseHelper.cpp
AUPublic/Utility/AUBaseHelper.h
AUPublic/Utility/AUMIDIDefs.h
PublicUtility/CAAUMIDIMap.cpp
PublicUtility/CAAUMIDIMap.h
PublicUtility/CAAUMIDIMapManager.cpp
PublicUtility/CAAUMIDIMapManager.h
PublicUtility/CAAtomic.h
PublicUtility/CAAtomicStack.h
PublicUtility/CAByteOrder.h
PublicUtility/CAAudioChannelLayout.cpp
PublicUtility/CAAudioChannelLayout.h
PublicUtility/CAAutoDisposer.h
PublicUtility/CABufferList.cpp
PublicUtility/CABufferList.h
PublicUtility/CADebugMacros.h
PublicUtility/CADebugPrintf.cpp
PublicUtility/CADebugPrintf.h
PublicUtility/CAException.h
PublicUtility/CAHostTimeBase.cpp
PublicUtility/CAHostTimeBase.h
PublicUtility/CAMath.h
PublicUtility/CAMutex.cpp
PublicUtility/CAMutex.h
PublicUtility/CAReferenceCounted.h
PublicUtility/CAStreamBasicDescription.cpp
PublicUtility/CAStreamBasicDescription.h
PublicUtility/CAThreadSafeList.h
PublicUtility/CAVectorUnit.cpp
PublicUtility/CAVectorUnit.h
PublicUtility/CAVectorUnitTypes.h
PublicUtility/CAXException.cpp
PublicUtility/CAXException.h
'''.split()

paths = aumf_paths

x = '''
      <key>%(path)s</key>
      <dict>
        <key>Path</key>
        <string>%(path)s</string>
        <key>Group</key>
        <array>
          %(dstrings)s
        </array>
      </dict>

'''.strip('\n')

for path in paths:
    print('      <string>%s</string>' % path)

print()

for path in paths:
    dir, file = os.path.split(path)
    dstrings = '\n          '.join('<string>%s</string>' % d
                                   for d in dir.split('/'))
    print(x % locals())

