/** Copyright (C) 2013 Ultimaker - Released under terms of the AGPLv3 License */
#include "gettime.h"
#include <stdio.h>

namespace cura
{
    
TimeKeeper::TimeKeeper()
{
    restart();
}

double TimeKeeper::restart()
{
    double ret = getTime() - startTime;
    startTime = getTime();
    return ret;
}

double TimeKeeper::getMs(char * buf)
{
    double ret = getTime() - startTime;
    if (buf) {
        int t = static_cast<int>(ret * 1000);
        int h, m, s, ms;
        ms = t % 1000; t -= ms; t /= 1000; 
        s = t % 60; t -= s; t /= 60;
        m = t % 60; t -= m; t /= 60;
        h = t;
        if (h <= 0 && m <= 0) 
            sprintf(buf, "[%02d::%03d] ", s, ms);
        else if (h <= 0)
            sprintf(buf, "[%02d:%02d::%03d] ", m, s, ms);
        else
            sprintf(buf, "[%d:%02d:%02d::%03d] ", h, m, s, ms);
    }
    return ret;
}

double TimeKeeper::getMsRestart(char* buf)
{
    double ret = getTime() - startTime;
    startTime = getTime();
    if (buf) {
        int t = static_cast<int>(ret * 1000);
        int h, m, s, ms;
        ms = t % 1000; t -= ms; t /= 1000;
        s = t % 60; t -= s; t /= 60;
        m = t % 60; t -= m; t /= 60;
        h = t;
        if (h <= 0 && m <= 0)
            sprintf(buf, "[%02d::%03d] ", s, ms);
        else if (h <= 0)
            sprintf(buf, "[%02d:%02d::%03d] ", m, s, ms);
        else
            sprintf(buf, "[%d:%02d:%02d::%03d] ", h, m, s, ms);
    }
    return ret;
}

}//namespace cura
