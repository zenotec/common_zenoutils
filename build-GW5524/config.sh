#!/bin/bash

PATH="$LEDE_ROOT/staging_dir/toolchain-arm_cortex-a9+neon_gcc-5.4.0_musl-1.1.16_eabi/bin:$PATH"; export PATH
STAGING_DIR="$LEDE_ROOT/staging_dir/toolchain-arm_cortex-a9+neon_gcc-5.4.0_musl-1.1.16_eabi"; export STAGING_DIR

../configure --host=arm-openwrt-linux --target=arm-openwrt-linux CXXFLAGS="-I$LEDE_ROOT/staging_dir/toolchain-arm_cortex-a9+neon_gcc-5.4.0_musl-1.1.16_eabi/include -I$LEDE_ROOT/staging_dir/target-arm_cortex-a9+neon_musl-1.1.16_eabi/usr/include -std=gnu++11 -g" LDFLAGS="-L$LEDE_ROOT/staging_dir/target-arm_cortex-a9+neon_musl-1.1.16_eabi/usr/lib" STAGING_DIR="$LEDE_ROOT/staging_dir/toolchain-arm_cortex-a9+neon_gcc-5.4.0_musl-1.1.16_eabi" PATH="$LEDE_ROOT/staging_dir/toolchain-arm_cortex-a9+neon_gcc-5.4.0_musl-1.1.16_eabi/bin:$PATH" --with-boost=$LEDE_ROOT/staging_dir/target-arm_cortex-a9+neon_musl-1.1.16_eabi/usr/include
