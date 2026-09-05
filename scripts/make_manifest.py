#!/usr/bin/env python3
"""Emit the dist manifest for one release.

Reads the packed GWHB binary and describes it per the GWRG distribution spec
(https://github.com/slash-proc/gwrg-dist-spec). Everything that can be derived
from the built artifact is derived: the firmware ABI requirement and the
display name come out of the packed header, never from a constant here.

Usage:
  python3 scripts/make_manifest.py --bin minesweeper.bin --tag v0.1.2 \
      --repo slash-proc/mine-sweeper-retro-go-sd --commit "$GITHUB_SHA" \
      --out manifest.json
"""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAKEFILE = ROOT / "Makefile"

SCHEMA_VERSION = 1

GWHB_MAGIC = b"GWHB"
# gwhb_meta_t, from sdk/include/Core/Inc/retro-go/gwhb.h. Exactly 96 bytes.
META_FORMAT = "<7I32s4B32s"
META_SIZE = struct.calcsize(META_FORMAT)

TARGET = {
    "id": "gnw-retro-go",
    "platform": "game-and-watch",
    "label": "Game & Watch (Retro-Go SD)",
}


def make_var(name: str) -> str:
    out = subprocess.check_output(
        ["make", "-f", str(MAKEFILE), "--no-print-directory", f"print-{name}"],
        cwd=ROOT,
        text=True,
    )
    return out.strip()


def read_gwhb(path: Path) -> dict:
    """Parse the GWHB envelope. Raises if this is not a packed homebrew."""
    data = path.read_bytes()
    if len(data) < 8 + META_SIZE:
        raise SystemExit(f"{path}: too short to be a GWHB binary")
    if data[:4] != GWHB_MAGIC:
        raise SystemExit(f"{path}: missing GWHB magic — is this the packed binary?")

    header_version, header_length = struct.unpack_from("<HH", data, 4)
    if header_length < META_SIZE:
        raise SystemExit(
            f"{path}: header_length {header_length} is smaller than gwhb_meta_t "
            f"({META_SIZE}) — legacy binaries carry no ABI requirement"
        )

    fields = struct.unpack_from(META_FORMAT, data, 8)
    (
        required_abi_version,
        required_abi_min_size,
        _flags,
        code_size,
        bss_size,
        _cover_offset,
        cover_size,
        display_name,
        ver_major,
        ver_minor,
        ver_patch,
        _reserved0,
        _reserved,
    ) = fields

    return {
        "header_version": header_version,
        "required_abi_version": required_abi_version,
        "required_abi_min_size": required_abi_min_size,
        "code_size": code_size,
        "bss_size": bss_size,
        "cover_size": cover_size,
        "display_name": display_name.split(b"\0", 1)[0].decode("utf-8", "replace"),
        "version": f"{ver_major}.{ver_minor}.{ver_patch}",
    }


def build_manifest(*, bin_path: Path, tag: str, repo: str, commit: str) -> dict:
    meta = read_gwhb(bin_path)
    payload = bin_path.read_bytes()

    project_kind = make_var("PROJECT_KIND")
    if project_kind != "homebrew":
        raise SystemExit(f"unsupported PROJECT_KIND for a dist manifest: {project_kind!r}")

    project = make_var("CORE_NAME")
    title = meta["display_name"] or project

    return {
        "schemaVersion": SCHEMA_VERSION,
        "project": project,
        "title": title,
        # Where a human reads about this project. Derived from the repo rather
        # than written down, so it cannot name a repository this is not.
        "docs": f"https://github.com/{repo}#readme",
        "source": {"repo": repo, "commit": commit, "ref": tag},
        # No proprietary assets to convert: the user supplies nothing. Stated
        # explicitly, because an absent key cannot be told from a truncated file.
        "tools": [],
        "targets": [
            {
                **TARGET,
                "kind": "homebrew",
                "requiresAbi": {
                    "version": meta["required_abi_version"],
                    "minSize": meta["required_abi_min_size"],
                },
                "artifacts": [
                    {
                        "filename": bin_path.name,
                        "bytes": len(payload),
                        "sha256": hashlib.sha256(payload).hexdigest(),
                        "url": bin_path.name,
                    }
                ],
            }
        ],
    }


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--bin", dest="bin_path", type=Path, required=True)
    ap.add_argument("--tag", required=True)
    ap.add_argument("--repo", required=True, help="owner/name")
    ap.add_argument("--commit", required=True)
    ap.add_argument("--out", type=Path, required=True)
    args = ap.parse_args()

    if not args.bin_path.is_file():
        raise SystemExit(f"packed binary not found: {args.bin_path}")

    manifest = build_manifest(
        bin_path=args.bin_path, tag=args.tag, repo=args.repo, commit=args.commit
    )
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")

    target = manifest["targets"][0]
    print(f"make_manifest: wrote {args.out}")
    print(f"  project={manifest['project']!r} title={manifest['title']!r}")
    print(
        f"  requiresAbi version={target['requiresAbi']['version']} "
        f"minSize={target['requiresAbi']['minSize']}"
    )
    print(f"  {target['artifacts'][0]['filename']} sha256={target['artifacts'][0]['sha256'][:16]}…")


if __name__ == "__main__":
    main()
