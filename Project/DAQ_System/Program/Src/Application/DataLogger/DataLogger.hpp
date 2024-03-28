/*
* Data Logger
* Author:   Cristian Cruz
*
* Email:    cris14.alex@gmail.com
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

// Standard Libraries
#include <cstring>
#include <cinttypes>
#include <memory>

// DFR Custom Dependencies
#include "../data_payload.hpp"
#include "../FileSystem/ifile_system.hpp"


namespace application {

class DataLogger {
public:
	/// @param file_system An implementation of a file system following the
	/// @ref application.IFileSystem abstract interface. The current
	/// implementation of the `DataLogger` requires the following functionalities:
	/// - Mounting/unmounting.
	/// - File searching.
	/// - File opening/closing.
	/// - File writing.
	DataLogger(std::shared_ptr<IFileSystem> file_system);

	~DataLogger();

	/// Mounts the file system. This must be done before starting the `DataLogger`.
	/// @return Whether the operation completed successfully or failed.
	bool Enable();

	/// Unmounts the file system. This must be done before the file system's
	/// block device is ejected.
	/// @return Whether the operation completed successfully or failed.
	bool Disable();

	/// Prepares a new CSV to be ready for writing data to it.
	bool Start();

	/// Logs the provided @ref application.DataPayload to the CSV file.
	/// @param data A batch of sampled data, associated with a timestamp. 
	bool RecordDataSample(DataPayload& data);

	/// Closes the current CSV file.
	bool Stop();

private:
	void FindUniqueFileName();

	std::shared_ptr<IFileSystem> file_system_;

	char file_name_[16] = "\0";
};

} // namespace application

#endif // DATA_LOGGER_H
