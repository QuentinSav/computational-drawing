#include "DrawingDevice.h"
#include "Pintograph.h"
#include <filesystem>


int main(int argc, char** argv) 
{ 

    Pintograph object(THREE_CRANKS_LINKAGE);
    object.setPredefinedParam(DEFAULT_3);
    object.run();

    return 0; 
}
