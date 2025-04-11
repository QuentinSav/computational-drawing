#ifndef SPIROGRAPH_H
#define SPIROGRAPH_H

#include "DrawingDevice.h"

class Spirograph : protected DrawingDevice
{
public:
    Spirograph();
    virtual void setParam();
    virtual void run();
};

#endif // SPIROGRAPH_H