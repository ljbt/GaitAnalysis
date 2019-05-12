#include "Mask.h"

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>

Mask::Mask() // default constructor
{
    _minValue = 0;
    _maxValue = 255;
}

Mask::Mask(int min, int max)
{
    if(min <= max)
    {
        _minValue = min;
        _maxValue = max;
    }
    else
    {
        _minValue = max;
        _maxValue = min;
    }
}

Mask::~Mask()
{
    // nothing to release yet
}

int Mask::min()
{
    return _minValue;
}

int Mask::max()
{
    return _maxValue;
}


MaskHSV::MaskHSV()
{
    _H = Mask(0,255);
    _S = Mask(0,255);
    _V = Mask(0,255);
}

MaskHSV::MaskHSV(Mask H, Mask S, Mask V)
{
    _H = H;
    _S = S;
    _V = V;
}

MaskHSV::~MaskHSV()
{
    // nothing to release yet
}

Mask MaskHSV::maskH()
{
    return _H;
}

Mask MaskHSV::maskS()
{
    return _S;
}

Mask MaskHSV::maskV()
{
    return _V;
}