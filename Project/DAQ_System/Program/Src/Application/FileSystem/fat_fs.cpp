#include "fat_fs.hpp"

namespace application {

FatFs::FatFs(char logical_drive_path[kMaxDrivePathLength], FATFS& file_system, FIL& file)
  : logical_drive_path_(logical_drive_path), file_system_(file_system), file_(file) { }

FatFs::~FatFs() { }

bool FatFs::Mount () {
	fs_result_ = f_mount(&file_system_, logical_drive_path_, kForceMount);
	return EvaluateResult(ErrorFlags::Mounting);
}

bool FatFs::Unmount () {
	fs_result_ = f_mount(NULL, logical_drive_path_, kForceMount);
	return EvaluateResult(ErrorFlags::Unmounting);
}

bool FatFs::FileExists(char* path) {
	fs_result_ = f_stat(path, &file_info_);
	return EvaluateResult(ErrorFlags::FileNotFound);
}

bool FatFs::CreateFile(char* path) {
	return FileDoesNotExist(path) &&
			OpenFile(path, kEmtpyFileAndWriteMode) &&
			CloseFile();
}

bool FatFs::OpenFile(char* path, const char* access_mode) {
	if (access_mode == nullptr) {
		return false;
	}

	BYTE fatfs_mode = '\0';

	// Convert POSIX compliant flag to FATFS flag
	if (access_mode == kReadOnlyMode) {
		fatfs_mode = FA_READ;
	} else if (strcmp(access_mode, kReadWriteMode) == 0) {
		fatfs_mode =  FA_READ | FA_WRITE;
	} else if (strcmp(access_mode, kEmtpyFileAndWriteMode) == 0) {
		fatfs_mode = FA_CREATE_ALWAYS | FA_WRITE;;
	} else if (strcmp(access_mode, kEmptyFileAndReadWriteMode) == 0) {
		fatfs_mode = FA_CREATE_ALWAYS | FA_WRITE | FA_READ;
	} else if (strcmp(access_mode, kAppendFileAndWriteMode) == 0) {
		fatfs_mode = FA_OPEN_APPEND | FA_WRITE;
	} else if (strcmp(access_mode, kAppendFileAndReadWriteMode) == 0) {
		fatfs_mode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
	} else if (strcmp(access_mode, kNewFileAndWriteMode) == 0) {
		fatfs_mode = FA_CREATE_NEW | FA_WRITE;
	} else if (strcmp(access_mode, kNewFileAndReadWriteMode) == 0) {
		fatfs_mode = FA_CREATE_NEW | FA_WRITE | FA_READ;
	} else {
		return false;
	}

	fs_result_ = f_open(&file_, path, fatfs_mode);
	return EvaluateResult(ErrorFlags::OpenFile);
}

bool FatFs::CloseFile() {
	fs_result_ = f_close(&file_);
	return EvaluateResult(ErrorFlags::CloseFile);
}

bool FatFs::WriteFile(char *data) {
	fs_result_ = f_write(&file_, data, strlen(data), &num_bytes_written_);
	return EvaluateResult(ErrorFlags::WriteFile);
}

bool FatFs::ReadFile(char* buffer) {
	fs_result_ = f_read (&file_, buffer, f_size(&file_), &num_bytes_read_);
	bool status = EvaluateResult(ErrorFlags::ReadFile);

	if (!status) {
		free(buffer);
		buffer = nullptr;
	}

	return status;
}

bool FatFs::CreateDirectory(char *name) {
	fs_result_ = f_mkdir(name);
	return EvaluateResult(ErrorFlags::CreateDirectory);
}

void FatFs::CheckStorageCapacity () {
	DWORD fre_clust;
	FATFS *pUSBHFatFS;
	f_getfree("", &fre_clust, &pUSBHFatFS);

	// TODO: Compute space, as the below variables may be counting
	// sectors, not bytes
	// http://elm-chan.org/fsw/ff/doc/getfree.html
	total_space_ = (uint32_t)((pUSBHFatFS->n_fatent - 2) * pUSBHFatFS->csize * 0.5);
	free_space_ = (uint32_t)(fre_clust * pUSBHFatFS->csize * 0.5);
}

bool FatFs::FileDoesNotExist(char* path) {
	fs_result_ = f_stat(path, &file_info_);
	if (fs_result_ == FR_OK) {
		return false;
	}

	return true;
}

bool FatFs::EvaluateResult(ErrorFlags flag) {
	if (fs_result_ == FR_OK) {
		return true;
	}

	SetError(flag);
	return false;
}

} // namespace application
