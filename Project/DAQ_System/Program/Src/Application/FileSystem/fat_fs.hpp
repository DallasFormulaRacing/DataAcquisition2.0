/*
* Fat FS Library Wrapper
* Author:   Cristian Cruz
*
* Email:    cris14.alex@gmail.com
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef FAT_FS_H
#define FAT_FS_H

// Standard Libraries
#include "string.h"
#include "stdio.h"

// File System Framework
#include "fatfs.h"

// DFR Custom Dependencies
#include "ifile_system.hpp"

namespace application {

class FatFs : public IFileSystem {
private:
	bool FileDoesNotExist(char* path);
	bool EvaluateResult(ErrorFlags flag);

	char* logical_drive_path_;
	FATFS& file_system_;
	FIL& file_;

	FILINFO file_info_;
	FRESULT fs_result_;

	UINT num_bytes_read_ = 0;
	UINT num_bytes_written_ = 0;

	uint32_t total_space_ = 0;
	uint32_t free_space_ = 0;

	static constexpr uint8_t kMaxDrivePathLength = 4; // Drive letter, colon, backslash, terminating null character
	static constexpr BYTE kForceMount = 1;

public:
	FatFs(char logical_drive_path[kMaxDrivePathLength], FATFS& file_system, FIL& file);
	~FatFs();

	bool Mount() override;
	bool Unmount() override;
	bool FileExists(char* path) override;
	bool CreateFile(char *path) override;
	bool OpenFile(char* path, const char* access_mode) override;
	bool CloseFile() override;
	bool WriteFile(char *data) override;
	bool ReadFile(char* buffer) override;
	bool CreateDirectory (char *name) override;

	// Will temporarily keep this here, away from the interface, until it is
	// decided whether this logic will be necessary. Perhaps for detecting whether
	// the storage is full
	void CheckStorageCapacity();
};


} // namespace application

#endif // FAT_FS_H
