VST Implementation of WaveSpace Synthesizer using JUCE

Unfortunately, we couldn't find a straightforward way to distribute the VST version of WaveSpace Synthesizer (WSS). Therefore, to utilize WSS, you'll need to compile it using cmake.
Instructions:
1. Place the JUCE folder in the root directory. (https://github.com/juce-framework/JUCE)
2. Place the LibTorch folder in the root directory. (In case you have M1, https://github.com/mlverse/libtorch-mac-m1)
3. Create a folder named "build" (execute mkdir build in the terminal).
4. Navigate to the build folder (cd build).
5. Execute cmake (cmake ..).
6. Run the make command (make).

Check out WSSCmake/build/AudioPlugin_artefacts where you should find the VST, AU, and standalone app for WaveSpace Synthesizer.

Please note: This version of WaveSpace Synthesizer is still unstable, and there are many updates pending. Feel free to reach out if you encounter any issues during the compilation process or have any suggestions for improvemen

Enjoy using WaveSpace Synthesizer!
