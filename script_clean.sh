#!/usr/bin/env bash

set -euo pipefail

project_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
build_dir="$project_root/Saga/build"

if [[ -d "$build_dir" ]]; then
    rm -rf -- "$build_dir"
    echo "Removed: $build_dir"
else
    echo "Nothing to clean: $build_dir does not exist"
fi
