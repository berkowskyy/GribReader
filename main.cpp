#include "gribdecoder.h"

int main()
{
    GribDecoder decoder;
    decoder.readGribFile("all.grib");
    return 0;
}
