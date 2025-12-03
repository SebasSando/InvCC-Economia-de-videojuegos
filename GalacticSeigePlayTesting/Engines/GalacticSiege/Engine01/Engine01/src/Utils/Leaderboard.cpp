#include "Leaderboard.hpp"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>

Leaderboard& Leaderboard::GetInstance() {
	static Leaderboard instance;
	return instance;
}

bool Leaderboard::AddEntry(int score, const std::string& outcome) {
	Load();
	int previousBest = entries.empty() ? std::numeric_limits<int>::min() : entries.front().score;
	LeaderboardEntry entry{ score, outcome, CurrentTimestamp() };
	entries.push_back(entry);
	std::sort(entries.begin(), entries.end(), [](const LeaderboardEntry& lhs, const LeaderboardEntry& rhs) {
		if (lhs.score != rhs.score) {
			return lhs.score > rhs.score;
		}
		return lhs.timestamp < rhs.timestamp;
	});
	if (entries.size() > kMaxEntries) {
		entries.resize(kMaxEntries);
	}
	Save();
	return score > previousBest;
}

std::vector<LeaderboardEntry> Leaderboard::GetEntries() {
	Load();
	return entries;
}

int Leaderboard::GetTopScore() {
	Load();
	if (entries.empty()) {
		return 0;
	}
	return entries.front().score;
}

void Leaderboard::Reset() {
	entries.clear();
	loaded = true;
	Save();
	loaded = false;
}

void Leaderboard::Load() {
	if (loaded) {
		return;
	}
	std::ifstream input(kLeaderboardFilePath);
	if (!input.is_open()) {
		loaded = true;
		return;
	}
	std::string line;
	while (std::getline(input, line)) {
		if (line.empty()) {
			continue;
		}
		LeaderboardEntry entry = ParseLine(line);
		if (entry.score >= 0) {
			entries.push_back(entry);
		}
	}
	input.close();
	std::sort(entries.begin(), entries.end(), [](const LeaderboardEntry& lhs, const LeaderboardEntry& rhs) {
		if (lhs.score != rhs.score) {
			return lhs.score > rhs.score;
		}
		return lhs.timestamp < rhs.timestamp;
	});
	if (entries.size() > kMaxEntries) {
		entries.resize(kMaxEntries);
	}
	loaded = true;
}

void Leaderboard::Save() {
	std::ofstream output(kLeaderboardFilePath, std::ios::trunc);
	if (!output.is_open()) {
		return;
	}
	for (const auto& entry : entries) {
		output << SerialiseEntry(entry) << '\n';
	}
	output.close();
}

std::string Leaderboard::CurrentTimestamp() {
	auto now = std::chrono::system_clock::now();
	std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
	std::tm tm {};
#ifdef _WIN32
	localtime_s(&tm, &nowTime);
#else
	localtime_r(&nowTime, &tm);
#endif
	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d %H:%M");
	return oss.str();
}

LeaderboardEntry Leaderboard::ParseLine(const std::string& line) {
	LeaderboardEntry entry{ -1, "", "" };
	std::stringstream ss(line);
	std::string scoreStr;
	std::string outcome;
	std::string timestamp;
	if (std::getline(ss, scoreStr, '|') &&
		std::getline(ss, outcome, '|') &&
		std::getline(ss, timestamp)) {
		try {
			entry.score = std::stoi(scoreStr);
		}
		catch (...) {
			entry.score = -1;
		}
		if (entry.score >= 0) {
			entry.outcome = outcome.empty() ? "Unknown" : outcome;
			entry.timestamp = timestamp.empty() ? "-" : timestamp;
		}
	}
	return entry;
}

std::string Leaderboard::SerialiseEntry(const LeaderboardEntry& entry) {
	std::ostringstream oss;
	oss << entry.score << '|' << entry.outcome << '|' << entry.timestamp;
	return oss.str();
}
