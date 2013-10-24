function get_tag_from_flac() {
  file=$1
  tag=$2

  if [ ! -s $file ]; then
    __log_err "You must provide a valid flac file"
  fi

  case $tag in
    "art")
      image_file="`get_image_from_flac $file`"
      if [ ! -z "$image_file" ]; then return 1; fi
      echo $image_file
      return 0
    ;;
    "art_pieces")
      metaflac --list "$file" | egrep -c "\s+type:\ \d\ \(PICTURE\)"
      return 0
    ;;
    "artist")
      tag="ARTIST"
    ;;
    "album_artist")
      tag="ALBUMARTIST"
    ;;
    "track_number")
      tag="TRACKNUMBER"
    ;;
    "track_total")
      tag="TRACKTOTAL"
    ;;
    "disc_number")
      tag="DISCNUMBER"
    ;;
    "disc_total")
      tag="DISCTOTAL"
    ;;
  esac

  data=`metaflac --show-tag="${tag}" "$file"`
  if [ -z "$data" ]; then
    return 1
  else
    echo $data | cut -d= -f2-
    return 0
  fi
}

function add_tag_to_alac() {
  # TODO: don't split the tag and data
  file=$1
  tag=$2
  data=$3

  if [ ! -s $file ]; then
    file $file
    __log_err "$file is not a valid alac file."
    return 1
  fi

  if [ $# -ne 3 ]; then
    __log_err "You must provide the file, tag, and data you want to set"
    return 1
  fi

  case $tag in
    "album")
      switch="-A"
    ;;
    "album_artist")
      switch="-R"
    ;;
    "art")
      [ -s $data ] || __log_error "Data must contain an image file"
      switch="-P"
    ;;
    "artist")
      switch="-a"
    ;;
    "comment")
      switch="-c"
    ;;
    "composer")
      switch="-w"
    ;;
    "date")
      switch="-y"
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
      return 1
    ;;
  esac

  mp4tags $switch "$data" "$file" &> /tmp/out
}

function add_tag_to_flac(){
  file=$1
  tag=$2
  value=$3

  # TODO: check better if file is valid
  if [ ! -s $file ]; then
    file $file
    __log_err "$file is not a valid flac file."
    return 1
  fi

  if [ $# -ne 3 ]; then
    __log_err "You must provide a file, tag and value."
    return 1
  fi

  case $tag in
    "artist")
      name="ARTIST"
    ;;
    "album")
      name="ALBUM"
    ;;
    "album_artist")
      name="ALBUMARTIST"
    ;;
    "art")
      metaflac "$file" --import-picture-from="3||||$value" &> /tmp/out
      return $?
    ;;
    "comment")
      for name in DESCRIPTION COMMENT; do
        metaflac "$file" --remove-tag="$name"
        metaflac "$file" --set-tag="${name}=${value}"
      done
      return 0
    ;;
    #"composer")
    #  switch="-w"
    #;;
    "date")
      name="DATE"
    ;;
    #"disc_number")
    #  switch="-d"
    #;;
    #"disc_total")
    #  switch="-D"
    #;;
    "genre")
      name="GENRE"
    ;;
    "title")
      name="TITLE"
    ;;
    #"track_number")
    #  switch="-t"
    #;;
    #"track_total")
    #  switch="-T"
    #;;
    *)
      __log_error "Tag '$tag' not yet supported"
      return 1
    ;;
  esac

  metaflac "$file" --remove-tag="$name"
  metaflac "$file" --set-tag="${name}=${value}"
  return 0
}

function get_tag_from_alac() {
  file=$1
  tag=$2

  if [ ! -s $file ]; then
    __log_err "You must provide a valid alac file"
  fi

  case $tag in
    "art")
      if [ `get_tag_from_alac $file art_pieces` -ne 0 ]; then
        tag=`get_image_from_alac "$file"`
      fi   
    ;;
    "art_pieces")
      tag=`mp4info "$file" | grep "^\ Cover\ Art\ pieces:" | cut -d: -f2`
      if [ -z $tag ]; then tag=0; fi
    ;;
    "artist")
      tag=`mp4info "$file" | grep "^\ Artist:" | cut -d: -f2- | sed 's/^ //g'`
    ;;
    "album_artist")
      tag=`mp4info "$file" | grep "^\ Album\ Artist:" | cut -d: -f2- | sed 's/^ //g'`
    ;;
    "album")
      tag=`mp4info "$file" | grep "^\ Album:" | cut -d: -f2- | sed 's/^ //g'`
    ;;
    "comment")
      tag=`mp4info "$file" | grep "^\ Comments:" | cut -d: -f2- | sed 's/^ //g'`
    ;;
    "date")
      tag=`mp4info "$file" | grep "^\ Release\ Date:" | cut -d: -f2- | awk '{ print $1 }'`
    ;;
    "disc_number")
      tag=`mp4info "$file" | grep "^\ Disk:" | cut -d: -f2- | awk '{ print $1 }'`
    ;;
    "disc_total")
      tag=`mp4info "$file" | grep "^\ Disk:" | cut -d: -f2- | awk '{ print $3 }'`
    ;;
    "genre")
      tag=`mp4info "$file" | grep "^\ Genre:" | cut -d: -f2- | sed 's/^ //g'`
    ;;
    "title")
      tag=`mp4info "$file" | grep "^\ Name:" | cut -d: -f2- | sed 's/^ //g'`
    ;;
    *)
      __log_err "Tag '$tag' not yet supported"
      return 1
    ;;
  esac

  if [ -z "$tag" ]; then return 1; fi
  echo $tag
}

function get_image_from_flac() {
  file=$1
  tmp_img="`__mktemp`"

  if [ ! -s $file ]; then
    __log_err "You must provide a valid flac file"
  fi

  image_type=`metaflac --list "$file" | grep "MIME type" | awk '{ print $3 }'`

  case $image_type in
    "image/jpeg")
      metaflac --export-picture-to="${tmp_img}.jpg" "$file"
    ;;
    "image/png")
      metaflac --export-picture-to="${tmp_img}.png" "$file"
      convert "${tmp_img}.png" "${tmp_img}.jpg"
    ;;
    *)
      return 1
    ;;
  esac

  echo "${tmp_img}.jpg"
  return 0
}

function get_image_from_alac() {
  file=$1
  art_file=`mp4art -f --extract "$file" | sed 's/.*->\ //g'`
  if [ -f $art_file ]; then
    tmp_art=`__mktemp`
    mv "$art_file" "$tmp_art"
    echo $tmp_art
  else
    echo "Art failed to extract."
    return 1
  fi
}

# vim: set ft=sh :
