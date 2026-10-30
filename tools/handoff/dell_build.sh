#!/usr/bin/env bash
# Run on Dell, in an isolated source snapshot. Shared lock; no attached devices.
set -euo pipefail
project=$(pwd)
base=/srv/nvme-fast-2/toolchains/cad-runtime/rootfs
toolchain=/srv/nvme-fast-2/toolchains/pm12-avr-7.3/usr
exec flock /srv/nvme-fast-1/vivado-work/.vivado-build.lock podman run --rm --init --network=none --cpus=4 --memory=16g --memory-swap=16g --workdir /work \
 --mount "type=bind,src=$base/usr,dst=/usr,ro=true" \
 --mount "type=bind,src=$base/etc,dst=/etc,ro=true" \
 --mount "type=bind,src=$base/var,dst=/var,ro=true" \
 --mount "type=bind,src=$toolchain,dst=/opt/avr,ro=true" \
 --mount "type=bind,src=$project,dst=/work" \
 --env PATH=/opt/avr/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin \
 localhost/dell-cad-base:20261005 bash tools/handoff/build_offline.sh
