#include <iostream>
#include "ParserArguments/include/ArgParser.h"
#include "Archive.h"

#include <map>

using namespace ArgumentParser;

int main(int argc, const char** argv) { 
	ArgParser parser("ArhiveParser");

	std::vector<std::string> files;

	parser.AddStringArgument('f', "file").StoreValues(files).MultiValue().Positional();
	parser.AddFlag('l', "list");
	parser.AddFlag('x', "extract");
	parser.AddFlag('a', "append");
	parser.AddFlag('d', "delete");
	parser.AddFlag('A', "concatenate");
	parser.AddFlag('c', "create");

	parser.Parse(argv, argc);

	std::map<std::string, Archive> archives;

	std::string name = files[0];
	files.erase(files.begin());

	if (parser.GetFlag("create")) {
		archives[name] = Archive(name);
		archives[name].Append(files);
	} else if (parser.GetFlag("append")) {
		archives[name].Append(files);
	} else if (parser.GetFlag("delete")) {
		archives[name].Delete(files);
	} else if (parser.GetFlag("concatenate")) {
		ArchiveConcatenate(name, files[0], files[1]);
	} 
	
}