//
// Created by Xu Lin on 8/9/20.
//
#ifndef _PROCEDURES_H_
#define _PROCEDURES_H_

#include <iostream>
#include <cstdlib>
#include <unordered_set>
#include <CustomTypes.h>

class Procedures {
 private:
   PROC_NAME_SET* data;
 public:
    PROC_NAME_SET* GetAllProcedures();
};
#endif
