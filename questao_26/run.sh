#!/bin/bash
#SBATCH --nodes=1
#SBATCH --time=00:20:00
#SBATCH --cpus-per-task=32
#SBATCH -p sequana_cpu_dev
#SBATCH -J run_atomic_omp26
#SBATCH --exclusive
#SBATCH -o run_both_%j.out
#SBATCH -e run_both_%j.err

# -----------------------
# Script: run_both_sdumont.sh
# Objetivo: compilar e executar test_atomic (atomic test) e omp_26
#           em uma única execução no SDumont, com módulos carregados.
# Uso:
#   sbatch run_both_sdumont.sh            # usa THREADS=(1 2 4 8) e N=20000000
#   sbatch run_both_sdumont.sh 4 10000000 # usa THREADS=4 e N=10000000
# -----------------------

cd $SLURM_SUBMIT_DIR

echo "Nó alocado: $SLURM_JOB_NODELIST"
echo "Job id: $SLURM_JOB_ID"
echo "Iniciando em: $(date)"

# Diretório do projeto (mude se necessário)
WORKDIR="/scratch/pex1272-ufersa/iverton.emiquison/PCD/questao_26"
mkdir -p "$WORKDIR"
cd "$WORKDIR" || { echo "Erro: não foi possível entrar em $WORKDIR"; exit 1; }

# Carregar módulos do SDumont
module purge
module load gcc/14.2.0_sequana
module load openmpi/gnu/4.1.1_sequana

echo "Módulos carregados:"
module list

# Parâmetros (opcionais)
# 1º argumento: número de threads único (opcional)
# 2º argumento: valor de n (opcional)
USER_THREADS_ARG="$1"
USER_N_ARG="$2"

# Se o usuário passou um número de threads, usaremos apenas ele,
# caso contrário varremos a lista padrão.
if [ -n "$USER_THREADS_ARG" ]; then
    THREADS_LIST=("$USER_THREADS_ARG")
else
    THREADS_LIST=(1 2 4 8)
fi

# n (número de iterações) - padrão 20_000_000 (pode ajustar)
if [ -n "$USER_N_ARG" ]; then
    NVAL="$USER_N_ARG"
else
    NVAL=20000000
fi

echo "THREADS_LIST = ${THREADS_LIST[*]}"
echo "N (iterações) = $NVAL"

# -----------------------
# Compilar os programas
# -----------------------
echo ""
echo "==> Compilando test_atomic.c e omp_26.c ..."
# Compila test_atomic (programa que testa atomic)
gcc -g -Wall -fopenmp -o test_atomic test_atomic.c
RET=$?; if [ $RET -ne 0 ]; then echo "Erro ao compilar test_atomic.c"; exit 2; fi

# Compila omp_26
gcc -g -Wall -fopenmp -o omp_26 omp_26.c
RET=$?; if [ $RET -ne 0 ]; then echo "Erro ao compilar omp_26.c"; exit 3; fi

echo "Compilação OK."

# Diretório de logs
LOGDIR="${WORKDIR}/logs_run_${SLURM_JOB_ID}"
mkdir -p "$LOGDIR"

# -----------------------
# Executar testes
# -----------------------
for TH in "${THREADS_LIST[@]}"; do
    echo ""
    echo "=============================================="
    echo ">>>> Executando com $TH threads (n = $NVAL) <<<<"
    echo "=============================================="

    # 1) test_atomic: espera entrada 'n' via stdin
    LOG_TEST="${LOGDIR}/test_atomic_t${TH}_n${NVAL}.log"
    echo "--- test_atomic (threads=$TH) -> $LOG_TEST"
    {
        printf "%d\n" "$NVAL"
        echo "=== Rodando test_atomic com $TH threads (n=$NVAL) ==="
        /usr/bin/time -f "TEMPO_TOTAL: %E  (wall)  CPU:%P" ./test_atomic "$TH"
    } &> "$LOG_TEST"

    # 2) omp_26: espera entrada 'n' via stdin
    LOG_OMP26="${LOGDIR}/omp_26_t${TH}_n${NVAL}.log"
    echo "--- omp_26 (threads=$TH) -> $LOG_OMP26"
    {
        printf "%d\n" "$NVAL"
        echo "=== Rodando omp_26 com $TH threads (n=$NVAL) ==="
        /usr/bin/time -f "TEMPO_TOTAL: %E  (wall)  CPU:%P" ./omp_26 "$TH"
    } &> "$LOG_OMP26"

    echo "Resultados escritos em: $LOG_TEST e $LOG_OMP26"
done

echo ""
echo "Todas execuções concluídas em: $(date)"
echo "Logs em: $LOGDIR"
