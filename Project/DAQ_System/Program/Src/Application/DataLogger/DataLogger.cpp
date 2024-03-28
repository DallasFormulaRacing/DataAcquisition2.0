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

bool DataLogger::Enable() { return file_system_->Mount(); }

bool DataLogger::Disable() { return file_system_->Unmount(); }

bool DataLogger::Start() {
	FindUniqueFileName();
	printf("Creating new file: %s\n", file_name_);

	// Create CSV with header row
	return file_system_->CreateFile(file_name_) &&
			file_system_->OpenFile(file_name_, (char*)"a") &&
			file_system_->WriteFile((char*)"Timestamp,Linpot1,Linpot2,Linpot3,Linpot4\n");
}

void DataLogger::FindUniqueFileName() {
	strcpy(file_name_, "\0");
	bool file_found = false;
	uint32_t i = 0;

	do {
		snprintf(file_name_, sizeof(file_name_), "data%ld.csv", i);
		file_found = file_system_->FileExists(file_name_);
		i++;
	} while(file_found);
}

bool DataLogger::RecordDataSample(DataPayload& data) {
	char* csv_row = NULL;
	int length = data.GetCsvFormatLength();
	csv_row = new char[length + 1]();

	data.CsvFormat(csv_row, length + 1);
	bool status = file_system_->WriteFile(csv_row);

	delete csv_row;
	return status;
}

bool DataLogger::Stop() {
	return file_system_->CloseFile();
}



} // namespace application
