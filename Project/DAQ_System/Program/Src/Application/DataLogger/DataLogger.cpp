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

#include "DataLogger.hpp"

namespace application {

DataLogger::DataLogger(std::shared_ptr<IFileSystem> file_system)
  : file_system_(file_system) { }

DataLogger::~DataLogger() { }

bool DataLogger::Enable() {
	return file_system_->Mount();
}

bool DataLogger::Disable() {
	return file_system_->Unmount();
}

void DataLogger::Start() {
	// Create unique file name
	bool file_found = false;
	uint8_t i = 0;

	do {
		snprintf(file_name_, sizeof(file_name_), "data%d.csv", i);
		file_found = file_system_->FileExists(file_name_);
	} while(!file_found);

	printf("Creating new file: %s\n", file_name_);

	bool status = file_system_->CreateFile(file_name_);

	if (status == false) {
		printf("Failed to create new file\n");
	}

	// Create CSV Header row
	file_system_->OpenFile(file_name_, (char*)"a");
	file_system_->WriteFile((char*)"Timestamp,Linpot1,Linpot2,Linpot3,Linpot4");

}

void DataLogger::RecordDataSample(DataPayload& data) {
	char* csv_row = NULL;
	int length = data.GetLength();
//	csv_row = (char*)malloc(length + 1);
	csv_row = new char[length + 1]();

	data.CsvFormat(csv_row, length + 1);
	file_system_->WriteFile(csv_row);

//	free(csv_row);
	delete csv_row;
}

void DataLogger::Stop() {
	file_system_->CloseFile();
}



} // namespace application
