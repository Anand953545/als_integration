@echo off
set AWARE_DAM_RO_BASE=0x43000000
set FOTA_DAM_RO_BASE=0x41000000

set LLVM_ROOT_PATH=C:\Users\2166772\Downloads\snapdragon_sd_llvm_arm.win.4.0_installer_41720.103-16-22_11_46_50\Snapdragon_SD_LLVM_ARM_WIN_4_0_Installer_41720_1\Snapdragon-llvm-4.0.17-windows64
set TOOLCHAIN_PATH=%LLVM_ROOT_PATH%\bin
set TOOLCHAIN_PATH_STANDARDS=%LLVM_ROOT_PATH%\armv7m-none-eabi\libc\include
set LLVMLIB=%LLVM_ROOT_PATH%\lib\clang\4.0.17\lib
set LLVMLINK_PATH=%LLVM_ROOT_PATH%\tools\bin
set DAM_ELF_OUTPUT_PATH=bin
set DAM_INC_BASE=common\include
set DAM_LIB_PATH=common\libs
set DAM_SRC_PATH=src

set AWARE_DAM_INC_PATH=src\aware_client\core\inc
set AWARE_DAM_SRC_PATH=src\aware_client\core\src

set AWARE_DAM_DATA_MGR_INC_PATH=src\aware_client\data_mgr\inc
set AWARE_DAM_DATA_MGR_SRC_PATH=src\aware_client\data_mgr\src

set AWARE_DAM_CFG_MGR_INC_PATH=src\aware_client\cfg_mgr\inc
set AWARE_DAM_CFG_MGR_SRC_PATH=src\aware_client\cfg_mgr\src

set AWARE_DAM_NANO_PB_INC_PATH=src\aware_client\nanopb\inc
set AWARE_DAM_NANO_PB_SRC_PATH=src\aware_client\nanopb\src

set AWARE_DAM_FOTA_INC_PATH=src\aware_client\fota\inc
set AWARE_DAM_FOTA_SRC_PATH=src\aware_client\fota\src

set AWARE_DAM_SENSOR_MGR_INC_PATH=src\aware_client\sensor_mgr\inc
set AWARE_DAM_SENSOR_MGR_SRC_PATH=src\aware_client\sensor_mgr\src

set FOTA_DAM_INC_PATH=src\fota_client\inc
set FOTA_DAM_SRC_PATH=src\fota_client\src

set AWARE_DAM_APP_UT_PATH=src\aware_client\tests

set PYTHON_PATH=C:\Python27\python.exe

set AWARE_DAM_APP_OUTPUT_PATH=src\build\aware_client
set AWARE_DAM_APP_LD_PATH=src\aware_client\aware_client.ld

set FOTA_DAM_APP_OUTPUT_PATH=src\build\fota_client
set FOTA_DAM_APP_LD_PATH=src\fota_client\fota_client.ld

set DAM_LIBNAME=txm_lib.lib
set TIMER_LIBNAME=timer_dam_lib.lib
set DIAG_LIB_NAME=diag_dam_lib.lib
set QMI_LIB_NAME=qcci_dam_lib.lib
set QMI_QCCLI_LIB_NAME=IDL_DAM_LIB.lib

set AWARE_DAM_ELF_NAME=aware_client_v1.elf
set AWARE_DAM_TARGET_BIN=aware_client_v1.bin
set AWARE_DAM_MAP_NAME=aware_client_v1.map

set FOTA_DAM_ELF_NAME=fota_client_v1.elf
set FOTA_DAM_TARGET_BIN=fota_client_v1.bin
set FOTA_DAM_MAP_NAME=fota_client_v1.map

echo y | del %AWARE_DAM_APP_OUTPUT_PATH%
if not exist %DAM_ELF_OUTPUT_PATH% (
  mkdir %DAM_ELF_OUTPUT_PATH%
)

if not exist %AWARE_DAM_APP_OUTPUT_PATH% (
  mkdir %AWARE_DAM_APP_OUTPUT_PATH%
)

if not exist %FOTA_DAM_APP_OUTPUT_PATH% (
  mkdir %FOTA_DAM_APP_OUTPUT_PATH%
)

