#!/bin/bash
#SBATCH --nodes=1
#SBATCH --time=00:10:00
#SBATCH --cpus-per-task=32
#SBATCH -p sequana_cpu_dev
#SBATCH -J trap3
#SBATCH --exclusive

echo "Nó alocado: $SLURM_JOB_NODELIST"

cd /scratch/pex1272-ufersa/iverton.emiquison/PCD/questao_trap3/

module load gcc/14.2.0_sequana
module load openmpi/gnu/4.1.1_sequana

make clean
make

if [ ! -f omp_trap3 ]; then
    echo "Erro ao compilar o programa!"
    exit 1
fi

THREADS=(2 4 8)
TRAPS=(100 1000)
SCHEDULES=("static" "dynamic" "guided")

echo "=== Iniciando execuções com schedule(runtime) ==="
echo ""

for sched in "${SCHEDULES[@]}"; do
    echo "=============================================="
    echo "        >>> Testando schedule: $sched <<<"
    echo "=============================================="
    export OMP_SCHEDULE="$sched"

    for t in "${THREADS[@]}"; do
        echo ""
        echo "---- Threads = $t ----"
        for n in "${TRAPS[@]}"; do
            echo ""
            echo "Executando com $t threads e n = $n trapezios"
            printf "0 10 %d\n" "$n" | ./omp_trap3 "$t"
            echo ""
        done
    done
    echo ""
done

echo "=== Fim ==="
