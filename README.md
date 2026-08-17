# Snake — Retro-Go SD Homebrew

GWHB port of [slipperstree/game-and-watch-snake](https://github.com/slipperstree/game-and-watch-snake)
for [Game & Watch Retro-Go SD](https://github.com/sylverb/game-and-watch-retro-go-sd).

Drop `Snake.bin` on the SD card under `/roms/homebrew/`.

## Build

```bash
make                    # PROJECT_KIND=homebrew is the default
# or without a local ARM toolchain:
make docker
```

Produces `Snake.bin` (optional cover override: `/covers/homebrew/Snake.img`).

Host SDL preview (same `src/main.c`, no G&W flash cycle):

```bash
make host                       # SDL2 → ./snake_host
make host HOST_SDL=3            # SDL3
./snake_host                    # Esc / close window to quit
```

On macOS, if `pkg-config sdl2` fails:

```bash
export PKG_CONFIG_PATH="$(brew --prefix)/lib/pkgconfig:${PKG_CONFIG_PATH:-}"
```

Controls: arrows = D-pad, `Z`/`X` = B/A, Enter = Start, Shift = Select. Scale with `HOST_SCALE=2` (default).

## Controls

| Screen | Controls |
|--------|----------|
| Home | Any button — start · wait — demo · **GAME** / **TIME** — switch mode |
| In game | **D-pad** — steer · **B** — sound on/off · **GAME** — mode helpers |
| Game over | Any button — home · **TIME / SELECT / Y** — info |

Pause / power stay with Retro-Go (system menu / sleep). Hi-score is saved via app settings.

## Credits

- Game logic & UI: MANDA ([slipperstree](https://github.com/slipperstree/game-and-watch-snake)), BSD-3-Clause
- Base hardware bring-up in upstream: [game-and-watch-base](https://github.com/ghidraninja/game-and-watch-base)
- This tree: Retro-Go SD SDK template (MIT) + homebrew glue

## License

- Homebrew glue / SDK template: MIT (see `LICENSE`)
- Ported `src/snake/` sources retain upstream BSD-3-Clause (see `src/snake/COPYING`)
- Vendored files under `sdk/include/` keep their upstream licenses
