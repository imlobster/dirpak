#!/usr/bin/env sh

for c in c++ clang++ g++; do
	command -v "$c" >/dev/null 2>&1 && { compiler="$c"; break; }
done

if [ -z "$compiler" ]; then
	printf "%s: no matching c++ compiler found!\n" "$0" >&2
	exit 1
fi

target="dirpak.cc"
output="dirpak"

PS4=''
{ set -x; } 2>/dev/null

"$compiler" "$target" "-o" "$output" "-O3" "-Wall" "-Wextra" "-Werror=return-type" "-fno-rtti" "-fno-exceptions"

{ set +x; } 2>/dev/null

exit
