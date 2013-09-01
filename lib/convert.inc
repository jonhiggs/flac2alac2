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
  if [ $# -ne 1 ]; then
    __log_err "You must parse in a flac file."
    return 1
  fi

  if [ ! -f $1 ]; then
    __log_err "Input file does not exist."
    return 1
  fi

  tmp_alac="`__mktemp`.m4a"
  ffmpeg -v -1 -i "$1" -acodec alac "${tmp_alac}"

  if [ -s ${tmp_alac} ]; then
    echo ${tmp_alac}
    return 0
  else
    return 1
  fi
}

function conversion_stats() {
  if [ $# -ne 2 ]; then
    echo "You must provide both an input and an output file."
    return 1
  fi

  in_file=$1
  out_file=$2

  in_size=`stat -f "%z" "$in_file"`
  out_size=`stat -f "%z" "$out_file"`
  PCT=$(( $in_size * 100 / $out_size ))
  echo "ALAC is ${PCT}% the size of the input FLAC"
}
