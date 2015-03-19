/* acconfig.h
   This file is in the public domain.

   Descriptive text for the C preprocessor macros that
   the distributed Autoconf macros can define.
   No software package will use all of them; autoheader copies the ones
   your configure.in uses into your configuration header file templates.

   The entries are in sort -df order: alphabetical, case insensitive,
   ignoring punctuation (such as underscores).  Although this order
   can split up related entries, it makes it easier to check whether
   a given entry is in the file.

   Leave the following blank line there!!  Autoheader needs it.  */


/* These get defined to the version numbers in configure.in */

#undef PACKAGE
#undef VERSION

/* Define if symbol tables on this system use leading underscores. */
#undef USCORE

/* Define if dlsym automatically supplies a leading underscore. */
#undef DLSYM_ADDS_USCORE

/* Define to enable GNOME-specific code. */
#undef GNOME

#undef GNOME_PRINT

/* Define if your locale.h file contains LC_MESSAGES.  */
#undef HAVE_LC_MESSAGES

/* Define if you have the stpcpy function.  */
#undef HAVE_STPCPY

/* Define to 1 if NLS is requested.  */
#undef ENABLE_NLS

/* Define as 1 if you have catgets and don't want to use GNU gettext.  */
#undef HAVE_CATGETS

/* Define if you have the popt library (-lpopt).  */
#undef HAVE_LIBPOPT

/* Define as 1 if you have gettext and don't want to use GNU gettext.  */
#undef HAVE_GETTEXT



/* Leave that blank line there!!  Autoheader needs it.
   If you're adding to this file, keep in mind:
   The entries are in sort -df order: alphabetical, case insensitive,
   ignoring punctuation (such as underscores).  */
