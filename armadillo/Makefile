all: armadillo

VERSION = 1.1.0
TARBALL = build/armadillo-$(VERSION).tar.gz
TARBALL_URL = http://downloads.sourceforge.net/arma/armadillo-$(VERSION).tar.gz
SOURCE_DIR = build/armadillo-$(VERSION)
include $(shell rospack find mk)/download_unpack_build.mk

armadillo: $(SOURCE_DIR)/unpacked
	cd $(SOURCE_DIR) && cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=$(CURDIR)/armadillo .
	cd $(SOURCE_DIR) && make install
	touch armadillo

clean:
	rm -rf armadillo $(SOURCE_DIR)

wipe: clean
	rm -rf build
