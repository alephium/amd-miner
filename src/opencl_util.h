#ifndef ALEPHIUM_OPENCL_UTIL_H
#define ALEPHIUM_OPENCL_UTIL_H

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define CL_TARGET_OPENCL_VERSION 120

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define TRY(x)                                                                                  \
    {                                                                                           \
        cl_int err = (x);                                                                       \
        if (err != CL_SUCCESS)                                                                  \
        {                                                                                       \
            printf("opencl error %d calling '%s' (%s line %d)\n", err, #x, __FILE__, __LINE__); \
        }                                                                                       \
    }

#define CHECK(x)                                                                                \
    {                                                                                           \
        x;                                                                                      \
        if (err != CL_SUCCESS)                                                                  \
        {                                                                                       \
            printf("opencl error %d calling '%s' (%s line %d)\n", err, #x, __FILE__, __LINE__); \
        }                                                                                       \
    }

void log_build_info(cl_program program, cl_device_id device_id)
{
    char *build_log;
    size_t log_size;
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
    build_log = new char[log_size + 1];
    // Second call to get the log
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL);
    build_log[log_size] = '\0';
    printf("==== build === %s\n", build_log);
    delete[] build_log;
}

#endif
