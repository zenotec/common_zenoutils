#!/bin/bash

PATH="$LEDE_ROOT/staging_dir/toolchain-arm_cortex-a9+neon_gcc-5.4.0_musl-1.1.16_eabi/bin:$PATH"; export PATH
STAGING_DIR="$LEDE_ROOT/staging_dir/toolchain-arm_cortex-a9+neon_gcc-5.4.0_musl-1.1.16_eabi"; export STAGING_DIR

echo $PATH
echo $STAGING_DIR

make ${@}