echo "Aware DAM RO base selected = %AWARE_DAM_RO_BASE%"

set DAM_CPPFLAGS=-DENABLE_ALL_LOGS -DTRACKER_REF_APP -DQAPI_TXM_MODULE -DTXM_MODULE -DTX_DAM_QC_CUSTOMIZATIONS -DTX_ENABLE_PROFILING -DTX_ENABLE_EVENT_TRACE -DTX_DISABLE_NOTIFY_CALLBACKS  -DFX_FILEX_PRESENT -DTX_ENABLE_IRQ_NESTING  -DTX3_CHANGES

set DAM_CFLAGS= -marm -target armv7m-none-musleabi -mfloat-abi=softfp -mfpu=none -mcpu=cortex-a7 -mno-unaligned-access  -fms-extensions -Osize -fshort-enums -Wbuiltin-macro-redefined

set DAM_INC_PATHS=-I %DAM_INC_BASE% -I %DAM_INC_BASE%\threadx_api  -I %DAM_INC_BASE%\qapi -I %TOOLCHAIN_PATH_STANDARDS% -I %LLVMLIB%

set AWARE_DAM_APP_INC_PATHS=-I %AWARE_DAM_INC_PATH% -I %AWARE_DAM_SENSOR_MGR_INC_PATH% -I %AWARE_DAM_FOTA_INC_PATH% -I %AWARE_DAM_NANO_PB_INC_PATH% -I %AWARE_DAM_DATA_MGR_INC_PATH% -I %AWARE_DAM_CFG_MGR_INC_PATH% -I %AWARE_DAM_APP_UT_PATH% %DAM_INC_PATHS%

set FOTA_DAM_APP_INC_PATHS=-I %FOTA_DAM_INC_PATH% %DAM_INC_PATHS%

if "%~1" == "-u" (
  set DAM_CPPFLAGS=%DAM_CPPFLAGS% -DRUN_UNIT_TESTS
  %TOOLCHAIN_PATH%\clang.exe -c %DAM_CPPFLAGS% %DAM_CFLAGS% %AWARE_DAM_APP_INC_PATHS% %AWARE_DAM_APP_UT_PATH%\*.c
)

echo "Compiling Aware DAM application"

@echo on

%TOOLCHAIN_PATH%\clang.exe -E  %DAM_CPPFLAGS% %DAM_CFLAGS% %DAM_SRC_PATH%\txm_module_preamble_llvm.S > txm_module_preamble_llvm_pp.S
%TOOLCHAIN_PATH%\clang.exe  -c %DAM_CPPFLAGS% %DAM_CFLAGS% txm_module_preamble_llvm_pp.S -o %AWARE_DAM_APP_OUTPUT_PATH%\txm_module_preamble_llvm.o
%TOOLCHAIN_PATH%\clang.exe  -c %DAM_CPPFLAGS% %DAM_CFLAGS% txm_module_preamble_llvm_pp.S -o %FOTA_DAM_APP_OUTPUT_PATH%\txm_module_preamble_llvm.o

if %ERRORLEVEL%==0 goto proceed
if %ERRORLEVEL%==1 goto exit

:proceed
del txm_module_preamble_llvm_pp.S

%TOOLCHAIN_PATH%\clang.exe -c %DAM_CPPFLAGS% %DAM_CFLAGS% %AWARE_DAM_APP_INC_PATHS% %AWARE_DAM_SRC_PATH%\*.c
%TOOLCHAIN_PATH%\clang.exe -c %DAM_CPPFLAGS% %DAM_CFLAGS% %AWARE_DAM_APP_INC_PATHS% %AWARE_DAM_DATA_MGR_SRC_PATH%\*.c
%TOOLCHAIN_PATH%\clang.exe -c %DAM_CPPFLAGS% %DAM_CFLAGS% %AWARE_DAM_APP_INC_PATHS% %AWARE_DAM_CFG_MGR_SRC_PATH%\*.c
%TOOLCHAIN_PATH%\clang.exe -c %DAM_CPPFLAGS% %DAM_CFLAGS% %AWARE_DAM_APP_INC_PATHS% %AWARE_DAM_NANO_PB_SRC_PATH%\*.c
%TOOLCHAIN_PATH%\clang.exe -c %DAM_CPPFLAGS% %DAM_CFLAGS% %AWARE_DAM_APP_INC_PATHS% %AWARE_DAM_FOTA_SRC_PATH%\*.c
%TOOLCHAIN_PATH%\clang.exe -c %DAM_CPPFLAGS% %DAM_CFLAGS% %AWARE_DAM_APP_INC_PATHS% %AWARE_DAM_SENSOR_MGR_SRC_PATH%\*.c

