# Changelog

## [v0.0.1]

### Added

- Port of [slipperstree/game-and-watch-snake](https://github.com/slipperstree/game-and-watch-snake)
  as a freestanding GWHB (`Snake.bin` → `/homebrews/`)
- Hi-score / sound preference persistence via `odroid_settings`
- Short beeps for eat / death / game over through the Retro-Go audio path

### Install

**Homebrew**

- Unzip the release archive onto the SD card root. It already contains
  `/homebrews/Snake.bin`.
- Optional coverflow override: `/covers/homebrew/Snake.img` (JPEG ≤186×100,
  ≤10 KiB).
