BUILD=Debug
BUILD_DIR=build-$(BUILD)
INSTALL_DIR=$(BUILD_DIR)

.PHONY : 3rd-party
3rd-party : cmocka

.PHONY : cmocka
cmocka : FORCE
	mkdir -p $(BUILD_DIR)/$@;
	cd $(BUILD_DIR)/$@; cmake \
		-DCMAKE_INSTALL_PREFIX=.. \
		-DCMAKE_BUILD_TYPE=$(BUILD) \
		-DCMAKE_CC_COMPILER=$(CC) \
		-DWITH_STATIC_LIB=ON \
		../../cmocka; make install

.PHONY : clean
clean : FORCE
	@-$(RM) -rf *~

.PHONY : clobber
clobber : clean
	@-$(RM) -rf $(BUILD_DIR)

FORCE :
