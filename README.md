### Monte Carlo Path Tracing
A path tracer using Monte Carlo for image rendering written in C.

#### Build: <br>
   cmake -B build
   make -C build
   
<br>

#### Run: <br>
    mkdir -p image
    mkdir -p performance

    export BOUNCES=number_of_bounces (facultative)
    ./build/ppm width height amount_of_sample
    
<br>
