#!/bin/sh



DAM_RO_BASE=0x43000000

export LLVM_ROOT_PATH=/home/anand/Desktop/llvm/llvm
export TOOLCHAIN_PATH=$LLVM_ROOT_PATH/bin
export TOOLCHAIN_PATH_STANDARDS=$LLVM_ROOT_PATH/armv7m-none-eabi/libc/include
export LLVMLIB=$LLVM_ROOT_PATH/clang/4.0.17/lib
export LLVMLINK_PATH=$LLVM_ROOT_PATH/tools/bin

export DAM_ELF_OUTPUT_PATH="./bin"
export DAM_INC_BASE="./common/include"
export DAM_LIB_PATH="./common/libs"
export DAM_SRC_PATH="./src"

export AWARE_DAM_INC_PATH="./src/aware_client/core/inc"
export AWARE_DAM_SRC_PATH="./src/aware_client/core/src"

export AWARE_DAM_DATA_MGR_INC_PATH="./src/aware_client/data_mgr/inc"
export AWARE_DAM_DATA_MGR_SRC_PATH="./src/aware_client/data_mgr/src"

export AWARE_DAM_CFG_MGR_INC_PATH="./src/aware_client/cfg_mgr/inc"
export AWARE_DAM_CFG_MGR_SRC_PATH="./src/aware_client/cfg_mgr/src"

export AWARE_DAM_NANO_PB_INC_PATH="./src/aware_client/nanopb/inc"
export AWARE_DAM_NANO_PB_SRC_PATH="./src/aware_client/nanopb/src"

export AWARE_DAM_FOTA_INC_PATH="./src/aware_client/fota/inc"
export AWARE_DAM_FOTA_SRC_PATH="./src/aware_client/fota/src"

export FOTA_DAM_INC_PATH="./src/fota_client/inc"
export FOTA_DAM_SRC_PATH="./src/fota_client/src"

export AWARE_DAM_APP_UT_PATH="./src/aware_client/tests"

export AWARE_DAM_APP_OUTPUT_PATH="./src/build/aware_client"
export AWARE_DAM_APP_LD_PATH="./src/aware_client/aware_client.ld"

export FOTA_DAM_APP_OUTPUT_PATH="./src/build/fota_client"
export FOTA_DAM_APP_LD_PATH="./src/fota_client/fota_client.ld"

export DAM_LIBNAME="txm_lib.lib"
export TIMER_LIBNAME="timer_dam_lib.lib"
export DIAG_LIB_NAME="diag_dam_lib.lib"
export QMI_LIB_NAME="qcci_dam_lib.lib"
export QMI_QCCLI_LIB_NAME="IDL_DAM_LIB.lib"

export AWARE_DAM_ELF_NAME="aware_client_v1.elf"
export AWARE_DAM_TARGET_BIN="aware_client_v1.bin"
export AWARE_DAM_MAP_NAME="aware_client_v1.map"

export FOTA_DAM_ELF_NAME="fota_client_v1.elf"
export FOTA_DAM_TARGET_BIN="fota_client_v1.bin"
export FOTA_DAM_MAP_NAME="fota_client_v1.map"

rm -rf  "$AWARE_DAM_APP_OUTPUT_PATH"

if [ ! -d "$DAM_ELF_OUTPUT_PATH" ]
then
  mkdir -p "$DAM_ELF_OUTPUT_PATH"
fi

if [ ! -d "$AWARE_DAM_APP_OUTPUT_PATH" ]
then
  mkdir -p "$AWARE_DAM_APP_OUTPUT_PATH"
fi

if [ ! -d "$FOTA_DAM_APP_OUTPUT_PATH" ]
then
  mkdir -p "$FOTA_DAM_APP_OUTPUT_PATH"
fi


echo "Application RO base selected = $DAM_RO_BASE"


