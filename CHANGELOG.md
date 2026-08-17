# Changelog

This file follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) and
[Semantic Versioning](https://semver.org/spec/v2.0.0.html). Release tags must
match a section heading exactly (for example `v1.0.0`).

When you cut a release:

1. Move items from `[Unreleased]` into a new `## [vX.Y.Z] - YYYY-MM-DD` section.
2. Commit the changelog update.
3. Push the tag: `git tag vX.Y.Z && git push origin vX.Y.Z`

CI reads the matching section and uses it as the GitHub Release notes. The tag
is also used in staged asset names (`<binary>-<tag>.bin`, `<binary>-<tag>.zip`).

## [Unreleased]

### Added

- (your changes here)

### Changed

- (your changes here)

### Fixed

- (your changes here)

## [v1.0.0] - 2026-08-17

Initial Snake GWHB homebrew for Retro-Go SD.

### Added

- Port of [slipperstree/game-and-watch-snake](https://github.com/slipperstree/game-and-watch-snake)
  as a freestanding GWHB (`Snake.bin` → `/roms/homebrew/`)
- Hi-score / sound preference persistence via `odroid_settings`
- Short beeps for eat / death / game over through the Retro-Go audio path
