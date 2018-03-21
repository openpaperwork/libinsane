Behavior normalizations
=======================

.. todo::

    TWAIN


Ensuring there is at least one source
-------------------------------------

.. note::

    Culprits: Microsoft, Sane project


Sane: `Some scanner do not provide the option "source"`_.

WIA2: `Some scanners do not always provide a source`_. Scan is done directly on them.

.. _Some scanner do not provide the option "source": https://openpaper.work/en-us/scanner_db/report/57
.. _Some scanners do not always provide a source: https://openpaper.work/en/scanner_db/report/28


Ensuring that sources are represented as node
---------------------------------------------

WIA2: For each scanner, provide a device tree (see :ref:`WIA2` description).

Sane: There is no tree (see :ref:`Sane` description). Children nodes (sources) must be simulated.


Ensuring all options are available on sources
---------------------------------------------

.. note::

    Culprits: Microsoft, Sane project


Goal: Makes sure the application can find easily options by making
the scanner options available on all its sources.

Sane: Only the scanner itself has options. Options must all be mapped
too on all its sources.

WIA: Some options are on the scanner itself, some options are on the
sources. Scanner options must be mapped on all its sources.


Ensuring scan area option names are all the same
------------------------------------------------

Sane: `Sane scan area options`_ are used as reference.

WIA2: Fake options are added to simulate Sane options. They act like Sane
options, and any change to these options is applied back to the WIA2 options.

.. _Sane scan area options: http://sane.alioth.debian.org/sane2/0.08/doc014.html#s4.5.4


Ensuring resolution constraint is always in the same format
-----------------------------------------------------------

.. note::

    Culprits: Microsoft, Sane project


Sane and WIA: Resolution constraint can be expressed as a range or as a list
of possible values. This normalization makes sure they are always expressed as
a list. If the range has an interval < 25dpi, the interval used to generate the
list will be 25dpi.


Identifying source types
------------------------

.. note::

    Culprit: Sane project


Sane: Sources have "name", but the exact names are up to the drivers.

WIA2: source types are already clearly defined.


Ensuring there is only supported devices
----------------------------------------

.. note::

    Culprits: Sane project, Microsoft


Sane: Remove v4l devices (video for linux ; Camera).

WIA: Video devices are directly stripped by the WIA support module of LibInsane.


Ensure the output format is RAW24
---------------------------------

.. note::

    Culprit: Microsoft.


Always getting the image as RAW24 is much more handy if you want to
display the scan on-the-fly.

Sane: Image is always outputed as RAW24 (unless some scanner-specific
options are set to non-default values).

WIA2: Drivers may return the image in a variety of file formats: RAW, BMP,
JPEG, PNG, etc. Not all drivers support returning the image as RAW24.
LibInsane supports only BMP (luckily, all WIA drivers appear to support
this one) and will output the image as RAW24.


Ensure the behavior is always the same whenever we scan from a flatbed or a feeder
----------------------------------------------------------------------------------

.. note::

    Culprits: Sane project, Microsoft

On both Sane and WIA:

* When scanning from feeder: at the end of a scan, we get an error code
  (or a function call) telling us when the whole page has been scanned.
  When requesting another scan, it will scan the next page. It no next
  page is available, another error code (or function call) will tell
  us we have reached the end.
* When scanning from flatbed: at the end of a scan, we get an error code
  (or a function call) telling us when the whole page has been scanned.
  Requesting another scan, it will scan again the very same page. It will
  never tell us that there is no paper left to scan

Problem: Behavior should be independent of the actual source. Having 2 behaviors
is just bug-prone.

Wanted behavior:

* When scanning from feeder: unchanged
* When scanning from flatbed: the first scan is unchanged. When requesting
  a second scan, it must return an error code indicating that there is
  no paper left to scan.
