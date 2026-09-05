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

## [v1.0.1] - 2026-09-05

Release tooling only. Same game, same binary layout.

### Added

- A debug symbols archive (`Snake-<tag>-debug.zip`) holding the ELF and the
  linker map, published for the first time.

### Changed

- `build_dist.py` and `stage_release.py` are taken verbatim from the shared
  set, so they are now byte-identical across snake, mine-sweeper and smw. These
  scripts are meant to be vendored by any project adopting the
  [GWRG distribution spec](https://github.com/slash-proc/gwrg-dist-spec), and a
  script that has to be edited on the way in is a script that drifts. Every
  project-specific value they need, they ask the Makefile for, which is why the
  Makefile grows `print-TARGET_ELF` and `print-TARGET_MAP`.
- The mirror now restores the filename a manifest declares when GitHub has
  rewritten it in the release asset name — a space becomes a dot, so
  `Super Mario World.bin` uploads as `Super.Mario.World.bin`. Inert here, since
  `Snake.bin` has nothing GitHub objects to, but it comes with the shared
  script.
- Release assets: the tag-stamped copy of the binary (`Snake-<tag>.bin`) is
  gone, replaced by the debug archive. The loose `Snake.bin` the manifest names
  and the SD install zip are unchanged.

## [v1.0.0] - 2026-08-17

Initial Snake GWHB homebrew for Retro-Go SD.

### Added

- Port of [slipperstree/game-and-watch-snake](https://github.com/slipperstree/game-and-watch-snake)
  as a freestanding GWHB (`Snake.bin` → `/roms/homebrew/`)
- Hi-score / sound preference persistence via `odroid_settings`
- Short beeps for eat / death / game over through the Retro-Go audio path
