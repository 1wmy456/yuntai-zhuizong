# -*- coding: utf-8 -*-
import argparse, hashlib, json, zipfile
from pathlib import Path

root = Path(__file__).resolve().parents[1]
p = argparse.ArgumentParser()
p.add_argument('--profile', choices=['speed300'], default='speed300')
a = p.parse_args()
manifest = json.loads((root / 'release/manifest.json').read_text(encoding='utf-8'))
for name, expected in manifest['files'].items():
    path = root / name
    if not path.is_file() or hashlib.sha256(path.read_bytes()).hexdigest() != expected:
        raise SystemExit('RELEASE_CHECK FAIL: ' + name)
folder = root / 'release' / a.profile
assert (folder / 'system_wrapper.xsa').read_bytes() == (root / 'platform/system_wrapper.xsa').read_bytes()
for name in ['system_wrapper.bit', 'system_wrapper.xsa', 'ps7_init.tcl', 'ov5640_lcd.elf', 'source_build_manifest.json']:
    assert f'release/{a.profile}/{name}' in manifest['files'], name
build = json.loads((folder / 'source_build_manifest.json').read_text(encoding='utf-8'))
assert hashlib.sha256((folder / 'ov5640_lcd.elf').read_bytes()).hexdigest() == build['elf_sha256']
with zipfile.ZipFile(folder / 'system_wrapper.xsa') as xsa:
    for suffix, local in [('.bit', 'system_wrapper.bit'), ('ps7_init.tcl', 'ps7_init.tcl')]:
        names = [n for n in xsa.namelist() if n.endswith(suffix)]
        assert len(names) == 1 and xsa.read(names[0]) == (folder / local).read_bytes(), local
print('RELEASE_CHECK PASS:', a.profile)
