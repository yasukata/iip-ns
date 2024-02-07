set(source_files
    helper/iip-helper.cc
)

set(header_files
    ${header_files}
    helper/iip-helper.h
)

build_lib(
  LIBNAME iip-ns
  SOURCE_FILES ${source_files}
  HEADER_FILES ${header_files}
  LIBRARIES_TO_LINK
  numa
)
