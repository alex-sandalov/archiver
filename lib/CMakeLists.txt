add_library(
	Archiver
	Archive.cpp
	HammingCode.cpp
)

target_include_directories(Archiver PUBLIC "${PROJECT_SOURCE_DIR}/include")