export DAM_CPPFLAGS="-DENABLE_UART_DEBUG -DENABLE_ALL_LOGS -DQAPI_TXM_MODULE -DTXM_MODULE -DTX_DAM_QC_CUSTOMIZATIONS -DTX_ENABLE_PROFILING -DTX_ENABLE_EVENT_TRACE -DTX_DISABLE_NOTIFY_CALLBACKS  -DFX_FILEX_PRESENT -DTX_ENABLE_IRQ_NESTING  -DTX3_CHANGES"

export DAM_CFLAGS="-marm -target armv7m-none-musleabi -mfloat-abi=softfp -mfpu=none -mcpu=cortex-a7 -mno-unaligned-access -fms-extensions -Osize -fshort-enums -Wbuiltin-macro-redefined"

export DAM_INC_PATHS="-I $DAM_INC_BASE -I $DAM_INC_BASE/threadx_api  -I $DAM_INC_BASE/qapi -I $TOOLCHAIN_PATH_STANDARDS -I $LLVMLIB"

export AWARE_DAM_APP_INC_PATHS="-I $AWARE_DAM_INC_PATH -I $AWARE_DAM_FOTA_INC_PATH -I $AWARE_DAM_NANO_PB_INC_PATH -I $AWARE_DAM_DATA_MGR_INC_PATH -I $AWARE_DAM_CFG_MGR_INC_PATH -I $AWARE_DAM_APP_UT_PATH $DAM_INC_PATHS"

export FOTA_DAM_APP_INC_PATHS="-I $FOTA_DAM_INC_PATH $DAM_INC_PATHS"

