Scanner APIs
============

All scanner APIs provides similar features:

* Read/write access to scanner/sources options
* Access to scanner/sources options constraint (valid resolution range, valid
  modes, etc)
* Obtaining images

Some APIs provide extra features but it is simply not humanely possible
to support them all in this library.


Sane
----

`Sane API`_ is a simple and flexible API.

Each scanner is always a single scan source.
For each scanner, there are a list of options that can be set.
Each option has its constraints. Option names are not hard-coded.
There are however some `well-known options`_ that are found on most
scanners (.. but not all).

All features of Sane are supported by LibInsane.

.. _Sane API: http://sane-project.org/html/doc009.html
.. _well-known options: http://sane.alioth.debian.org/sane2/0.08/doc014.html


TWAIN
-----

.. todo::

    TODO

WIA2
----

`Windows Image Acquisition`_ is a Microsoft API.

For each scanner, there are many scan sources.
The scanner itself and each of its scan sources have their own options.

Possible options are limited to what is defined in `wia.h`. However
value types and constraint types are not.

There are actually 2 implementations for the WIA API:

* The first one uses the low-level WIA API. It is able to pass it a stream
  object to get the image, and therefore is able to get the image as the
  scan goes.
* The second one uses the WIA automation layer. It can only store the
  scan directly in a file. Therefore displayed the scan as it goes is
  actually a hack reading the file as it is being written.
  This implementation is only used as a fallback.

WIA2 provide also features that LibInsane doesn't support:

* Access to devices other than scanners (camera, etc)
* Access to file storages on devices
* Dialogs (Why in hell are there dialogs in a scan API in a first place !?)

.. _Windows Image Acquisition: https://msdn.microsoft.com/en-us/library/windows/desktop/ms630368(v=vs.85).aspx
