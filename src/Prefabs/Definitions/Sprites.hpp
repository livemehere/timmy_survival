#pragma once

#include "../Types/SpritePreset.hpp"

namespace SpriteClips {
inline const SpriteClip PLAYER_IDLE = {
    .name = "Idle",
    .texturePath = "../assets/source.png",
    .startX = 128,
    .startY = 32,
    .frameWidth = 16,
    .frameHeight = 32,
    .frameCount = 2,
    .frameSpeed = 0.5f,
    .loop = true,
};

inline const SpriteClip PLAYER_WALK = {
    .name = "Walk",
    .texturePath = "../assets/source.png",
    .startX = 192,
    .startY = 32,
    .frameWidth = 16,
    .frameHeight = 32,
    .frameCount = 4,
    .frameSpeed = 0.15f,
    .loop = true,
};

inline const SpriteClip KNIGHT_WALK = {
    .name = "Walk",
    .texturePath = "../assets/source.png",
    .startX = 192,
    .startY = 64,
    .frameWidth = 16,
    .frameHeight = 32,
    .frameCount = 4,
    .frameSpeed = 0.15f,
    .loop = true,
};

inline const SpriteClip WIZARD_FEMALE_WALK = {
    .name = "Walk",
    .texturePath = "../assets/source.png",
    .startX = 192,
    .startY = 128,
    .frameWidth = 16,
    .frameHeight = 32,
    .frameCount = 4,
    .frameSpeed = 0.15f,
    .loop = true,
};

inline const SpriteClip WIZARD_MALE_WALK = {
    .name = "Walk",
    .texturePath = "../assets/source.png",
    .startX = 192,
    .startY = 160,
    .frameWidth = 16,
    .frameHeight = 32,
    .frameCount = 4,
    .frameSpeed = 0.15f,
    .loop = true,
};

inline const SpriteClip DINOSAUR_WALK = {
    .name = "Walk",
    .texturePath = "../assets/source.png",
    .startX = 192,
    .startY = 224,
    .frameWidth = 16,
    .frameHeight = 32,
    .frameCount = 4,
    .frameSpeed = 0.15f,
    .loop = true,
};

inline const SpriteClip BOSS_BRUTE_WALK = {
    .name = "Walk",
    .texturePath = "../assets/source.png",
    .startX = 144,
    .startY = 256,
    .frameWidth = 32,
    .frameHeight = 48,
    .frameCount = 4,
    .frameSpeed = 0.18f,
    .loop = true,
};

inline const SpriteClip GOLEM_WALK = {
    .name = "Walk",
    .texturePath = "../assets/source.png",
    .startX = 144,
    .startY = 304,
    .frameWidth = 32,
    .frameHeight = 48,
    .frameCount = 4,
    .frameSpeed = 0.18f,
    .loop = true,
};

inline const SpriteClip VENOM_WALK = {
    .name = "Walk",
    .texturePath = "../assets/source.png",
    .startX = 144,
    .startY = 352,
    .frameWidth = 32,
    .frameHeight = 48,
    .frameCount = 4,
    .frameSpeed = 0.18f,
    .loop = true,
};

inline const SpriteClip COIN_IDLE = {
    .name = "Idle",
    .texturePath = "../assets/source.png",
    .startX = 288,
    .startY = 272,
    .frameWidth = 8,
    .frameHeight = 8,
    .frameCount = 4,
    .frameSpeed = 0.1f,
    .loop = true,
};

inline const SpriteClip ENERGY_BALL = {
    .name = "EnergyBall",
    .texturePath = "../assets/source.png",
    .startX = 288,
    .startY = 0,
    .frameWidth = 16,
    .frameHeight = 16,
    .frameCount = 4,
    .frameSpeed = 0.15f,
    .loop = true,
};

inline const SpriteClip KNIFE = {
    .name = "Knife",
    .texturePath = "../assets/source.png",
    .startX = 288,
    .startY = 16,
    .frameWidth = 16,
    .frameHeight = 16,
    .frameCount = 1,
    .frameSpeed = 0.1f,
    .loop = false,
};

inline const SpriteClip ORBIT_BLADE = {
    .name = "OrbitBlade",
    .texturePath = "../assets/source.png",
    .startX = 304,
    .startY = 112,
    .frameWidth = 16,
    .frameHeight = 32,
    .frameCount = 1,
    .frameSpeed = 0.1f,
    .loop = false,
};
} // namespace SpriteClips

