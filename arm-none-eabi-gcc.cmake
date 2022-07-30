set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# Additional toolchain utilities
set(CMAKE_OBJCOPY "C:\\Users\\moskw\\AppData\\Roaming\\xPacks\\arm-none-eabi-gcc\\11.2.1-1.2\\bin\\arm-none-eabi-objcopy.exe" CACHE INTERNAL "")
set(CMAKE_SIZE "C:\\Users\\moskw\\AppData\\Roaming\\xPacks\\arm-none-eabi-gcc\\11.2.1-1.2\\bin\\arm-none-eabi-size.exe" CACHE INTERNAL "")

# Perform compiler test with static library
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

#---------------------------------------------------------------------------------------
# Set compiler/linker flags
#---------------------------------------------------------------------------------------

# Object build options
# -O0                   No optimizations, reduce compilation time and make debugging produce the expected results.
# -mthumb               Generat thumb instructions.
# -fno-builtin          Do not use built-in functions provided by GCC.
# -Wall                 Print only standard warnings, for all use Wextra
# -ffunction-sections   Place each function item into its own section in the output file.
# -fdata-sections       Place each data item into its own section in the output file.
# -fomit-frame-pointer  Omit the frame pointer in functions that don’t need one.
# -mabi=aapcs           Defines enums to be a variable sized type. See 3.19.5
set(OBJECT_GEN_FLAGS "\
    -Og \
    -ggdb \
    -O0 \
    -mthumb \
    -fsigned-char \
    -Wall \
    -Wno-comment \
    -Wno-unused-function \
    -Wno-unused-variable \
    -ffunction-sections \
    -fdata-sections \
    -fomit-frame-pointer \
    -mabi=aapcs \
    -fno-builtin \
") 

set(CMAKE_C_FLAGS   "${OBJECT_GEN_FLAGS} -std=gnu99 " CACHE INTERNAL "C Compiler options")
set(CMAKE_CXX_FLAGS "${OBJECT_GEN_FLAGS} -std=c++14 " CACHE INTERNAL "C++ Compiler options")
set(CMAKE_ASM_FLAGS "${OBJECT_GEN_FLAGS} -x assembler-with-cpp " CACHE INTERNAL "ASM Compiler options")


# -Wl,--gc-sections     Perform the dead code elimination.
# --specs=nano.specs    Link with newlib-nano.
# --specs=nosys.specs   No syscalls, provide empty implementations for the POSIX system calls.
set(CMAKE_EXE_LINKER_FLAGS "\
    -Wl,--gc-sections \
    -Wl,--print-memory-usage \
    -mthumb \
    -mabi=aapcs \
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map \
    --specs=nano.specs \
    --specs=nosys.specs \
" CACHE INTERNAL "Linker options") 

# Set executable suffix
set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")