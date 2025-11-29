#!/bin/sh

set -e

./build.sh

exec $(dirname "$0")/build/libcall "$@"
