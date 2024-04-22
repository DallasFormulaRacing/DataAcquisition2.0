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
#include "../circular_queue.hpp"
#include "../data_payload.hpp"
#include "../FileSystem/ifile_system.hpp"
#include "../../Platform/GPIO/igpio.hpp"

namespace application {

class DataLogger {
public:
	/// Initializes a DataLogger object.
	/// @param file_system An implementation of a file system following
	/// the @ref application.IFileSystem abstract interface. The current
	/// implementation of the `DataLogger` requires the following functionalities:
	/// - Mounting/unmounting.
	/// - File searching.
	/// - File opening/closing.
	/// - File writing.
	/// @param user_input An implementation of a GPIO following
	/// the @ref platform.IGpio abstract interface. The following functionalities
	/// are required:
	/// - Reading the logical `HIGH` and `LOW` states.
	/// - Setting a flag for detecting whether the GPIO state has cleared, and
	///	  clearing this flag after it has been read.
	/// @param queue A FIFO data structure to be shared with sensors for sharing
	/// data samples.
	/// @param storage_connected_observer A pointer to a flag that monitors whether the
	/// storage block device is connected or ejected. To be used as a boolean
	/// variable with only binary values.
	DataLogger(std::shared_ptr<IFileSystem> file_system,
			   std::shared_ptr<platform::IGpio> user_input,
			   CircularQueue<DataPayload>& queue,
			   uint8_t& storage_connected_observer);

	~DataLogger();

	/// Informs the `DataLogger` to evaluate its current state and perform
	/// the according tasks.
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
	std::shared_ptr<platform::IGpio> user_input_;
	CircularQueue<DataPayload>& queue_;
	uint8_t& storage_connected_observer_;

	char file_name_[16] = "\0";
	bool logging_enabled_ = false;

	Idle idle_state_;
	Standby standby_state_;
	Logging logging_state_;
	State* current_state_{&idle_state_};
};

} // namespace application

#endif // DATA_LOGGER_H
