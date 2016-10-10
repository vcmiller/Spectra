#pragma once
#include <fstream>
#include <iostream>

namespace spectra {
	class Log {
	private:
		// Singleton, only constructed once.
		Log();

		// No copy.
		Log(const Log& l);

		// No assign.
		void operator=(const Log& l);

		// Local, non-const storage for singleton instance.
		static Log instance;

		// Log file.
		mutable std::ofstream file;

		// Whether file has been opened.
		bool fileOpen;

		// Whether to flush after each new line.
		bool flush;

		// Whether log writes to console.
		bool writeConsole;

		// Whether log writs to a file.
		bool writeFile;

	public:
		// Destructor, close file.
		~Log();

		// Set write mode for console and file.
		static void setWriteMode(bool console, bool file);

		// Set whether to flush on newline (false by default).
		static void setFlush(bool flush);

		// Public, const reference to singleton instance.
		static const Log &log;

		// Write to log as if it were a function consuming any type of data.
		// Argument type must support writing with the << operator to an std::ostream.
		template <typename T> void operator () (const T &val, bool newline = true) const {
			(*this) << val;;
			if (newline) {
				(*this) << "\n";
			}

			if (writeFile && fileOpen && flush) {
				file.flush();
			}
		}

		// Write any data to log using pipe operator.
		// Argument type must support writing with the << operator to an std::ostream.
		template <typename T> const Log& operator << (const T &val) const {
			if (writeConsole) {
				cout << val;
			}

			if (writeFile && fileOpen) {
				file << val;

				if (flush) {
					file.flush();
				}
			}

			return (*this);
		}
	};
}