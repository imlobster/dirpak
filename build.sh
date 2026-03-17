#!/usr/bin/env sh

if [ -z "$CXX" ]; then
	for c in clang++ c++ g++; do
		command -v "$c" >/dev/null 2>&1 && { CXX="$c"; break; }
	done
fi

if [ -z "$CXX" ]; then
	printf "%s: no matching c++ compiler found!\n" "$0" >&2
	exit 1
fi

target="dirpak.cc"
output="dirpak"

PS4=''
{ set -x; } 2>/dev/null

"$CXX" "$target" "-o" "$output" "-std=c++17" "-O3" "-Wall" "-Wextra" "-Werror=return-type" "-fno-rtti" "-fno-exceptions" "-static"

{ set +x; } 2>/dev/null

exit
