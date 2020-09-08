//
// Created by Xu Lin on 8/9/20.
//
#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "CustomTypes.h"

class Variables {
private:
    VAR_NAME_SET* data;
public:
    VAR_NAME_SET* GetAllVariables();
};

#endif