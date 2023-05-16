#ifndef READCL_H
#define READCL_H

#include<iostream>
#include<CL/cl.hpp>

void initReadCL();
cl::Kernel makeKernel(std::string path, std::string funcName);
const char *getErrorString(cl_int error);

#endif
