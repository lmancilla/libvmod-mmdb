============
vmod_mmdb
============

----------------------
Varnish Module
----------------------

:Date: 2015-03-30
:Version: 1.0

SYNOPSIS
========

import mmdb;

DESCRIPTION
===========

Implements GeoIP as a vmod using MaxmindDB.

DEPENDENCIES
============

First, download the database

http://geolite.maxmind.com/download/geoip/database/GeoLite2-Country.mmdb.gz

Second,

apt-get install libmaxminddb-dev


FUNCTIONS
=========

get_code
-----

Prototype
        ::

                get_code(STRING IP_STR)
Return value
	STRING
Description
	Returns a two iso code chars for country
Example
        ::

                set resp.http.X-GeoIP = mmdb.get_code("x.x.x.x");

INSTALLATION
============

The source tree is based on autotools to configure the building, and
does also have the necessary bits in place to do functional unit tests
using the ``varnishtest`` tool.

Building requires the Varnish header files and uses pkg-config to find
the necessary paths.

Usage::

 ./autogen.sh
 ./configure

If you have installed Varnish to a non-standard directory, call
``autogen.sh`` and ``configure`` with ``PKG_CONFIG_PATH`` pointing to
the appropriate path. For mmdb, when varnishd configure was called
with ``--prefix=$PREFIX``, use

 PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig
 export PKG_CONFIG_PATH

Make targets:

* make - builds the vmod.
* make install - installs your vmod.
* make check - runs the unit tests in ``src/tests/*.vtc``
* make distcheck - run check and prepare a tarball of the vmod.

In your VCL you could then use this vmod along the following lines::

        import mmdb;

        sub vcl_init {
                mmdb.load_db("/var/lib/geoip/GeoLite2-Country.mmdb.gz");
        }

        sub vcl_deliver {
                set resp.http.X-GeoIP = mmdb.get_code("x.x.x.x");
        }

COMMON PROBLEMS
===============

* configure: error: Need varnish.m4 -- see README.rst

  Check if ``PKG_CONFIG_PATH`` has been set correctly before calling
  ``autogen.sh`` and ``configure``
