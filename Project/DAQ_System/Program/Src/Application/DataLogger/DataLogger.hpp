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
#include <memory>

// DFR Custom Dependencies
#include "../data_payload.hpp"
#include "../FileSystem/ifile_system.hpp"


namespace application {

class DataLogger {
public:
	DataLogger(std::shared_ptr<IFileSystem> file_system);

	~DataLogger();

	bool Enable();

	bool Disable();

	// Create CSV file/header with unique file name
	void Start();

	// Input: DataPayload instance. Use fatfs
	// queue will be checked externally
	void RecordDataSample(DataPayload& data);

	// Close file
	void Stop();

private:
	std::shared_ptr<IFileSystem> file_system_;

	char file_name_[16] = "\0";



};

} // namespace application

#endif // DATA_LOGGER_H
