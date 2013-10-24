install:
	install -d ${HOME}/bin
	install -m 0755 -C ./bin/flac2alac ${HOME}/bin/flac2alac
	sudo apt-get install flac ffmpeg imagemagick mp4v2

install-homebrew:
	brew install ffmpeg
	brew install flac
	brew install gnu-sed
	brew install imagemagick
	echo "You need to install mp4v2 version 2.0.0 or later."
	echo "Homebrew currently doesn't have a recipe for doing this."
	echo "Take a look at https://code.google.com/p/mp4v2/downloads/list"

test:
	./tests/testSystem
	./tests/testConvert
	./tests/testMeta
