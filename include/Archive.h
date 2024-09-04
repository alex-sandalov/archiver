#pragma once

#include <fstream>
#include <vector>
#include <cstdint>
#include <iostream>
#include <string>
#include <algorithm>

static const size_t kBufferSize = 20;

struct HeaderFile {
	size_t size_file;
	size_t size_name_file;
	std::string name_file;

	HeaderFile();
};

class File {
public:
	File(const std::string& name_file);
	~File();
	inline bool SetReadFile();
	inline bool SetAppendFile();
	inline bool Read(char* buffer, size_t size_buffer);
	inline bool Write(const char* buffer, size_t size_buffer);
	inline bool Seekg(size_t index);
	size_t GetSize();

private:
	std::string name_file_;
	std::fstream file_;
};

class Archive {
public:
	Archive();
	Archive(const std::string& name);
	inline void List();
	bool Extract(const std::vector<std::string>& name_files);
	bool Append(const std::vector<std::string>& name_files);
	bool Delete(const std::vector<std::string>& name_files);

private:
	std::string name_;
	size_t count_files_;
	std::vector<std::string> name_files_;
};

bool ArchiveConcatenate(const std::string& archive_name_one, const std::string& archive_name_two, const std::string& archive_name_merge);