#include "Archive.h"
#include "HammingCode.h"

HeaderFile::HeaderFile() {}

File::File(const std::string& name_file)
	: name_file_(name_file)
{}

inline bool File::SetReadFile() {
	file_.open(name_file_, std::ios::binary | std::ios::in);
	return file_.is_open();
}

inline bool File::SetAppendFile() {
	file_.open(name_file_, std::ios::binary | std::ios::app);
	return file_.is_open();
}

inline bool File::Seekg(size_t index) {
    file_.seekg(index, std::ios::cur);
    return !file_.fail();
}

size_t File::GetSize() {
	size_t file_size = -1;
	if (file_.is_open()) {
		std::streampos current_position = file_.tellg();

		file_.seekg(0, std::ios::end);
		file_size = file_.tellg();
		
		file_.seekg(current_position, std::ios::beg);
	}
	return file_size;
}

inline bool File::Read(char* buffer, size_t size_buffer) {
	return static_cast<bool>(file_.read(buffer, size_buffer));
}

inline bool File::Write(const char* buffer, size_t size_buffer) {
	return static_cast<bool>(file_.write(buffer, size_buffer));
}

File::~File() {
	file_.close();
}

Archive::Archive()
	: name_("unknow.haf")
	, count_files_(0)
{}

Archive::Archive(const std::string& name)
	: name_(name)
	, count_files_(0)
{}

inline void Archive::List() {
	for (const auto& name_file : name_files_) {
		std::cout << name_file << '\n';
	}
}

bool Archive::Extract(const std::vector<std::string>& name_files) {
    File archive(name_);
    if (!archive.SetReadFile()) {
        return false;
    }

    for (int i = 0; i < count_files_; ++i) {
        HeaderFile header_file;
        archive.Read(reinterpret_cast<char*>(&header_file.size_name_file), sizeof(header_file.size_name_file));
        header_file.name_file.resize(header_file.size_name_file);
        archive.Read(&header_file.name_file[0], header_file.size_name_file);
        archive.Read(reinterpret_cast<char*>(&header_file.size_file), sizeof(header_file.size_file));
		
		if (std::find(name_files_.begin(), name_files_.end(), header_file.name_file) == name_files_.end()) {
			archive.Seekg(header_file.size_file);;
			continue;
		}

		File extract_file("extract_file_" + header_file.name_file);
		if (!extract_file.SetAppendFile()) {
			return false;
		}
			
		std::vector<uint8_t> buffer(kBufferSize + 1);
		size_t size_file = header_file.size_file;
		while (size_file > 0) {
			size_t size_block = std::min(size_file, kBufferSize + 1);
			size_file -= size_block;

			archive.Read(reinterpret_cast<char*>(buffer.data()), size_block);

			std::vector<uint8_t> hamming_block = DecodeHamming(buffer, size_block * kSizeOneByte);
			extract_file.Write(reinterpret_cast<char*>(hamming_block.data()), hamming_block.size());
		}
    }

    return true;
}

bool Archive::Append(const std::vector<std::string>& name_files) {
	File archive(name_);
	if (!archive.SetAppendFile()) {
		return false;
	}

	for (const auto& name_file : name_files)  {
		File new_file(name_file);
		if (!new_file.SetReadFile()) {
			return false;
		}

		HeaderFile header_file;
		header_file.size_file = new_file.GetSize();
		size_t size_file = header_file.size_file;
		header_file.size_name_file = name_file.length();
		header_file.size_file += header_file.size_file / kBufferSize + 1;

		archive.Write(reinterpret_cast<const char*>(&header_file.size_name_file), sizeof(header_file.size_name_file));	
		archive.Write(name_file.c_str(), header_file.size_name_file);
		archive.Write(reinterpret_cast<const char*>(&header_file.size_file), sizeof(header_file.size_file));

		std::vector<uint8_t> buffer(kBufferSize);
		while (size_file > 0) {
			size_t size_block = std::min(size_file, static_cast<size_t>(kBufferSize));
			size_file -= size_block;

			new_file.Read(reinterpret_cast<char*>(buffer.data()), size_block);
			std::vector<uint8_t> hamming_block = EncodeHamming(buffer, size_block * kSizeOneByte);
			
			archive.Write(reinterpret_cast<const char*>(hamming_block.data()), hamming_block.size());
		}

		name_files_.push_back(name_file);
		++count_files_;
	}

	return true;
}

bool Archive::Delete(const std::vector<std::string>& name_files) {
	File temp_archive("temp_archive.haf");
	if (!temp_archive.SetAppendFile()) {
		return false;
	}

	File archive(name_);
  	if (!archive.SetReadFile()) {
		return false;
	}

	for (int i = 0; i < count_files_; ++i) {
        HeaderFile header_file;
        archive.Read(reinterpret_cast<char*>(&header_file.size_name_file), sizeof(header_file.size_name_file));
        header_file.name_file.resize(header_file.size_name_file);
        archive.Read(&header_file.name_file[0], header_file.size_name_file);
        archive.Read(reinterpret_cast<char*>(&header_file.size_file), sizeof(header_file.size_file));
		
		if (std::find(name_files_.begin(), name_files_.end(), header_file.name_file) == name_files_.end()) {
			temp_archive.Write(reinterpret_cast<const char*>(&header_file.size_name_file), sizeof(header_file.size_name_file));	
			temp_archive.Write(header_file.name_file.c_str(), header_file.size_name_file);
			temp_archive.Write(reinterpret_cast<const char*>(&header_file.size_file), sizeof(header_file.size_file));

			size_t size_file = header_file.size_file;
			std::vector<uint8_t> buffer(kBufferSize);
			while (size_file > 0) {
				size_t size_block = std::min(size_file, static_cast<size_t>(kBufferSize));
				size_file -= size_block;

				archive.Read(reinterpret_cast<char*>(buffer.data()), size_block);
				
				temp_archive.Write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
			}
		} else {
			archive.Seekg(header_file.size_file);
		}
    }
	
	std::string name = "temp_archive.haf";
	std::rename(name.c_str(), name_.c_str());
}

bool ArchiveConcatenate(const std::string& archive_name_one, const std::string& archive_name_two, const std::string& archive_name_merge) {
	File archive_one(archive_name_one);
	if (!archive_one.SetReadFile()) {
		return false;
	}

	File archive_two(archive_name_two);
	if (!archive_two.SetReadFile()) {
		return false;
	}

	File archive_merge(archive_name_merge);
	if (!archive_merge.SetAppendFile()) {
		return false;
	}

	std::vector<uint8_t> buffer(kBufferSize);
	while (archive_one.Read(reinterpret_cast<char*>(buffer.data()), kBufferSize)) {
		archive_merge.Write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
	}

	while (archive_two.Read(reinterpret_cast<char*>(buffer.data()), kBufferSize)) {
		archive_merge.Write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
	}

	return true;
}