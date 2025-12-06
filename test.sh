#!/bin/sh

set -e

./build.sh

# exec $(dirname "$0")/build/libcall-tests "$@"

exec $(dirname "$0")/build/libcall -j "./test_configs/base_type/literal_call.json"
