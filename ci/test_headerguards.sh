#!/bin/bash

SOURCE_HASH=$( find include -type f | xargs cat | sha1sum )

./ci/generate_headerguards.sh

if [ "${SOURCE_HASH}" == "$( find include -type f | xargs cat | sha1sum )" ];
then
  exit 0 # success
else
  echo 1 # failure
fi
