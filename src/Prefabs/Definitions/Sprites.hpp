#pragma once

#include "../Types/SpritePreset.hpp"

namespace SpriteClips {
inline const SpriteClip PLAYER_IDLE = {
    "Idle", "../assets/source.png", 128, 32, 16, 32, 2, 0.5f, true};

inline const SpriteClip PLAYER_WALK = {
    "Walk", "../assets/source.png", 192, 32, 16, 32, 4, 0.15f, true};

inline const SpriteClip KNIGHT_IDLE = {
    "Idle", "../assets/source.png", 128, 64, 16, 32, 2, 0.5f, true};

inline const SpriteClip KNIGHT_WALK = {
    "Walk", "../assets/source.png", 192, 64, 16, 32, 4, 0.15f, true};

inline const SpriteClip COIN_IDLE = {
    "Idle", "../assets/source.png", 288, 272, 8, 8, 4, 0.1f, true};

inline const SpriteClip ENERGY_BALL = {
    "EnergyBall", "../assets/source.png", 288, 0, 16, 16, 4, 10.0f / 60.0f,
    true};

inline const SpriteClip KNIFE = {
    "Knife", "../assets/source.png", 288, 16, 16, 16, 1, 0.1f, false};
} // namespace SpriteClips

namespace SpritePresets {
inline const SpritePreset PLAYER = {{SpriteClips::PLAYER_IDLE, SpriteClips::PLAYER_WALK},
                                    "Idle",
                                    {1.0f, 1.0f},
                                    {0.5f, 0.75f},
                                    {0.0f, 0.0f},
                                    WHITE};

inline const SpritePreset COIN = {{SpriteClips::COIN_IDLE},
                                  "Idle",
                                  {1.0f, 1.0f},
                                  {0.5f, 0.5f},
                                  {0.0f, 0.0f},
                                  WHITE};

inline const SpritePreset ENERGY_BALL = {{SpriteClips::ENERGY_BALL},
                                         "EnergyBall",
                                         {0.4f, 0.4f},
                                         {0.5f, 0.5f},
                                         {0.0f, 0.0f},
                                         WHITE};
} // namespace SpritePresets
