
# Copyright 2017 Benjamin J. Andre.
# All Rights Reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

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

