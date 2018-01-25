Cross compilation instructions:

Environment variables:
	LEDE_ROOT - location of lede(openwrt) root directory e.g. /home/user/lede
	ZENO_ROOT - location of common_zenoutils project root directory e.g. /home/user/common_zenoutils
	PATH = $LEDE_ROOT/staging_dir/toolchain-arm_cortex-a9+neon_gcc-5.4.0_musl-1.1.16_eabi/bin:$PATH

Eclipse Settings:

	Create a configuration for cross compilation:
	Right click on project -> Build configuration -> manage -> new
	Enter name, description.
	Copy settings from -> default configuration: Build

	Right click on zenoutils project -> properties -> Autotools -> Configure Settings
	Platform specifiers - Host platform : arm-openwrt-linux
					Target platform : arm-openwrt-linux
	Environment variables - Add the following variables-
	CXXFLAGS="-I$LEDE_ROOT/staging_dir/toolchain-arm_cortex-a9+neon_gcc-5.4.0_musl-1.1.16_eabi/include -I$LEDE_ROOT/staging_dir/target-arm_cortex-a9+neon_musl-1.1.16_eabi/usr/include -std=gnu++11 -g" 
	LDFLAGS="-L$LEDE_ROOT/staging_dir/target-arm_cortex-a9+neon_musl-1.1.16_eabi/usr/lib" 
	STAGING_DIR="$LEDE_ROOT/staging_dir/toolchain-arm_cortex-a9+neon_gcc-5.4.0_musl-1.1.16_eabi" 
	PATH="$LEDE_ROOT/staging_dir/toolchain-arm_cortex-a9+neon_gcc-5.4.0_musl-1.1.16_eabi/bin:$PATH"

	Advanced -> Additional command-line options:
	--with-boost=$LEDE_ROOT/staging_dir/target-arm_cortex-a9+neon_musl-1.1.16_eabi/usr/include

Manual build:

cd $ZENO_ROOT
./autogen.sh
cd build-<build configuration name>
../configure --host=arm-openwrt-linux --target=arm-openwrt-linux CXXFLAGS="-I$LEDE_ROOT/staging_dir/toolchain-arm_cortex-a9+neon_gcc-5.4.0_musl-1.1.16_eabi/include -I$LEDE_ROOT/staging_dir/target-arm_cortex-a9+neon_musl-1.1.16_eabi/usr/include -std=gnu++11 -g" LDFLAGS="-L$LEDE_ROOT/staging_dir/target-arm_cortex-a9+neon_musl-1.1.16_eabi/usr/lib" STAGING_DIR="$LEDE_ROOT/staging_dir/toolchain-arm_cortex-a9+neon_gcc-5.4.0_musl-1.1.16_eabi" PATH="$LEDE_ROOT/staging_dir/toolchain-arm_cortex-a9+neon_gcc-5.4.0_musl-1.1.16_eabi/bin:$PATH" --with-boost=$LEDE_ROOT/staging_dir/target-arm_cortex-a9+neon_musl-1.1.16_eabi/usr/include
make
