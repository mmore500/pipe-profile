#!/bin/bash

SOURCE_HASH=$( find -path ./third-party -prune -false -o -type f | xargs cat | sha1sum )

./ci/strip_trailing_whitespace.sh

if [ "${SOURCE_HASH}" == "$( find -path ./third-party -prune -false -o -type f | xargs cat | sha1sum )" ];
then
  exit 0 # success
else
  exit 1 # failure
fi
