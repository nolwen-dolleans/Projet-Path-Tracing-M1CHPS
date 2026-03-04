#!/bin/bash

if [ -f "performance/measures/mpi_bounces.json" ]; then
    rm "performance/measures/mpi_bounces.json"
fi
OUTPUT_FILE="performance/measures/mpi_bounces.json"

mkdir -p "$(dirname "$OUTPUT_FILE")"

echo "[" > "$OUTPUT_FILE"

BOUNCES=(1 5 10 15 20 25 30)

for i in "${!BOUNCES[@]}"; do
    ITER=${BOUNCES[$i]}

    echo "Running MPI program with bounces=$ITER..."

    # Collect CPU cycles, instructions, cache references, cache misses, memory usage
    PERF_OUTPUT=$(BOUNCES="$ITER" perf stat -e cycles,instructions,cache-references,cache-misses -o /dev/stdout -- \
        /usr/bin/time -v mpirun -np 8 ./ppm 32 800 600 500 2>&1)

    CPU_CYCLES=$(echo "$PERF_OUTPUT" | grep "cycles" | awk '{print $1}' | tr -d ',')
    INSTRUCTIONS=$(echo "$PERF_OUTPUT" | grep "instructions" | awk '{print $1}' | tr -d ',')
    CACHE_REFERENCES=$(echo "$PERF_OUTPUT" | grep "cache-references" | awk '{print $1}' | tr -d ',')
    CACHE_MISSES=$(echo "$PERF_OUTPUT" | grep "cache-misses" | awk '{print $1}' | tr -d ',')
    RAW_TIME=$(echo "$PERF_OUTPUT" | grep "Elapsed (wall clock) time" | awk '{print $8}')

    # Convert time format to total seconds (supports M:SS or H:MM:SS)
    IFS=: read -r t1 t2 t3 <<< "$RAW_TIME"

    if [ -z "$t3" ]; then
        # Format is M:SS
        ELAPSED_TIME=$(echo "$t1 * 60 + $t2" | bc)
    else
        # Format is H:MM:SS
        ELAPSED_TIME=$(echo "$t1 * 3600 + $t2 * 60 + $t3" | bc)
    fi
    MAX_MEMORY=$(echo "$PERF_OUTPUT" | grep "Maximum resident set size" | awk '{print $6}')

    JSON_ENTRY=$(cat <<EOF
{
  "bounces": $ITER,
  "cpu_cycles": $CPU_CYCLES,
  "instructions": $INSTRUCTIONS,
  "cache_references": $CACHE_REFERENCES,
  "cache_misses": $CACHE_MISSES,
  "elapsed_time_sec": $ELAPSED_TIME,
  "max_memory_kb": $MAX_MEMORY
}
EOF
)

    # Add comma if not last element
    if [ $i -lt $((${#BOUNCES[@]} - 1)) ]; then
        JSON_ENTRY="$JSON_ENTRY,"
    fi

    # Append to output JSON file
    echo "$JSON_ENTRY" >> "$OUTPUT_FILE"

done

# Close JSON array
echo "]" >> "$OUTPUT_FILE"

echo "Performance data saved to $OUTPUT_FILE"

# Notif end of execution
powershell.exe -Command "[console]::beep(250,300)"