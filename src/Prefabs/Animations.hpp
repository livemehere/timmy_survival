#pragma once

#include "../Components/Render/SpriteRenderer.hpp"

namespace Animation {
inline const AnimationConfig PLAYER_IDLE = {
    "Idle", "../assets/source.png", 128, 32, 16, 32, 2, 0.5f, true};

inline const AnimationConfig PLAYER_WALK = {
    "Walk", "../assets/source.png", 192, 32, 16, 32, 4, 0.15f, true};

inline const AnimationConfig KNIGHT_IDLE = {
    "Idle", "../assets/source.png", 128, 64, 16, 32, 2, 0.5f, true};

inline const AnimationConfig KNIGHT_WALK = {
    "Walk", "../assets/source.png", 192, 64, 16, 32, 4, 0.15f, true};

inline const AnimationConfig COIN_IDLE = {
    "Idle", "../assets/source.png", 288, 272, 8, 8, 4, 0.1f, true};

// weapon
inline const AnimationConfig ENERGY_BALL = {
    "EnergyBall", "../assets/source.png", 288, 0, 16, 16, 4, 10.0f / 60.0f,
    true};

// projectile animation
inline const AnimationConfig KNIFE = {
    "Knife", "../assets/source.png", 288, 16, 16, 16, 1, 0.1f, false};
} // namespace Animation