if %ERRORLEVEL%==0 (
echo "Compilation succeeded"
move *.o %AWARE_DAM_APP_OUTPUT_PATH%
echo "Linking Aware Client DAM"

%TOOLCHAIN_PATH%\clang++.exe -d -o %DAM_ELF_OUTPUT_PATH%\%AWARE_DAM_ELF_NAME% -target armv7m-none-musleabi -fuse-ld=qcld -lc++ -Wl,-mno-unaligned-access -fuse-baremetal-sysroot -fno-use-baremetal-crt -Wl,-entry=%AWARE_DAM_RO_BASE% %AWARE_DAM_APP_OUTPUT_PATH%\txm_module_preamble_llvm.o -Wl,-T%AWARE_DAM_APP_LD_PATH% -Wl,-Map=%DAM_ELF_OUTPUT_PATH%\%AWARE_DAM_MAP_NAME%,-gc-sections -Wl,-gc-sections %AWARE_DAM_APP_OUTPUT_PATH%\*.o %DAM_LIB_PATH%\*.lib
%PYTHON_PATH% %LLVMLINK_PATH%\llvm-elf-to-hex.py --bin %DAM_ELF_OUTPUT_PATH%\%AWARE_DAM_ELF_NAME% --output %DAM_ELF_OUTPUT_PATH%\%AWARE_DAM_TARGET_BIN%

echo "Aware Client DAM is built at" %DAM_ELF_OUTPUT_PATH%\%AWARE_DAM_TARGET_BIN%
del %AWARE_DAM_APP_OUTPUT_PATH%\*.o
set /p =aware_client_v1.bin;fota_client_v1.bin<nul > .\bin\oem_app_path.ini

REM if %ERRORLEVEL%==0 (
REM   %TOOLCHAIN_PATH%\clang.exe -c %DAM_CPPFLAGS% %DAM_CFLAGS% %FOTA_DAM_APP_INC_PATHS% %FOTA_DAM_SRC_PATH%\*.c
REM move *.o %FOTA_DAM_APP_OUTPUT_PATH%

REM echo "Linking FOTA Client DAM"
REM %TOOLCHAIN_PATH%\clang++.exe -d -o %DAM_ELF_OUTPUT_PATH%\%FOTA_DAM_ELF_NAME% -target armv7m-none-musleabi -fuse-ld=qcld -lc++ -Wl,-mno-unaligned-access -fuse-baremetal-sysroot -fno-use-baremetal-crt -Wl,-entry=%FOTA_DAM_RO_BASE% %FOTA_DAM_APP_OUTPUT_PATH%\txm_module_preamble_llvm.o -Wl,-T%FOTA_DAM_APP_LD_PATH% -Wl,-Map=%DAM_ELF_OUTPUT_PATH%\%FOTA_DAM_MAP_NAME%,-gc-sections -Wl,-gc-sections %FOTA_DAM_APP_OUTPUT_PATH%\*.o %DAM_LIB_PATH%\*.lib
REM %PYTHON_PATH% %LLVMLINK_PATH%\llvm-elf-to-hex.py --bin %DAM_ELF_OUTPUT_PATH%\%FOTA_DAM_ELF_NAME% --output %DAM_ELF_OUTPUT_PATH%\%FOTA_DAM_TARGET_BIN%

REM echo "FOTA Client DAM is built at" %DAM_ELF_OUTPUT_PATH%\%FOTA_DAM_TARGET_BIN%
REM del %FOTA_DAM_APP_OUTPUT_PATH%\*.o
REM )

) else (
del *.o
echo "Fail to compile. Exiting...."
echo "compilation failed with errors"
EXIT /B %ERRORLEVEL% 
:exit
EXIT /B %ERRORLEVEL% 
)
