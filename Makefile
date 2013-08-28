install:
	install -d ${HOME}/bin
	install -m 0555 -C ./bin/flac2alac ${HOME}/bin/flac2alac

install-homebrew:
	brew install ffmpeg
	brew install flac
	brew install gnu-sed
	brew install imagemagick
	brew install mp4v2
