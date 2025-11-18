#!/bin/bash
#SBATCH --nodes=4
#SBATCH --time=00:10:00
#SBATCH --cpus-per-task=24
#SBATCH -p sequana_cpu_dev
#SBATCH -J exemplo
#SBATCH --exclusive

echo $SLUM_JOB_NODELIST

cd /scratch/pex1272-ufersa/iverton.emiquison/PCD/questao_20/

module load gcc/14.2.0_sequana
module load openmpi/gnu/4.1.1_sequana

make clean
make

if [ ! -f omp_trap_1 ]; then
    echo "Erro ao compilar o programa!"
    exit 1
fi

THREADS=(1 2 4 8 16 32)
TRAPS=(128 256 512 1024)

echo "=== Iniciando Execuções ==="
echo ""

for t in "${THREADS[@]}"; do
    echo "### Threads: $t ###"
    for n in "${TRAPS[@]}"; do
        echo "--> Executando com $t threads e $n trapezios"

        printf "0 10 %d\n" "$n" | ./omp_trap_1 "$t"
        echo ""
    done
    echo ""
done

echo "=== Fim das Execuções ==="
