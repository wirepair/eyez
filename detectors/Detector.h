#ifndef DETECTOR_H
#define DETECTOR_H

class Detector
{
public:
    /** Any initialization of a detector should go here **/
    virtual bool Init() = 0;

    /** iff true, x & y contain the normalized (-1.0f, 1.0f) xy center coords of detection  **/
    virtual bool Detect(float* x, float* y) = 0;
};

#endif // DETECTOR_H