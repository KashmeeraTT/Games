#pragma once

// Constants.h - Holds all magic numbers and configurations for the game
namespace Constants {
    const int WINNING_SCORE = 5;
    const float DEFAULT_BALL_SPEED = 10.0f;
    const float PLAYER_SPEED = 12.0f;

    // Paddle size
    const float PADDLE_WIDTH = 20.0f;
    const float PADDLE_HEIGHT = 120.0f;

    // Ball size
    const float BALL_SIZE = 20.0f;

    // Obstacle size
    const float OBSTACLE_WIDTH = 30.0f;
    const float OBSTACLE_HEIGHT = 150.0f;
    const float OBSTACLE_SPEED = 3.0f;

    const float SHAKE_DURATION_HIT = 0.1f;
    const float SHAKE_DURATION_SCORE = 0.3f;
    const float SHAKE_DECAY = 0.016f;

    enum class GameState { MENU, DIFFICULTY_SELECT, NAME_ENTRY, PLAYING, GAME_OVER, HISTORY, ERROR_STATE };
    enum class GameMode { SINGLE_PLAYER, MULTIPLAYER };
    enum class AIDifficulty { EASY, MEDIUM, HARD };
    enum class PaddleSide { LEFT, RIGHT };
}
