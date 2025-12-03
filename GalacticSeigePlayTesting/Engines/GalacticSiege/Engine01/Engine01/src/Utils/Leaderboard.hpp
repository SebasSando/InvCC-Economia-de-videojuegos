#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <string>
#include <vector>

/**
 * @brief Represents a single leaderboard entry with score, outcome and timestamp.
 */
struct LeaderboardEntry {
	int score;
	std::string outcome;
	std::string timestamp;
};

/**
 * @brief Singleton utility to load, update and persist leaderboard data.
 *
 * Stores entries in a plain text file located at ./assets/leaderboard.txt,
 * keeping the highest scores along with the outcome and timestamp of each attempt.
 */
class Leaderboard {
public:
	/**
	 * @brief Provides access to the singleton instance.
	 */
	static Leaderboard& GetInstance();

	/**
	 * @brief Adds a new score entry and persists the updated leaderboard.
	 *
	 * @param score Final player score.
	 * @param outcome Text describing the result (e.g., "Victory", "Defeat").
	 * @return true if the score is strictly higher than the previous best.
	 */
	bool AddEntry(int score, const std::string& outcome);

	/**
	 * @brief Retrieves the current leaderboard entries ordered by score.
	 */
	std::vector<LeaderboardEntry> GetEntries();
	/**
	 * @brief Returns the highest score stored in the leaderboard. Returns 0 if empty.
	 */
	int GetTopScore();
	/**
	 * @brief Clears all stored entries and resets the leaderboard file.
	 */
	void Reset();

private:
	static constexpr const char* kLeaderboardFilePath = "./assets/leaderboard.txt";
	static constexpr std::size_t kMaxEntries = 10;

	std::vector<LeaderboardEntry> entries;
	bool loaded = false;

	Leaderboard() = default;
	void Load();
	void Save();
	static std::string CurrentTimestamp();
	static LeaderboardEntry ParseLine(const std::string& line);
	static std::string SerialiseEntry(const LeaderboardEntry& entry);
};

#endif // LEADERBOARD_HPP
