function __mktemp() {
  file=$(mktemp -t flac2alac)
  rm $file
  echo $file
}

function __rmtemp() {
  [ -z $TMPDIR ] && TMPDIR="/tmp"
  find $TMPDIR -name flac2alac.\* -delete
}

function __die() {
  echo "ERROR: $1" > /dev/stderr
  __rmtemp
  rm -f "${ARTFILE}"
  exit 1
}

function __move_on() {
  echo "INFO: $1" > /dev/stderr
  __rmtemp
  rm -f "${ARTFILE}"
}

function __log_err() {
  echo "$1" > /dev/stderr
}

function __has_afconvert() {
  if [ -z $(which afconvert) ]; then
    __log_err "flac_to_wav requires afconvert"
    return 1
  fi
}

function __has_flac() {
  if [ -z $(which flac) ]; then
    __log_err "flac_to_wav requires flac"
    return 1
  fi
}

function __has_ffmpeg() {
  if [ -z $(which ffmpeg) ]; then
    __log_err "flac_to_alac requires ffmpeg"
    return 1
  fi
}
