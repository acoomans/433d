function(add_external_boost_project Boost_VERSION Boost_MD5)
    set(Boost_COMPONENTS "${ARGN}")

    include(ExternalProject)

    string(REPLACE "." "_" Boost_VERSION_U ${Boost_VERSION})
    set(Boost_URL "https://downloads.sourceforge.net/project/boost/boost/${Boost_VERSION}/boost_${Boost_VERSION_U}.tar.bz2")
    set(Boost_INSTALL ${CMAKE_CURRENT_BINARY_DIR}/install/boost)
    set(Boost_INCLUDE_DIR ${Boost_INSTALL}/include)
    set(Boost_LIB_DIR ${Boost_INSTALL}/lib)

    foreach(COMPONENT ${Boost_COMPONENTS})
        set(Boost_CONFIGURE_COMMAND_WITH_LIB
                ${Boost_CONFIGURE_COMMAND_WITH_LIB}
                --with-libraries=${COMPONENT}
                )
        #            set(Boost_LIBRARIES  PARENT_SCOPE)
        list(INSERT Boost_LIBRARIES 0 "${Boost_LIB_DIR}/libboost_${COMPONENT}.a")
    endforeach()

    ExternalProject_Add(external_boost
            PREFIX Boost
            URL ${Boost_URL}
            URL_HASH MD5=${Boost_MD5}
            BUILD_IN_SOURCE 1
            CONFIGURE_COMMAND ./bootstrap.sh ${Boost_CONFIGURE_COMMAND_WITH_LIB} --prefix=<INSTALL_DIR>
            BUILD_COMMAND ./b2 install link=static variant=release threading=multi runtime-link=static
            INSTALL_COMMAND ""
            INSTALL_DIR ${Boost_INSTALL}
            )

    include_directories(${Boost_INCLUDE_DIR})

    set(Boost_LIBRARIES ${Boost_LIBRARIES} PARENT_SCOPE)

endfunction()


function(find_boost Boost_VERSION Boost_MD5)
    set(Boost_COMPONENTS "${ARGN}")

    find_package(Boost ${Boost_VERSION} QUIET)

    if (Boost_FOUND)
        find_package(Boost ${Boost_VERSION} REQUIRED COMPONENTS "${Boost_COMPONENTS}")
    else()

        message("** Boost not found, will download and use external Boost")
        add_external_boost_project(${Boost_VERSION} ${Boost_MD5} ${ARGN})

    endif()
endfunction()

function(target_link_boost TARGET)
    if (TARGET external_boost)
        add_dependencies(${TARGET} external_boost)
    endif()
    target_link_libraries(${TARGET} ${Boost_LIBRARIES})
endfunction()