add_library(Substrate STATIC
        Substrate/hde64.c
        Substrate/SubstrateDebug.cpp
        Substrate/SubstrateHook.cpp
        Substrate/SubstratePosixMemory.cpp
        Substrate/SymbolFinder.cpp
)
target_include_directories(Substrate PUBLIC Substrate)
target_compile_options(Substrate PRIVATE "-w")
