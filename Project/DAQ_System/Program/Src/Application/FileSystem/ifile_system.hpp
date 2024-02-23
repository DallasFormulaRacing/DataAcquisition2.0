/*
* File System
* Authors:        Cristian Cruz
*                 Ethan Fischer
*
* Email:          cris14.alex@gmail.com
*                 ejf180001@utdallas.edu
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef IFILE_SYSTEM_H
#define IFILE_SYSTEM_H

namespace application {

class IFileSystem {

public:
	virtual ~IFileSystem() = default;

	virtual bool Mount() = 0;
	virtual bool Unmount() = 0;
	virtual bool FileExists(char* path) = 0;
	virtual bool CreateFile(char* path) = 0;
    virtual bool OpenFile(char* path, const char* access_mode) = 0;
	virtual bool CloseFile() = 0;
	virtual bool WriteFile(char* data) = 0;
	virtual bool ReadFile(char* buffer) = 0;
	virtual bool CreateDirectory (char *name) = 0;

protected:
	// POSIX compliant mode flags
	static constexpr char* kReadOnlyMode               = (char*)"r";
	static constexpr char* kReadWriteMode              = (char*)"r+";
	static constexpr char* kEmtpyFileAndWriteMode      = (char*)"w";
	static constexpr char* kEmptyFileAndReadWriteMode  = (char*)"w+";
	static constexpr char* kAppendFileAndWriteMode     = (char*)"a";
	static constexpr char* kAppendFileAndReadWriteMode = (char*)"a+";
	static constexpr char* kNewFileAndWriteMode        = (char*)"wx";
	static constexpr char* kNewFileAndReadWriteMode    = (char*)"w+x";

	enum class ErrorFlags : uint16_t {
		Mounting		= 1 << 0,
		Unmounting		= 1 << 1,
		FileNotFound	= 1 << 2,
		CreateFile		= 1 << 3,
		OpenFile		= 1 << 4,
		ReadFile		= 1 << 5,
		WriteFile		= 1 << 6,
		CloseFile		= 1 << 7,
		CreateDirectory = 1 << 8
	};

	static constexpr uint8_t kNumErrorFlags = 9;
	unsigned int errors_ : kNumErrorFlags;

	void SetError(ErrorFlags flag) {
		errors_ |= static_cast<uint16_t>(flag);
	}

	unsigned int GetError(ErrorFlags flag) {
		return errors_ & static_cast<uint16_t>(flag);
	}
};

}

#endif // IFILE_SYSTEM_H
