## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = 	liber_objects.la
liber_objects_la_SOURCES = \
			entity.c \
			relationship.c \
			attribute.c \
			participation.c \
			er.c

liber_objects_la_LDFLAGS = -export-dynamic -module -avoid-version
liber_objects_la_LIBADD = @GTK_LIBS@

INCLUDES = -I$(top_srcdir)/intl -I$(srcdir)/../../lib @GTK_CFLAGS@ @GNOME_CFLAGS@

EXTRA_DIST = \
	pixmaps/attribute.xpm \
	pixmaps/entity.xpm \
	pixmaps/participation.xpm \
	pixmaps/relationship.xpm
