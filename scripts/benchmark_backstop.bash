#! /bin/bash
baseline="$1"
contender="$2"

if [ ! -f "$baseline" ]; then
    echo "ERROR: baseline file $baseline does not exist"
    exit 1
fi

if [ ! -f "$contender" ]; then
    echo "ERROR: contender file $contender does not exist"
    exit 1
fi

if [ "$#" -eq 3 ]; then
    alpha="$3"
    alpha=$(echo "$alpha" | awk '{print $1+0}')
else
    alpha=0.01
fi

output=$(/ext/google_benchmark/tools/compare.py --no-color benchmarks "$baseline" "$contender")
echo "$output"
pvalue=$(echo "$output" | grep -E '^[A-Za-z0-9_]*pvalue[[:space:]]' |  awk '{print $3}')

# convert string pvalue to float:
pvalue=$(echo "$pvalue" | awk '{print $1+0}')

# if pvalue larger alpha, exit 0
if awk "BEGIN {exit !($pvalue >= $alpha)}"; then
    echo "pvalue is $pvalue, larger equals than alpha=$alpha, no significant change. OK"
    exit 0
else
    echo "pvalue is $pvalue, smaller than alpha=$alpha - check if performance change improved or degraded ..."
fi

mean=$(echo "$output" | grep -E '^[A-Za-z0-9_]*mean[[:space:]]' |  awk '{print $3}')
mean=$(echo "$mean" | awk '{print $1+0}')

if awk "BEGIN {exit !($mean > 0)}"; then
    echo "mean is positive, performance regression by factor=$mean"
    echo "FAIL"
    exit 1
else
    echo "mean is negative, performance improvement by factor=$mean"
    echo "OK"
    exit 0
fi

