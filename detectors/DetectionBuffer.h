#ifndef DETECTION_BUFFER_H
#define DETECTION_BUFFER_H

#include <memory>
#include "Point.h"

// Adapted from:
// https://github.com/embeddedartistry/embedded-resources/blob/master/examples/cpp/circular_buffer.cpp
class DetectionBuffer 
{
public:
    DetectionBuffer(size_t size) : size(size), points(std::unique_ptr<Point[]>(new Point[size])) {};

    void Push(float x, float y);

    void Clear();

    bool IsEmpty();
    
    bool IsFull();

    size_t Size();

    Point Average();

private:
    size_t head = 0;
    size_t tail = 0;
    size_t size;
    std::unique_ptr<Point[]> points;
}

void DetectionBuffer::Push(float x, float y)
{
    std::unique_ptr<Point> newPoint(x, y);
    points[head] = newPoint;
    head = (head + 1) % size;
    if (head == tail)
    {
        tail = (tail + 1) % size;
    }
}

void DetectionBuffer::Clear()
{
    head = tail;
}

bool DetectionBuffer::IsEmpty()
{
    return head == tail;
}

bool DetectionBuffer::IsFull()
{
    return ((head +1) % size) == tail;
}

size_t DetectionBuffer::Size()
{
    return size - 1;
}

#endif // DETECTION_BUFFER_H