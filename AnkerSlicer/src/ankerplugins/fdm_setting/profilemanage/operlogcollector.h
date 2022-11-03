#pragma once
#include "operatelog.h"
#include "fdmmachineprofilemanager.h"
#include "fdmmaterialprofilemanager.h"
#include "fdmparameterprofilemanager.h"


class OperLogCollector
{
public:
    static OperLogCollector* instance()
    {
        static OperLogCollector obj;
        return &obj;
    }

    //
    OperateLogPtr getLog();


};

