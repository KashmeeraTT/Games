#include "ScoreManager.h"
#include <iostream>

ScoreManager::ScoreManager(const std::string& fname) : filename(fname) {}

void ScoreManager::addMatch(const std::string& winner, int winnerScore, const std::string& loser, int loserScore) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << "Winner: " << winner << " (" << winnerScore << ") | Loser: " << loser << " (" << loserScore << ")\n";
        file.close();
    }
}

std::vector<std::string> ScoreManager::getHistory(int offset, int limit) const {
    std::ifstream file(filename);
    std::deque<std::string> lines;
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
    }

    std::vector<std::string> result;
    int startIndex = (int)lines.size() - 1 - offset;
    for (int i = 0; i < limit && startIndex - i >= 0; ++i) {
        result.push_back(lines[startIndex - i]);
    }

    return result;
}

int ScoreManager::getTotalMatches() const {
    std::ifstream file(filename);
    int count = 0;
    std::string line;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            count++;
        }
    }
    return count;
}