namespace SpritePresets {
inline const SpritePreset PLAYER = {
    .clips = {SpriteClips::PLAYER_IDLE, SpriteClips::PLAYER_WALK},
    .defaultClipName = "Idle",
    .scale = {1.0f, 1.0f},
    .anchorRatio = {0.5f, 0.75f},
    .offset = {0.0f, 0.0f},
    .tint = WHITE,
};

inline const SpritePreset COIN = {
    .clips = {SpriteClips::COIN_IDLE},
    .defaultClipName = "Idle",
    .scale = {1.0f, 1.0f},
    .anchorRatio = {0.5f, 0.5f},
    .offset = {0.0f, 0.0f},
    .tint = WHITE,
};

inline const SpritePreset ENERGY_BALL = {
    .clips = {SpriteClips::ENERGY_BALL},
    .defaultClipName = "EnergyBall",
    .scale = {0.4f, 0.4f},
    .anchorRatio = {0.5f, 0.5f},
    .offset = {0.0f, 0.0f},
    .tint = WHITE,
};

inline const SpritePreset KNIGHT = {
    .clips = {SpriteClips::KNIGHT_WALK},
    .defaultClipName = "Walk",
    .scale = {1.0f, 1.0f},
    .anchorRatio = {0.5f, 0.75f},
    .offset = {0.0f, 0.0f},
    .tint = WHITE,
};

inline const SpritePreset WIZARD_FEMALE = {
    .clips = {SpriteClips::WIZARD_FEMALE_WALK},
    .defaultClipName = "Walk",
    .scale = {1.0f, 1.0f},
    .anchorRatio = {0.5f, 0.75f},
    .offset = {0.0f, 0.0f},
    .tint = WHITE,
};

inline const SpritePreset WIZARD_MALE = {
    .clips = {SpriteClips::WIZARD_MALE_WALK},
    .defaultClipName = "Walk",
    .scale = {1.0f, 1.0f},
    .anchorRatio = {0.5f, 0.75f},
    .offset = {0.0f, 0.0f},
    .tint = WHITE,
};

inline const SpritePreset DINOSAUR = {
    .clips = {SpriteClips::DINOSAUR_WALK},
    .defaultClipName = "Walk",
    .scale = {1.0f, 1.0f},
    .anchorRatio = {0.5f, 0.75f},
    .offset = {0.0f, 0.0f},
    .tint = WHITE,
};

inline const SpritePreset BOSS_BRUTE = {
    .clips = {SpriteClips::BOSS_BRUTE_WALK},
    .defaultClipName = "Walk",
    .scale = {1.0f, 1.0f},
    .anchorRatio = {0.5f, 0.75f},
    .offset = {0.0f, 0.0f},
    .tint = WHITE,
};

inline const SpritePreset GOLEM = {
    .clips = {SpriteClips::GOLEM_WALK},
    .defaultClipName = "Walk",
    .scale = {1.0f, 1.0f},
    .anchorRatio = {0.5f, 0.75f},
    .offset = {0.0f, 0.0f},
    .tint = WHITE,
};

inline const SpritePreset VENOM = {
    .clips = {SpriteClips::VENOM_WALK},
    .defaultClipName = "Walk",
    .scale = {1.0f, 1.0f},
    .anchorRatio = {0.5f, 0.75f},
    .offset = {0.0f, 0.0f},
    .tint = WHITE,
};

inline const SpritePreset ORBIT_BLADE = {
    .clips = {SpriteClips::ORBIT_BLADE},
    .defaultClipName = "OrbitBlade",
    .scale = {1.0f, 1.0f},
    .anchorRatio = {0.5f, 0.75f},
    .offset = {0.0f, 0.0f},
    .tint = WHITE,
};
} // namespace SpritePresets
