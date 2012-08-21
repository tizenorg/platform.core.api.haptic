CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(fw_name "capi-system-haptic")
SET(fw_test "capi-system-haptic-test")

INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED glib-2.0 ecore-x elementary ecore evas)

FOREACH(flag ${pkgs_CFLAGS})
    SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -Wall -g")
INCLUDE_DIRECTORIES(../include)

#ADD_EXECUTABLE("system-sensor" system-sensor.c)
#TARGET_LINK_LIBRARIES("system-sensor" ${fw_name} ${${fw_test}_LDFLAGS})

aux_source_directory(. sources)
FOREACH(src ${sources})
    GET_FILENAME_COMPONENT(src_name ${src} NAME_WE)
    MESSAGE("${src_name}")
    ADD_EXECUTABLE(${src_name} ${src})
    IF(EXISTS "${src_name}.edc")
        ADD_CUSTOM_TARGET(
                "${src_name}.edj"
                COMMAND edje_cc 
                ${CMAKE_CURRENT_SOURCE_DIR}/${src_name}.edc 
                ${CMAKE_CURRENT_SOURCE_DIR}/${src_name}.edj
        )
        ADD_DEPENDENCIES(${src_name} ${src_name}.edj)
    ENDIF(EXISTS "${src_name}.edc")
    TARGET_LINK_LIBRARIES(${src_name} ${fw_name} ${pkgs_LDFLAGS} ${fw_name})
ENDFOREACH()

IF(UNIX)

ADD_CUSTOM_TARGET (distclean @echo cleaning for source distribution)
ADD_CUSTOM_COMMAND(
        DEPENDS clean 
        COMMENT "distribution clean"
        COMMAND find
        ARGS    . 
        -not -name config.cmake -and \(
        -name tester.c -or
        -name Testing -or
        -name CMakeFiles -or
        -name cmake.depends -or
        -name cmake.check_depends -or
        -name CMakeCache.txt -or
        -name cmake.check_cache -or
        -name *.cmake -or
        -name Makefile -or
        -name core -or
        -name core.* -or
        -name gmon.out -or
        -name install_manifest.txt -or
        -name *.pc -or
        -name *.edj -or
        -name *~ \)
        | grep -v TC | xargs rm -rf
        TARGET  distclean
        VERBATIM
)

ENDIF(UNIX)
