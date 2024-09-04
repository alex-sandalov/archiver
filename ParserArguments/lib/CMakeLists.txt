add_library(
	ParserArgumentsLibray 
	ArgParser.cpp
	Argument.cpp
)

target_include_directories(ParserArgumentsLibray PUBLIC "${PROJECT_SOURCE_DIR}/include")