function flac_to_wav() {
  if [ $# -ne 1 ]; then
    __log_err "You must parse in a flac file"
    return 1
  fi

  tmp_wav="`__mktemp`.wav"
  flac -d -o "${tmp_wav}" "$1" &> /dev/null
  if [ -s ${tmp_wav} ]; then
    echo ${tmp_wav}
    return 0
  else
    return 1
  fi
}

function wav_to_alac() {
  if [ $# -ne 1 ]; then
    __log_err "You must parse in a wav file."
    return 1
  fi

  if [ ! -f $1 ]; then
    __log_err "Input file does not exist."
    return 1
  fi

  tmp_alac="`__mktemp`.m4a"
  afconvert -d alac "$1" "${tmp_alac}"
  if [ -s ${tmp_alac} ]; then
    echo ${tmp_alac}
    return 0
  else
    return 1
  fi
}

function flac_to_alac() {
  ffmpeg -v -1 -i "$1" -acodec alac "${TMP_MP4}"|| __die "There was a problem converting flac to alac."
}
