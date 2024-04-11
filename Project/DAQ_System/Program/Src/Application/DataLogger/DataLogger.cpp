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
  : file_system_(file_system) {

	dummy_data_.timestamp_ = 15;
	dummy_data_.linpot_displacement_inches_[0] = 2.5;
	dummy_data_.linpot_displacement_inches_[1] = 0.5;
	dummy_data_.linpot_displacement_inches_[2] = 1.3;
	dummy_data_.linpot_displacement_inches_[3] = 4.0;
}

DataLogger::~DataLogger() { }

bool DataLogger::CreateCsvFile() {
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

void DataLogger::SetState(State* new_state) {
	if (new_state != nullptr) {
		current_state_->Exit(*this);
		current_state_ = new_state;
		current_state_->Enter(*this);
	}
}


//==========================================

void DataLogger::Idle::Enter(DataLogger& context) {
	context.file_system_->Unmount();
	printf("[DataLogger] Entering Idle state.\n");
}

void DataLogger::Idle::Compute(DataLogger& context) {
	if (*context.block_device_connected_) {
		context.SetState(&context.standby_state_);
	}
}

void DataLogger::Idle::Exit(DataLogger& context) {
	context.file_system_->Mount();
	printf("[DataLogger] Leaving Idle state.\n");
}

//=====================================================

void DataLogger::Standby::Enter(DataLogger& context) {
	printf("[DataLogger] Entering Standby state.\n");
}

void DataLogger::Standby::Compute(DataLogger& context) {
	if (!*context.block_device_connected_) {
		context.SetState(&context.idle_state_);
	}

	if (*context.logging_mode_changed_) { //TODO: Read from GPIO and clear internally
		context.logging_enabled_ = true; // TODO: Read from GPIO

		if (context.logging_enabled_) {
			context.SetState(&context.logging_state_);
		}
	}
}

void DataLogger::Standby::Exit(DataLogger& context) {
	printf("[DataLogger] Leaving Standby state.\n");
}

//=====================================================

void DataLogger::Logging::Enter(DataLogger& context) {
	printf("[DataLogger] Entering Logging state.\n");
	context.CreateCsvFile();
}

void DataLogger::Logging::Compute(DataLogger& context) {
	if (!*context.block_device_connected_) {
		context.SetState(&context.idle_state_);
	}

	if (*context.logging_mode_changed_) { //TODO: Read from GPIO and clear internally
		context.logging_enabled_ = false; // TODO: Read from GPIO

		if (!context.logging_enabled_) {
			context.SetState(&context.standby_state_);
		}
	}



	// TODO: check the queue and log
	 context.RecordDataSample(dummy_data);


}

void DataLogger::Logging::Exit(DataLogger& context) {
	context.file_system_->CloseFile();
	printf("[DataLogger] Leaving Logging state.\n");
}


//======================================================



} // namespace application
