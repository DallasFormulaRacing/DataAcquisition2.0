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
	
	/// A helper function for evaluating whether a FatFS operation is successful.
	/// If failed, an error flag will be set for the corresponding error indicator
	/// given in that context.
	///
	/// The functinoality is derived from the `ErrorFlags` enum defined in
	/// @ref application.IFileSystemErrorFlags.
	/// @param flag The error flag to be raised
	/// @return Whether the Fat FS operation resulted in `FR_OK`.
	bool EvaluateResult(ErrorFlags flag);

	char* logical_drive_path_;
	FATFS& file_system_;
	FIL& file_;

	FILINFO file_info_;
	FRESULT fs_result_;

	UINT num_bytes_read_ = 0;
	UINT num_bytes_written_ = 0;

	uint32_t total_kilobytes_ = 0;
	uint32_t free_kilobytes_ = 0;

	/// Accounts for the following characters:
	/// - Drive letter
	/// - Colon
	/// - Backslash
	/// - Terminating null character
	static constexpr uint8_t kMaxDrivePathLength = 4;

	/// An option that allows for an additional attempt of mounting when using
	/// file/directory operations, in the case that initial mounting had failed.
	static constexpr BYTE kForceMount = 1;

public:
	FatFs(char logical_drive_path[kMaxDrivePathLength], FATFS& file_system, FIL& file);
	~FatFs();

	/// Registers the logical drive for the block device and initializes the file
	/// system work area. Device mounting must be done before performing file or
	/// directory operations.
	///
	/// For more information, see
	/// <a href="http://elm-chan.org/fsw/ff/doc/mount.html"> f_mount() </a>.
	///
	/// @return Whether the operation completed succesfully or failed.
	bool Mount() override;

	/// Unregisters the work area. Unmounting the block device must be done before
	/// physical ejection.
	///
	/// For more information, see
	/// <a href="http://elm-chan.org/fsw/ff/doc/mount.html"> f_mount() </a>.
	///
	/// @return Whether the operation completed succesfully or failed.
	bool Unmount() override;

	/// Checks for the existence of a file or directory.
	///
	/// For more information, see
	/// <a href="http://elm-chan.org/fsw/ff/doc/stat.html"> f_stat() </a>.
	///
	/// @param path Directory or file path.
	/// @return Whether the given path exists.
	bool FileExists(char* path) override;

	/// Creates a new file, provided that the file does not already exist. 
	/// @param path Directory or file path.
	/// @return Whether the operation completed succesfully or failed.
	bool CreateFile(char *path) override;

	/// Opens a pre-existing file using POSIX-compliant flags.
	///
	/// For more information, see
	/// <a href="http://elm-chan.org/fsw/ff/doc/open.html"> f_open() </a> and
	/// <a href="https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm"> access modes </a>.
	///
	/// @param path Directory or file path.
	/// @param access_mode Determines whether to open a file for
	/// reading/writing and overwriting.
	/// @return Whether the operation completed succesfully or failed.
	bool OpenFile(char* path, const char* access_mode) override;

	/// Closes a file that was previously opened. Cached file changes are
	/// written to the volume.
	///
	/// For more information, see
	/// <a href="http://elm-chan.org/fsw/ff/doc/close.html"> f_close() </a>.
	///
	/// @return Whether the operation completed succesfully or failed.
	bool CloseFile() override;

	/// Writes to a previously opened file.
	///
	/// For more information, see
	/// <a href="http://elm-chan.org/fsw/ff/doc/write.html"> f_write() </a>.
	///
	/// @param data The content to be written to the file.
	/// @return Whether the operation completed succesfully or failed.
	bool WriteFile(char *data) override;

	/// Reads a previously opened file.
	///
	/// For more information, see
	/// <a href="http://elm-chan.org/fsw/ff/doc/read.html"> f_read() </a>.
	///
	/// @param buffer Where the content read from the file is to be stored.
	/// @return Whether the operation completed succesfully or failed.
	bool ReadFile(char* buffer) override;

	/// Creates a folder directory.
	///
	/// For more information, see
	/// <a href="http://elm-chan.org/fsw/ff/doc/mkdir.html"> f_mkdir() </a>.
	///
	/// @param name The name of the new directory.
	/// @return Whether the operation completed succesfully or failed.
	bool CreateDirectory (char *name) override;

	/// Updates the internal count for the block device's total and free space
	/// in Kilobytes.
	bool CheckStorageCapacity() override;

	/// @return The total space in Kilobytes. 
	uint32_t TotalSpace() override;

	/// @return The free space in Kilobytes.
	uint32_t FreeSpace() override;
};


} // namespace application

#endif // FAT_FS_H
