# Monte Carlo Path Tracing
A path tracer using Monte Carlo for image rendering written in C.



## Installation

### Build 
```bash
   cmake -B build
   make -C build
   ```

### Run 
```bash
    mkdir -p image
    mkdir -p performance
    mkdir -p performance/measures

    export BOUNCES=number_of_bounces #(optional; = 26 by default)
    ./build/ppm width height amount_of_sample 
```



## Experiments
We decide to focus our model on runtime optimization. For that, we use 2 protocols : one for the runtime itself and another for the convergence speed measurment. In fact, some optimizations to decrease the runtime can drastically affect the convergence speed.


### Protocol 1: The runtime

#### Objective
The goal of this experiment is to measure the performances of the Path Tracer by focusing on the runtime. It will be measured only on the rendering phase, not the scene initialization and the time computation neather. 

#### Requierment
   - Compiler: gcc / clang
   - CMake
   - mpich 4.3.0

#### Setting up
   - Width and Height resolution: WxH
   - Number of samples: N
   - Number of bounces: B
   - Benshmark, homemaid or an inclued one

#### Time Mesurement
The measure of runtime is already in the path tracer, using the clock.h library. Indeed, the time will be get with the function:
'''c
clock_gettime();
'''
called 2 times to get time at start and the end. then, they will be substracted to get the runtime:
'''c
int start, end;
clock_gettime(CLOCK_MONOTONIC, &start);
...
clock_gettime(CLOCK_MONOTONIC, &end);
int runtime = end - start;
'''


The runtime will be exported in the file runtime_by_samplings.csv in performance/measures converted in seconds.
     




