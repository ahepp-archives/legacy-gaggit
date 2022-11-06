add_executable(gaggit
        gaggit.c
        boiler.c
        task_boiler.c
        task_common.c
        task_log.c
        task_pid.c
        )
target_link_libraries(gaggit
        PRIVATE
        pidc
        pthread
        )
target_compile_options(gaggit
        PRIVATE
        -Wall
        )

install(TARGETS gaggit DESTINATION bin)
