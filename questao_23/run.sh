#!/bin/bash
#SBATCH --nodes=1
#SBATCH --time=00:10:00
#SBATCH --cpus-per-task=32
#SBATCH -p sequana_cpu_dev
#SBATCH -J omp23_q4
#SBATCH --exclusive

echo "Nó alocado: $SLURM_JOB_NODELIST"

# Ir para o diretório do projeto
cd /scratch/pex1272-ufersa/iverton.emiquison/PCD/questao_23/

# Carregar módulos
module load gcc/14.2.0_sequana
module load openmpi/gnu/4.1.1_sequana

# Compilar via Makefile
make clean
make

if [ ! -f omp_23 ]; then
    echo "Erro ao compilar o programa!"
    exit 1
fi

# Combinações solicitadas pelo estilo da questão
THREADS=(1 2 4 8)
ITERACOES=(4 8 16 32)

echo ""
echo "=== Execuções para análise do escalonamento padrão do OpenMP ==="
echo ""

for t in "${THREADS[@]}"; do
    echo "==============================="
    echo "#### Número de threads: $t ####"
    echo "==============================="
    for n in "${ITERACOES[@]}"; do
        echo ""
        echo ">>> Executando com $t threads e $n iterações"
        printf "%d\n" "$n" | ./omp_23 "$t"
        echo ""
    done
done

echo "=== Fim das execuções ==="
