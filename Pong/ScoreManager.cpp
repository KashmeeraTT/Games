#include "ScoreManager.h"
#include <iostream>

ScoreManager::ScoreManager(const std::string& fname) : filename(fname) {}

void ScoreManager::addMatch(const std::string& winner, int winnerScore, const std::string& loser, int loserScore) {
    bool isEmpty = true;
    {
        std::ifstream inFile(filename);
        if (inFile.peek() != std::ifstream::traits_type::eof()) {
            isEmpty = false;
        }
    }

    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        if (isEmpty) {
            file << "# Pong Match History v1\n";
        }
        file << "Winner: " << winner << " (" << winnerScore << ") | Loser: " << loser << " (" << loserScore << ")\n";
        file.close();
    }
}

int ScoreManager::getHistoryPage(int offset, int limit, std::vector<std::string>& outLines) const {
    std::ifstream file(filename);
    std::deque<std::string> lines;
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (!line.empty() && line[0] == '#') continue;
            lines.push_back(line);
        }
    }

    outLines.clear();
    int startIndex = (int)lines.size() - 1 - offset;
    for (int i = 0; i < limit && startIndex - i >= 0; ++i) {
        outLines.push_back(lines[startIndex - i]);
    }

    return static_cast<int>(lines.size());
}

