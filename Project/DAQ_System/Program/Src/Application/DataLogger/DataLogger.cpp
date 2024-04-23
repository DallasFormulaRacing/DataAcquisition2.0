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

DataLogger::DataLogger(std::shared_ptr<IFileSystem> file_system,
					   std::shared_ptr<platform::IGpio> user_input,
					   CircularQueue<DataPayload>& queue,
					   uint8_t& storage_connected_observer,
					   bool& logging_enabled_sharer)
  : file_system_(file_system),
	user_input_(user_input),
	queue_(queue),
	storage_connected_observer_(storage_connected_observer),
	logging_enabled_(logging_enabled_sharer) { }

DataLogger::~DataLogger() { }

bool DataLogger::CreateCsvFile() {
	FindUniqueFileName();
	printf("Creating new file: %s\n", file_name_);

	// Create CSV with header row
	return file_system_->CreateFile(file_name_) &&
			file_system_->OpenFile(file_name_, (char*)"a") &&
			file_system_->WriteFile((char*)
					"Timestamp,"

					"Linpot1,"
					"Linpot2,"
					"Linpot3,"
					"Linpot4,"

					"AccX,"
					"AccY,"
					"AccZ,"

					"GyroX,"
					"GyroY,"
					"GyroZ,"

					"RPM,"
					"TPS,"
					"FuelOpenTime,"
					"IgnitionAngle,"

					"Barometer,"
					"MAP,"
					"Lambda,"

					"AnalogInput1,"
					"AnalogInput2,"
					"AnalogInput3,"
					"AnalogInput4,"
					"AnalogInput5,"
					"AnalogInput6,"
					"AnalogInput7,"
					"AnalogInput8,"

					"BatteryVoltage,"
					"AirTemp,"
					"CoolantTemp\n");
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

	delete[] csv_row;
	return status;
}

void DataLogger::SetState(State* new_state) {
	if (new_state != nullptr) {
		current_state_->Exit(*this);
		current_state_ = new_state;
		current_state_->Enter(*this);
	}
}

void DataLogger::Run() {
	current_state_->Compute(*this);
}


//*************************************************
//					Idle State
//*************************************************
void DataLogger::Idle::Enter(DataLogger& context) {
	context.file_system_->Unmount();
	printf("[DataLogger] Entering Idle state.\n");
}

void DataLogger::Idle::Compute(DataLogger& context) {
	if (context.storage_connected_observer_) {
		context.SetState(&context.standby_state_);
	}
}

void DataLogger::Idle::Exit(DataLogger& context) {
	context.file_system_->Mount();
	printf("[DataLogger] Leaving Idle state.\n");
}


//*************************************************
//					Standby State
//*************************************************
void DataLogger::Standby::Enter(DataLogger& context) {
	printf("[DataLogger] Entering Standby state.\n");
}

void DataLogger::Standby::Compute(DataLogger& context) {
	if (!context.storage_connected_observer_) {
		context.SetState(&context.idle_state_);
	}

	if (context.user_input_->ToggleDetected()) {
		context.logging_enabled_ = context.user_input_->Read();

		if (context.logging_enabled_) {
			context.SetState(&context.logging_state_);
		}
	}
}

void DataLogger::Standby::Exit(DataLogger& context) {
	printf("[DataLogger] Leaving Standby state.\n");
}


//*************************************************
//					Logging State
//*************************************************
void DataLogger::Logging::Enter(DataLogger& context) {
	printf("[DataLogger] Entering Logging state.\n");
	context.CreateCsvFile();

	context.queue_.Lock();
	context.queue_.Clear();
	context.queue_.Unlock();
}

void DataLogger::Logging::Compute(DataLogger& context) {
	if (!context.storage_connected_observer_) {
		context.SetState(&context.idle_state_);
	}

	if (context.user_input_->ToggleDetected()) {
		context.logging_enabled_ = context.user_input_->Read();

		if (!context.logging_enabled_) {
			context.SetState(&context.standby_state_);
		}
	}


	context.queue_.Lock();

	DataPayload received_data;
	if(!context.queue_.IsEmpty()) {
		received_data = context.queue_.Dequeue();
		context.RecordDataSample(received_data);
	}

	context.queue_.Unlock();
}

void DataLogger::Logging::Exit(DataLogger& context) {
	context.file_system_->CloseFile();
	printf("[DataLogger] Leaving Logging state.\n");
}

} // namespace application
