# Timmy Survival

**[한국어](README-ko.md) | English**

[Play on Web](https://livemehere.github.io/timmy_survival/)

## Demo

![img](docs/screenshot.png)

<video controls src="https://github.com/livemehere/timmy_survival/blob/main/docs/sample.mp4" title="Sample Video"></video>

---

## Gameplay

Timmy Survival is a top-down survival game where you fend off endless waves of enemies.

- Each stage spawns a set number of enemies.
- **Defeat all enemies** to automatically advance to the next stage.
- After Stage 3, the game enters **infinite mode** — each cycle increases enemy HP, spawn count, and spawn speed.
- The game ends in **GAME OVER** when the player's HP reaches 0.

---

## Controls

| Key / Button | Action |
|---|---|
| `W` / `A` / `S` / `D` | Move |
| `Left Click (LMB)` | Burst Strike (after unlocking) |
| `Right Click (RMB)` | Place Poison Gas Zone (after unlocking) |
| `Q` | Open / Close Shop |
| `1` ~ `5` | Purchase shop item (while shop is open) |
| `E` | Reroll shop items (while shop is open) |
| `R` | Magnet — collect all coins on screen (60s cooldown) |
| `Tab` | Toggle camera between player and a random enemy |
| `←` / `→` Arrow Keys | Adjust game speed (0.1x ~ 5.0x) |
| `Space` | Reset game speed to 1.0x |
| `Enter` | Restart after game over |

> The game is **paused** while the shop is open.

---

## Player Stats

| Stat | Base Value |
|---|---|
| Max HP | 100 |
| Move Speed | 140 px/s |
| Invincibility on Hit | 0.8s |
| Coin Pickup Radius | 50 px |

- Move Speed upgrade: **+7%** per level
- Pickup Radius upgrade: **+15%** per level

---

## Stage Structure

### Stage 1
| Field | Value |
|---|---|
| Spawn Count | 40 ~ 54 |
| Enemies | Knight (100%) |
| Boss Limit | None |

### Stage 2
| Field | Value |
|---|---|
| Spawn Count | 66 ~ 88 |
| Enemies | Knight 45% / Wizard Female 30% / Wizard Male 25% |
| Boss Limit | None |

### Stage 3+ (Infinite Loop)
| Field | Value |
|---|---|
| Spawn Count | 105 ~ 140 |
| Enemies | Knight 22% / Wizard Female 15% / Wizard Male 15% / Dinosaur 20% / Boss Brute 2% / Golem 2% / Venom 2% |
| Max Simultaneous Bosses | 3 |

### Infinite Mode Scaling (per cycle)
| Field | Scale |
|---|---|
| Enemy HP | +22 ~ 36% per cycle |
| Spawn Count | +16 ~ 26% per cycle |
| Spawn Speed | +8 ~ 20% faster per cycle |

---

## Enemies

### Normal Enemies

| Name | HP | Damage | Speed | Attack Range | Attack Cooldown | Dash Multiplier |
|---|---|---|---|---|---|---|
| Knight | 9 | 8 | 68 | 30 | 2.4s | 3x |
| Wizard Female | 8 | 7 | 62 | 32 | 2.4s | 3x |
| Wizard Male | 8 | 7 | 64 | 32 | 2.4s | 3x |
| Dinosaur | 14 | 10 | 76 | 34 | 2.6s | 3x |

### Bosses

| Name | HP | Damage | Speed | Attack Range | Attack Cooldown | Coin Drop on Death |
|---|---|---|---|---|---|---|
| Boss Brute | 120 | 12 | 52 | 42 | 3.0s | 18 ~ 42 |
| Golem | 150 | 14 | 48 | 44 | 3.2s | 18 ~ 42 |
| Venom | 135 | 13 | 64 | 42 | 2.8s | 18 ~ 42 |

- Bosses are **2× the size** of normal enemies.
- Attack pattern: approach player → **charge (1 ~ 1.2s)** → **dash (0.3 ~ 0.4s at 3× speed)**

---

## Weapons

Weapons are unlocked and upgraded in the shop using coins.

### Energy Ball (Default)

Automatically fires projectiles at the nearest enemy.

| Stat | Base Value |
|---|---|
| Damage | 1.7 |
| Fire Rate | 0.58s cooldown |
| Range | 220 px |
| Projectile Speed | 300 px/s |
| Pierce | 1 |
| Knockback | 150 |
| Max Count | 2 |

**Upgrades:**
- Haste: fire rate −8% cooldown (min 0.42s)
- Power: damage +0.45
- Range Up: range +12%
- Pierce Up: pierce +1
- Knockback Up: knockback +15%

---

### Orbit Blade (Shop unlock — cost 14)

Rotating blades orbit the player, dealing continuous damage on contact.

| Stat | Base Value |
|---|---|
| Damage | 1.7 |
| Orbit Radius | 32 px |
| Orbit Speed | 210°/s |
| Hit Cooldown | 0.48s |
| Knockback | 120 |
| Max Count | 4 |

**Upgrades:**
- Speed Up: orbit speed +8%
- Power: damage +0.35

---

### Homing Missile (Shop unlock — cost 18)

Fires a self-guided missile that tracks the nearest enemy.

| Stat | Base Value |
|---|---|
| Damage | 1.35 |
| Fire Rate | 1.35s cooldown |
| Range | 320 px |
| Projectile Speed | 170 px/s |
| Turn Rate | 4.2 rad/s |
| Pierce | 1 |
| Knockback | 55 |
| Max Count | 2 |

**Upgrades:**
- Power: damage +0.3
- Speed Up: speed / turn rate / range +8%, fire rate reduced (min 0.55× base)

---

## Skills

Skills are unlocked in the shop and activated manually with mouse buttons.  
Charges regenerate on cooldown and can be stored up to **3 stacks**.

### Burst Strike (`LMB` — Shop unlock, cost 12)

Triggers a shockwave at the mouse cursor position.

| Stat | Base Value |
|---|---|
| Damage | 3.0 |
| Area | 126 × 56 px |
| Hit Delay | 0.2s |
| Duration | 0.45s |
| Knockback | 320 |
| Cooldown | 3.2s (min 0.4s) |
| Max Stacks | 3 |

**Upgrades:**
- Damage Up: damage +25%
- Delay Down: hit delay −0.02s / cooldown −0.18s
- Area Up: area +12%
- Keeping Count +1: max stacks +1

---

### Poison Gas (`RMB` — Shop unlock, cost 12)

Places a damage-over-time zone at the mouse cursor position.

| Stat | Base Value |
|---|---|
| Tick Damage | 1.0 |
| Area | 128 × 68 px |
| Duration | 3.0s |
| Tick Interval | 0.35s |
| Knockback | 30 |
| Cooldown | 5.0s (min 0.6s) |
| Max Stacks | 3 |

**Upgrades:**
- Damage Up: tick damage +20%
- Duration Up: duration +15%
- Tick Rate Up: tick interval −0.03s / cooldown −0.12s
- Area Up: area +12%
- Keeping Count +1: max stacks +1

---

## Shop

- Press `Q` to open the shop — the game **pauses**.
- Each time the shop opens, **5 random items** are offered.
- Press `1`~`5` to purchase the corresponding item.
- Press `E` to **reroll** items (first reroll costs 4 coins; each subsequent reroll costs 2 more).

### Stat Upgrades

| Item | Cost | Effect |
|---|---|---|
| Move Speed Up | 8 | Move speed +7% |
| Pickup Radius Up | 6 | Coin pickup radius +15% |
| Small Heal | 8 | Recover 25 HP |
| Large Heal | 15 | Recover 50 HP |

---

## Coins & Rewards

- Normal enemy kill: drops **1 coin**
- Boss kill: drops **18 ~ 42 coins** in an explosion (scales with boss HP)
- `R` key: instantly attract all coins on screen (cooldown **60s**)
- Coins within the pickup radius (default 50 px) are collected automatically.

---

## HUD

| Location | Content |
|---|---|
| Top Center | Player HP bar and numeric value |
| Top Left | WASD hint, key hints, coin count, game speed (xSpeed) |
| Top Right | Current stage name, spawn progress (Spawn X/Y) |
| Below Top Right | Current enemy count |
| Bottom Left | LMB Burst / RMB Gas skill state and cooldown |
| Center Screen | Stage start banner (displayed for 2.2s) |
