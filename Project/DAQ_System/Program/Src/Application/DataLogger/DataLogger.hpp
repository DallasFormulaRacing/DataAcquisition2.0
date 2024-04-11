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
#include "../../Platform/GPIO/igpio.hpp"


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
	DataLogger(std::shared_ptr<IFileSystem> file_system,
			   std::shared_ptr<platform::IGpio> gpio,
			   uint8_t* storage_connected);

	~DataLogger();

	void Run();

protected:
	class State {
	public:
		virtual ~State() = default;

		virtual void Enter(DataLogger& context) = 0;
		virtual void Compute(DataLogger& context) = 0;
		virtual void Exit(DataLogger& context) = 0;

	protected:
		State() = default;
	};

	class Idle : public State {
	public:
		Idle() = default;

		virtual void Enter(DataLogger& context) override;
		virtual void Compute(DataLogger& context) override;
		virtual void Exit(DataLogger& context) override;
	};

	class Standby : public State {
	public:
		Standby() = default;

		virtual void Enter(DataLogger& context) override;
		virtual void Compute(DataLogger& context) override;
		virtual void Exit(DataLogger& context) override;
	};

	class Logging : public State {
	public:
		Logging() = default;

		virtual void Enter(DataLogger& context) override;
		virtual void Compute(DataLogger& context) override;
		virtual void Exit(DataLogger& context) override;
	};






private:
	bool CreateCsvFile();
	void FindUniqueFileName();
	bool RecordDataSample(DataPayload& data);
	void SetState(State* new_state);


	std::shared_ptr<IFileSystem> file_system_;
	std::shared_ptr<platform::IGpio> gpio_;
	char file_name_[16] = "\0";

	Idle idle_state_;
	Standby standby_state_;
	Logging logging_state_;
	State* current_state_{&idle_state_};




	//======
	// Observer flags
	uint8_t* storage_connected_;

	bool logging_enabled_ = false;

	DataPayload dummy_data_;
};

} // namespace application

#endif // DATA_LOGGER_H
