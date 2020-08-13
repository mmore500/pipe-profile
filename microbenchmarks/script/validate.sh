#!/bin/bash

VERSION=$(git describe --dirty --always)
TIME=$(date +%s)
NNODE=$([ -z "$SLURM_JOB_NUM_NODES" ] && echo 1 || echo $SLURM_JOB_NUM_NODES)
NPROC=$(nproc)

OSF_PATH="microbenchmarks/context=${CONTEXT}/nnode=${NNODE}+nproc=${NPROC}/time=${TIME}+version=${VERSION}"

for f in $(find . -name "*.json"); do
  jsonlint-php $f
done