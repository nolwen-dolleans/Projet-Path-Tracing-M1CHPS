# Monte Carlo Path Tracing
A path tracer using Monte Carlo for image rendering written in C.

## Build 
```bash
   cmake -B build
   make -C build
   ```

## Run 
```bash
    mkdir -p image
    mkdir -p performance
    mkdir -p performance/measures

    export BOUNCES=number_of_bounces #(optional; = 26 by default)
    ./build/ppm width height amount_of_sample 
```
