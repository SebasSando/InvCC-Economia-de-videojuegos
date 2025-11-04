#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <mutex> 
#include <filesystem> 

class Logger {
public:
    // Define log levels
    enum Level { INFO, WARNING, ERROR, GAME_EVENT };

    // The Singleton accessor function
    static Logger& GetInstance() {
        static Logger instance; 
        return instance;
    }

    /**
     * @brief Writes a message to the console and the log file.
     * @param level The severity or type of the log message.
     * @param message The main content of the log message.
     */
    void Print(Level level, const std::string& message) {
        // Use a lock to prevent concurrent writes from different threads
        std::lock_guard<std::mutex> lock(m_mutex);

        // Get current time
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        // Use a mutable copy for std::localtime
        std::tm tm_struct = *std::localtime(&now); 

        // Determine log level prefix
        std::string levelStr;
        switch (level) {
            case INFO:       levelStr = "[INFO]"; break;
            case WARNING:    levelStr = "[WARNING]"; break;
            case ERROR:      levelStr = "[ERROR]"; break;
            case GAME_EVENT: levelStr = "[GAME_EVENT]"; break;
        }

        std::stringstream time_ss;
        time_ss << std::put_time(&tm_struct, "%Y-%m-%d %H:%M:%S"); 
        std::string timeStr = time_ss.str();
        
        std::string logLine = timeStr + " " + levelStr + " " + message;

        std::cout << logLine << std::endl;

        if (m_logFile.is_open()) {
            m_logFile << logLine << std::endl;
            m_logFile.flush();
        }
    }

    Logger(Logger const&) = delete;
    void operator=(Logger const&) = delete;

private:
    std::ofstream m_logFile;
    std::mutex m_mutex;
    bool m_isShuttingDown = false;
    
    // Private constructor
    Logger() {
        using namespace std::chrono;

        auto now = system_clock::now();
        auto timeT = system_clock::to_time_t(now);
        std::tm tm_struct = *std::localtime(&timeT);

        // Build timestamp string: YYYYMMDD_HHMMSS
        std::stringstream ss;
        ss << std::put_time(&tm_struct, "%Y%m%d_%H%M%S");

        // Create /logs folder if it doesn't exist
        std::filesystem::path logDir = std::filesystem::current_path() / "logs";
        std::filesystem::create_directories(logDir);

        // Construct filename
        std::string filename = "game_session_" + ss.str() + ".log";
        std::filesystem::path fullPath = logDir / filename;

        // Open the log file
        m_logFile.open(fullPath, std::ios::out);
        if (m_logFile.is_open()) {
            std::cout << "Logger: Writing to " << fullPath << std::endl;
            Print(INFO, "--- NEW GAME SESSION STARTED ---");
        } else {
            std::cerr << "ERROR: Failed to open log file: " << fullPath << std::endl;
        }

        std::atexit([]() {
            Logger::GetInstance().~Logger();
        });
    }
    
    ~Logger() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_isShuttingDown) {
            m_isShuttingDown = true;
            if (m_logFile.is_open()) {
                m_logFile << "[INFO] --- GAME SESSION ENDED ---" << std::endl;
                m_logFile.close();
            }
        }
    }
};

// Define a simple macro for convenience
#define LogMessage(level, message) Logger::GetInstance().Print(level, message)

#endif // LOGGER_HPP