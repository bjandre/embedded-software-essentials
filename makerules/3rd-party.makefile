#
# build third-party libraries
#
THIRD_PARTY_DIR = 3rd-party

ifdef THIRD_PARTY_DIR
.PHONY : $(THIRD_PARTY_DIR)
$(THIRD_PARTY_DIR) : 
	@if [ -d $@ ]; then \
		$(MAKE) --no-print-directory --directory=$@ \
			CC=$(CC) CFLAGS="$(CFLAGS)" $(MAKECMDGOALS); \
	fi
	$(BUILD_COMPLETE)
endif


CMOCKA_INCLUDE_DIR = $(THIRD_PARTY_DIR)/build-Debug/include
CMOCKA_LIBRARY = $(THIRD_PARTY_DIR)/build-Debug/lib/libcmocka.a

cmocka : $(THIRD_PARTY_DIR)

CFLAGS += -I $(CMOCKA_INCLUDE_DIR)
