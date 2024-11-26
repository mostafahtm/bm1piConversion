add_executable(main main.c conversion.c)

target_compile_options(main PRIVATE -msse4.1)
