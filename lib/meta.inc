function get_tag_from_flac() {
  file=$1
  tag=$2

  if [ ! -s $file ]; then
    __log_err "You must provide a valid flac file"
  fi

  case $tag in
    "album_artist")
      tag="albumartist"
    ;;
    "track_number")
      tag="tracknumber"
    ;;
    "track_total")
      tag="tracktotal"
    ;;
    "disc_number")
      tag="discnumber"
    ;;
    "disc_total")
      tag="disctotal"
    ;;
  esac

  data=`metaflac --show-tag=${tag} "$1"`
  if [ -z "$data" ]; then
    return 1
  else
    echo $data | cut -d= -f2-
    return 0
  fi
}

function add_tag_to_alac() {
  file=$1
  input=$2
  tag=`echo $input | cut -d= -f1`
  data=`echo $input | cut -d= -f2`

  if [ ! -s $file ]; then
    file $file
    __log_err "$file is not a valid alac file."
    return 1
  fi

  if [ $# -ne 2 ]; then
    __log_err "You must provide the tag you want to set"
    return 1
  fi

  case $tag in
    "album")
      switch="-A"
    ;;
    "album_artist")
      switch="-R"
    ;;
    "art_file")
      [ -s $data ] || __log_error "Data must contain an image file"
      switch="-P"
    ;;
    "artist")
      switch="-a"
    ;;
    "composer")
      switch="-w"
    ;;
    "date")
      switch="-y"
    ;;
    "description")
      switch="-c"
    ;;
    "disc_number")
      switch="-d"
    ;;
    "disc_total")
      switch="-D"
    ;;
    "genre")
      switch="-g"
    ;;
    "title")
      switch="-s"
    ;;
    "track_number")
      switch="-t"
    ;;
    "track_total")
      switch="-T"
    ;;
    *)
      __log_error "Tag '$tag' not yet supported"
    ;;
  esac

  mp4tags $switch "$data" "$file" &> /tmp/out
}

function get_tag_from_alac() {
  file=$1
  tag=$2

  if [ ! -s $file ]; then
    __log_err "You must provide a valid alac file"
  fi

  case $tag in
    "artist")
      mp4info "$file" | grep "^\ Artist:" | cut -d: -f2- | sed 's/^ //g'
    ;;
    "date")
      mp4info "$file" | grep "^\ Release Date:" | cut -d: -f2- | awk '{ print $1 }'
    ;;
    "disc_number")
      mp4info "$file" | grep "^\ Disk:" | cut -d: -f2- | awk '{ print $1 }'
    ;;
    "disc_total")
      mp4info "$file" | grep "^\ Disk:" | cut -d: -f2- | awk '{ print $3 }'
    ;;
    *)
      __log_err "Tag '$tag' not yet supported"
    ;;
  esac
}