if [$1 -eq "-u" ]
then
	export DAM_CPPFLAGS="$DAM_CPPFLAGS -DRUN_UNIT_TESTS"
	$TOOLCHAIN_PATH/clang -c $DAM_CPPFLAGS $DAM_CFLAGS $AWARE_DAM_APP_INC_PATHS $AWARE_DAM_APP_UT_PATH/*.c
fi


#Turn on verbose mode by default
set -x;

echo "Compiling Aware DAM application"

$TOOLCHAIN_PATH/clang -E  $DAM_CPPFLAGS $DAM_CFLAGS $DAM_SRC_PATH/txm_module_preamble_llvm.S > txm_module_preamble_llvm_pp.S
$TOOLCHAIN_PATH/clang -c $DAM_CPPFLAGS $DAM_CFLAGS txm_module_preamble_llvm_pp.S -o $AWARE_DAM_APP_OUTPUT_PATH/txm_module_preamble_llvm.o
$TOOLCHAIN_PATH/clang -c $DAM_CPPFLAGS $DAM_CFLAGS txm_module_preamble_llvm_pp.S -o $FOTA_DAM_APP_OUTPUT_PATH/txm_module_preamble_llvm.o
if [ $? -eq 0 ]; 
then
	echo "txm_module_preamble_llvm compiled sucessfully"
	rm txm_module_preamble_llvm_pp.S

	$TOOLCHAIN_PATH/clang -c $DAM_CPPFLAGS $DAM_CFLAGS $AWARE_DAM_APP_INC_PATHS $AWARE_DAM_SRC_PATH/*.c
	if [ $? -eq 0 ]; 
	then
		echo "AWARE DAM APP src compilation succeed"

		$TOOLCHAIN_PATH/clang -c $DAM_CPPFLAGS $DAM_CFLAGS $AWARE_DAM_APP_INC_PATHS $AWARE_DAM_DATA_MGR_SRC_PATH/*.c
		if [ $? -eq 0 ]; 
		then
			echo "AWARE DAM DATA MGR src compilation succeed"

			$TOOLCHAIN_PATH/clang -c $DAM_CPPFLAGS $DAM_CFLAGS $AWARE_DAM_APP_INC_PATHS $AWARE_DAM_CFG_MGR_SRC_PATH/*.c
			if [ $? -eq 0 ]; 
			then
				echo "AWARE DAM CFG MGR src compilation succeed"


				$TOOLCHAIN_PATH/clang -c $DAM_CPPFLAGS $DAM_CFLAGS $AWARE_DAM_APP_INC_PATHS $AWARE_DAM_NANO_PB_SRC_PATH/*.c
				if [ $? -eq 0 ]; 
				then
					echo "AWARE DAM NANO PB src compilation succeed"

					$TOOLCHAIN_PATH/clang -c $DAM_CPPFLAGS $DAM_CFLAGS $AWARE_DAM_APP_INC_PATHS $AWARE_DAM_FOTA_SRC_PATH/*.c
					if [ $? -eq 0 ]; 
					then
						echo "AWARE DAM FOTA src compilation succeed"


						mv *.o $AWARE_DAM_APP_OUTPUT_PATH
						echo "Linking Aware DAM application"
						$TOOLCHAIN_PATH/clang++ -d -o $DAM_ELF_OUTPUT_PATH/$AWARE_DAM_ELF_NAME -target armv7m-none-musleabi -fuse-ld=qcld -lc++ -Wl,-mno-unaligned-access -fuse-baremetal-sysroot -fno-use-baremetal-crt -Wl,-entry=$DAM_RO_BASE $AWARE_DAM_APP_OUTPUT_PATH/txm_module_preamble_llvm.o -Wl,-T$AWARE_DAM_APP_LD_PATH -Wl,-Map=$DAM_ELF_OUTPUT_PATH/$AWARE_DAM_MAP_NAME,-gc-sections -Wl,-gc-sections $AWARE_DAM_APP_OUTPUT_PATH/*.o $DAM_LIB_PATH/*.lib
						$LLVMLINK_PATH/llvm-elf-to-hex.py --bin $DAM_ELF_OUTPUT_PATH/$AWARE_DAM_ELF_NAME --output $DAM_ELF_OUTPUT_PATH/$AWARE_DAM_TARGET_BIN

						echo "Demo application is built at" $DAM_ELF_OUTPUT_PATH/$AWARE_DAM_TARGET_BIN
						rm $AWARE_DAM_APP_OUTPUT_PATH/*.o
						
						echo -n "aware_client_v1.bin";"fota_client_v1.bin" > ./bin/oem_app_path.ini


						$TOOLCHAIN_PATH/clang -c $DAM_CPPFLAGS $DAM_CFLAGS $FOTA_DAM_APP_INC_PATHS $FOTA_DAM_SRC_PATH/*.c

						mv *.o $FOTA_DAM_APP_OUTPUT_PATH
						echo "Linking FOTA DAM application"
						$TOOLCHAIN_PATH/clang++ -d -o $DAM_ELF_OUTPUT_PATH/$FOTA_DAM_ELF_NAME -target armv7m-none-musleabi -fuse-ld=qcld -lc++ -Wl,-mno-unaligned-access -fuse-baremetal-sysroot -fno-use-baremetal-crt -Wl,-entry=$DAM_RO_BASE $FOTA_DAM_APP_OUTPUT_PATH/txm_module_preamble_llvm.o -Wl,-T$FOTA_DAM_APP_LD_PATH -Wl,-Map=$DAM_ELF_OUTPUT_PATH/$FOTA_DAM_MAP_NAME,-gc-sections -Wl,-gc-sections $FOTA_DAM_APP_OUTPUT_PATH/*.o $DAM_LIB_PATH/*.lib
						$LLVMLINK_PATH/llvm-elf-to-hex.py --bin $DAM_ELF_OUTPUT_PATH/$FOTA_DAM_ELF_NAME --output $DAM_ELF_OUTPUT_PATH/$FOTA_DAM_TARGET_BIN

						echo "FOTA DAM application is built at" $DAM_ELF_OUTPUT_PATH/$FOTA_DAM_TARGET_BIN
						rm $FOTA_DAM_APP_OUTPUT_PATH/*.o

					else
					echo "Compilation failed with errors"
					fi
				fi
			fi
		fi
	fi
fi