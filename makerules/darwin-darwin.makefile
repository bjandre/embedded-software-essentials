#
# configuration options for native macOS
#
CC = cc
CFLAGS = 
CPPFLAGS = -E
AR = ar
LD = ld
LDFLAGS = -macosx_version_min 10.11 -lSystem -map $*.map
