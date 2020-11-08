# libdsp
A library providing some common DSP functions

This was made as a standalone (no dependencies like volk required) port of GNU Radio blocks I need in some projects, but without requiring the full GNU Radio library and dependencies. Thus, this library is kept lightweight (the current .so is under 100kB), 0 dependencies apart from C++ standard ones, making it a much better choice for non-experimental projects where you want to keep things... Slim :).   

All "blocks" use a similar work function, taking an input and output buffer + the input size, returning the number of output elements. Properly feeding / reading samples for the "block" to work properly is up to the user.    
```size_t work(TYPE_IN *in, size_t length, TYPE_OUT *out);```

**All credits goes to the GNU Radio and VOLK project.**

### Features
Currently this library contains the following "blocks" or functions (CC = Complex Float, FF = Float) :
- AGC, CC
- DC Blocker, CC
- Costas Loop, CC
- Clock Recovery MM, CC and FF
- Moving Average, CC and FF
- Root-Raised Cosine FIR generating fuction
- MMSE FIR Interpolator (required by the clock recovery), CC and FF
- Noise Source, CC and FF
- Random number generator (used by the Noise Source)
- Various utils functions from VOLK and GNU Radio

### Building
This project uses CMake, so all you need to do is those commands :

```
mkdir build && cd build
cmake ..
sudo make install
```