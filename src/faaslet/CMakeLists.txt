include_directories(
        ${FAASM_INCLUDE_DIR}/faaslet
)

file(GLOB HEADERS "${FAASM_INCLUDE_DIR}/faaslet/*.h")

set(LIB_FILES
        Faaslet.cpp
        ${HEADERS}
        )

faasm_private_lib(faaslet_lib "${LIB_FILES}")

target_link_libraries(faaslet_lib faabric module_cache system)

# WAMR module depends on SGX support
if(FAASM_SGX_SUPPORT)
    target_link_libraries(faaslet_lib sgx_wamr)
else()
    target_link_libraries(faaslet_lib wamrmodule)
endif()
