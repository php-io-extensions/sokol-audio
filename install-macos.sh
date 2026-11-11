#!/bin/bash

# Builds ext-sokol-audio in a disposable copy and installs it into each PHP given.
#
#   ./install-macos.sh                      # Homebrew php@8.4 (NTS) and php@8.4-zts
#   ./install-macos.sh /path/to/bin/php ... # specific PHP binaries
#
# For each PHP: phpize/configure/make against that PHP's php-config, copy the
# .so into its extension_dir, re-sign it ad hoc so amfid accepts it on first
# load, and enable it with 30-sokol_audio.ini in its conf.d scan dir.
#
# The copy is the build. A noexec mount never hosts phpize.

set -Eeuo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCES=(config.m4 php_sokol_audio.h src stubs lib)
INI_NAME="30-sokol_audio.ini"

die() { printf '✖ %s\n' "$*" >&2; exit 1; }
warn() { printf '⚠ %s\n' "$*" >&2; }
step() { printf '▶ %s\n' "$*"; }

# A startup warning from a half-installed copy is printed before the script's echo. The value is the last line.
php_last() {
    "$1" -r "$2" | tail -1
}

[[ "$(uname -s)" == "Darwin" ]] || die "This installer targets macOS; on Debian or Raspberry Pi OS use install-debian-trixie.sh."

if [[ $# -gt 0 ]]; then
    PHP_BINS=("$@")
else
    PHP_BINS=(/opt/homebrew/opt/php@8.4/bin/php /opt/homebrew/opt/php@8.4-zts/bin/php)
fi

BUILD_DIR=""
cleanup() { if [[ -n "$BUILD_DIR" ]]; then rm -rf "$BUILD_DIR"; fi; }
trap cleanup EXIT

for PHP_BIN in "${PHP_BINS[@]}"; do
    BIN_DIR="$(dirname "$PHP_BIN")"
    PHPIZE="${BIN_DIR}/phpize"
    PHP_CONFIG="${BIN_DIR}/php-config"
    for tool in "$PHP_BIN" "$PHPIZE" "$PHP_CONFIG"; do
        [[ -x "$tool" ]] || die "$tool not found or not executable"
    done

    step "Building for $(php_last "$PHP_BIN" 'echo PHP_VERSION, PHP_ZTS ? " ZTS" : " NTS";') (${PHP_BIN})"
    BUILD_DIR="$(mktemp -d "${TMPDIR:-/tmp}/sokol-audio-build.XXXXXX")"
    export COPYFILE_DISABLE=1
    for f in "${SOURCES[@]}"; do
        cp -R "${SCRIPT_DIR}/${f}" "${BUILD_DIR}/"
    done
    if ! (cd "$BUILD_DIR" \
        && "$PHPIZE" \
        && ./configure --enable-sokol-audio --with-php-config="$PHP_CONFIG" \
        && make -j"$(sysctl -n hw.ncpu)") >"${BUILD_DIR}/build.log" 2>&1; then
        tail -40 "${BUILD_DIR}/build.log" >&2
        die "Build failed for ${PHP_BIN}"
    fi

    EXT_DIR="$(php_last "$PHP_BIN" 'echo ini_get("extension_dir");')"
    SCAN_DIR="$(php_last "$PHP_BIN" 'echo PHP_CONFIG_FILE_SCAN_DIR;')"
    [[ -d "$EXT_DIR" ]] || die "extension_dir ${EXT_DIR} does not exist"
    [[ -n "$SCAN_DIR" && -d "$SCAN_DIR" ]] || die "conf.d scan dir '${SCAN_DIR}' does not exist"

    install -m 0755 "${BUILD_DIR}/modules/sokol_audio.so" "${EXT_DIR}/sokol_audio.so"
    codesign --force --sign - "${EXT_DIR}/sokol_audio.so" >/dev/null 2>&1
    # An ini that already loads sokol_audio (by name or by path) stays: a second one would load it twice.
    if grep -lqE '^[[:space:]]*extension[[:space:]]*=[[:space:]]*"?([^"]*/)?sokol_audio(\.so)?"?[[:space:]]*$' "${SCAN_DIR}"/*.ini 2>/dev/null; then
        warn "an ini in ${SCAN_DIR} already loads sokol_audio; kept"
    else
        printf 'extension=sokol_audio\n' > "${SCAN_DIR}/${INI_NAME}"
    fi

    "$PHP_BIN" -r 'exit(extension_loaded("sokol_audio") ? 0 : 1);' || die "sokol_audio did not load in ${PHP_BIN}"
    step "Installed ${EXT_DIR}/sokol_audio.so, enabled by ${SCAN_DIR}/${INI_NAME}"

    rm -rf "$BUILD_DIR"
    BUILD_DIR=""
done
