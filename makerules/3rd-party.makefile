#
# build third-party libraries
#
ifdef THIRD_PARTY_DIR
.PHONY : $(THIRD_PARTY_DIR)
$(THIRD_PARTY_DIR) : 
	@if [ -d $@ ]; then \
		$(MAKE) --no-print-directory --directory=$@ \
			CC=$(CC) CFLAGS="$(CFLAGS)" $(MAKECMDGOALS); \
	fi
	$(BUILD_COMPLETE)
endif

cmocka : $(THIRD_PARTY_DIR)

