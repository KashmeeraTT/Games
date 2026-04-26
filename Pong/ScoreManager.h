#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <deque>

// ScoreManager.h - Score tracking and file history I/O
class ScoreManager {
public:
    ScoreManager(const std::string& filename = "scores.txt");
    void addMatch(const std::string& winner, int winnerScore, const std::string& loser, int loserScore);
    std::vector<std::string> getHistory(int offset, int limit) const;
    int getTotalMatches() const;

private:
    std::string filename;
};
