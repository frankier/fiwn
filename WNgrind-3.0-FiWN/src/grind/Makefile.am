if LEX_TEST
FLG=-l
else
FLG=
endif
bin_PROGRAMS = grind
include_HEADERS = wngrind.h wn.h
grind_SOURCES = create.c files.c globals.c grind.c makedb.c resolve.c util.c wnglobal.c wnparse.y wnlex.l
# FiWN: GLib includes for UTF-8 handling
grind_CFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include
grind_LFLAGS = $(FLG) $(AM_LFLAGS)
grind_YFLAGS = -dl $(AM_YFLAGS)
# FiWN: GLib library for UTF-8 handling
grind_LDFLAGS = -lglib-2.0
