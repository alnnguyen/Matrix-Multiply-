#!/bin/bash
set -euo pipefail

# Always run from the project root (mm-bench), wherever the script is called from
cd "$(dirname "$0")/.."

# Rebuild everything fresh so we never benchmark an old binary
make clean
make

mkdir -p results
OUTPUT="results/results.csv"
SYSINFO="results/system_info.txt"
RUNS=5

# Record the system setup, for reproducibility
{
    echo "Date: $(date)"
    echo "Host: $(hostname)"
    echo
    echo "== Compiler =="
    gcc --version | head -1
    echo
    echo "== CPU =="
    lscpu
} > "$SYSINFO"

# CSV header (overwrites any old results)
echo "build,version,n,run,seconds,checksum" > "$OUTPUT"

for executable in mm_O0 mm_O2 mm_O3; do
    for version in ijk ikj; do
        for size in 128 256 512; do
            for run in $(seq 1 "$RUNS"); do
                # Program prints: version,n,seconds,checksum
                line=$("./$executable" "$version" "$size")
                v=${line%%,*}       # version (text before the first comma)
                rest=${line#*,}     # everything after the first comma: n,seconds,checksum
                n=${rest%%,*}       # n
                rest=${rest#*,}     # seconds,checksum
                echo "$executable,$v,$n,$run,$rest" | tee -a "$OUTPUT"
            done
        done
    done
done

echo "Done: $(($(wc -l < "$OUTPUT") - 1)) runs saved to $OUTPUT"
echo "System info saved to $SYSINFO"