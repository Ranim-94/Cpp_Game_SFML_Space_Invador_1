#!/usr/bin/env bash

set -euo pipefail

project_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
source_dir="$project_root/Saga"
build_dir="$source_dir/build"
executable="$build_dir/Saga_Game/Saga_Game"

cmake --fresh \
    -S "$source_dir" \
    -B "$build_dir" \
    -DSFML_DIR=/usr/local/lib/cmake/SFML

cmake --build "$build_dir"

"$executable"