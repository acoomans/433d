function(add_clang_static_analysis target)
    get_target_property(SRCs ${target} SOURCES)
    add_library(${target}_analyze OBJECT EXCLUDE_FROM_ALL ${SRCs})
    set_target_properties(${target}_analyze PROPERTIES
            COMPILE_OPTIONS "--analyze"
            EXCLUDE_FROM_DEFAULT_BUILD true)
endfunction()