# References: https://github.com/cpp-for-yourself/lectures-and-homeworks/blob/main/lectures/googletest.md
# https://www.youtube.com/watch?v=pxJoVRfpRPE

# Adapted from https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html
find_package(Git QUIET)
if(GIT_FOUND)
    # Check if the submodule folder exists
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/CMakeLists.txt")
        message("Submodule googletest not found. Adding Git submodule...")

        # Run the git submodule add command
        execute_process(
            COMMAND git submodule add https://github.com/google/googletest.git external/googletest
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMODULE_RESULT
        )

        # Check if the submodule was added successfully
        if(NOT ${GIT_SUBMODULE_RESULT} EQUAL 0)
            message("Failed to add Git googletest submodule. 
                ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest/CMakeLists.txt already exists.")
        endif()
    endif()

    option(UPDATE_SUBMODULES "Check submodules during build" ON)
    if(NOT UPDATE_SUBMODULES)
        return()
    endif()
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    OUTPUT_VARIABLE EXISTING_SUBMODULES
                    RESULT_VARIABLE RETURN_CODE
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Updating git submodules:\n${EXISTING_SUBMODULES}")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE RETURN_CODE)
    if(NOT RETURN_CODE EQUAL "0")
        message(WARNING "Cannot update submodules. Git command failed with ${RETURN_CODE}")
        return()
    endif()
    message(STATUS "Git submodules updated successfully")
endif()