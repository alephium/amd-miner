#ifndef ALEPHIUM_MINING_H
#define ALEPHIUM_MINING_H

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<double> duration_t;
typedef std::chrono::time_point<std::chrono::high_resolution_clock> time_point_t;

void CL_CALLBACK worker_kernel_callback(cl_event event, cl_int status, void *data);

void CL_CALLBACK _worker_kernel_callback(cl_event event, cl_int status, void *data)
{
    mining_worker_t *worker = (mining_worker_t *)data;
    clReleaseMemObject(worker->device_hasher);
    worker_kernel_callback(event, status, worker);
}

void start_worker_mining(mining_worker_t *worker)
{
    reset_worker(worker);

    cl_int err;
    time_point_t start = Time::now();

    size_t hasher_size = sizeof(blake3_hasher);
    worker->device_hasher = clCreateBuffer(worker->context, CL_MEM_READ_WRITE, hasher_size, NULL, NULL);
    TRY(clEnqueueWriteBuffer(worker->queue, worker->device_hasher, CL_FALSE, 0, hasher_size, worker->hasher, 0, NULL, NULL));
    TRY(clSetKernelArg(worker->kernel, 0, sizeof(cl_mem), &worker->device_hasher));
    TRY(clEnqueueNDRangeKernel(worker->queue, worker->kernel, 1, NULL, &(worker->grid_size), &(worker->block_size), 0, NULL, NULL));

    cl_event worker_completed;
    TRY(clEnqueueReadBuffer(worker->queue, worker->device_hasher, CL_FALSE, 0, hasher_size, worker->hasher, 0, NULL, &worker_completed));
    TRY(clSetEventCallback(worker_completed, CL_COMPLETE, worker_kernel_callback, worker));
   
    duration_t elapsed = Time::now() - start;
    // printf("=== mining time: %fs\n", elapsed.count());
}

#endif // ALEPHIUM_MINING_H
