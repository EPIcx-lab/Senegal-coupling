#!/bin/bash

##### begin of user info

COMMAND="./cluster_giulia/script/site"
NRUN=100
NNODES=4
ID_START=0
INPUT_FILE="./cluster_giulia/script/parameters/aggregation.txt"
OUTPUT_DIR="/data/giulia/err_out"

##### end of user info


NUM_CORES_PER_NODE=12
#NUM_LINES="$(wc -l < $INPUT_FILE)"

NUM_RUNS_IN_A_PROCESS=$(( 1+ ($NRUN / ($NNODES * $NUM_CORES_PER_NODE)) )) #$(($NRUN / $NCPU))
if [ $(( ($NRUN % ($NNODES * $NUM_CORES_PER_NODE)) )) = 0 ]; 
then
   NUM_RUNS_IN_A_PROCESS=$(( $NUM_RUNS_IN_A_PROCESS - 1 ))
fi

NUM_NODES_NEEDED=$(($NNODES - 1)) #$(($NCPU / $NUM_CORES_PER_NODE))
TEMP_JOB_FILE="assess_HET.job"

line=0
while read parameters;
do
    for node in $(seq 0 $NUM_NODES_NEEDED);
    do
	OFFSET=$(($ID_START + ($NUM_RUNS_IN_A_PROCESS*($NUM_CORES_PER_NODE * $node))))
	NAME="giulia-$node-$line"
	#create the SLURM file
	echo "#!/bin/bash -l" > $TEMP_JOB_FILE
	echo "#SBATCH -N 1" >> $TEMP_JOB_FILE
	echo "#SBATCH --ntasks-per-node=$NUM_CORES_PER_NODE" >> $TEMP_JOB_FILE
	echo "#SBATCH -t 15-20:00:00    # time limit to batch job" >> $TEMP_JOB_FILE
	echo "#SBATCH --job-name=$NAME" #>> $TEMP_JOB_FILE
	echo "#SBATCH -o $OUTPUT_DIR/$NAME.out    # output file" >> $TEMP_JOB_FILE
	echo "#SBATCH -e $OUTPUT_DIR/$NAME.err    # error file" >> $TEMP_JOB_FILE
	# run the process
	echo "mpirun ./cluster $OFFSET $NUM_RUNS_IN_A_PROCESS $COMMAND $parameters" >> $TEMP_JOB_FILE
	# launch the job on the cluster
	sbatch $TEMP_JOB_FILE
    done
    line=$(($line + 1))
done < $INPUT_FILE
