# -*- coding: utf-8 -*-
import argparse, hashlib, json
from pathlib import Path

root = Path(__file__).resolve().parents[1]
p = argparse.ArgumentParser()
p.add_argument('--profile', choices=['speed150', 'speed100', 'lost'], default='speed150')
a = p.parse_args()
manifest = json.loads((root / 'release/manifest.json').read_text(encoding='utf-8'))
for name, expected in manifest['files'].items():
    path = root / name
    if not path.is_file() or hashlib.sha256(path.read_bytes()).hexdigest() != expected:
        raise SystemExit('RELEASE_CHECK FAIL: ' + name)
folder = root / 'release' / a.profile
assert (folder / 'system_wrapper.xsa').read_bytes() == (root / 'platform/system_wrapper.xsa').read_bytes() or a.profile == 'lost'
print('RELEASE_CHECK PASS:', a.profile)
