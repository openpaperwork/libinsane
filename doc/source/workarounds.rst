Workarounds
===========

Option 'scan-resolution' --> 'resolution'
-----------------------------------------

.. note::

    * API: Sane
    * Culprit: Lexmark
    * Seen on: Lexmark MFP

The option 'resolution' is mistakenly named 'scan-resolution'.
This workaround creates a mirror option 'resolution'.


Option 'doc-source' --> 'source'
-----------------------------------------

.. note::

    * API: Sane
    * Culprit: Samsung
    * Seen on: `Samsung CLX-3300 <https://openpaper.work/en/scanner_db/report/31/>`_

The option 'resolution' is mistakenly named 'scan-resolution'.
This workaround creates a mirror option 'resolution'.


Option 'mode': Unusual mode values
----------------------------------

.. note::

    * API: Sane
    * Culprits: Brother, Samsung
    * Seen on: `Brother MFC-7360N <https://openpaper.work/en/scanner_db/report/20/>`_
    * Seen on: `Samsung CLX-3300 <https://openpaper.work/en/scanner_db/report/31/>`_

Override the option 'mode' so it changes the following possible values:

* Brother
    * '24bit Color' --> 'Color'
    * 'Black & White' --> 'Lineart'
    * 'True Gray' --> 'Gray'
* Samsung
    * 'Black and White - Line Art' --> 'Lineart'
    * 'Grayscale - 256 Levels' --> 'Gray'
    * 'Color - 16 Million Colors' --> 'Color'


Extra options 'page-height' and 'page-width'
--------------------------------------------

.. note::

    * API: Sane
    * Culprit: Fujitsu
    * Seen on: `Fujitsu ScanSnap S1500 <https://github.com/openpaperwork/paperwork/issues/230#issuecomment-22792362>`_

This workaround wraps the option `br_x`, `tl_x`, `br_y` and `tl_y` in order to
set `page-height` and `page-width` accordingly.

.. todo::

    Need to figure out how to set them exactly.
    Currently: set proportionally to (br_x - tl_x, br_y - tl_y).


Strip option translations
-------------------------

.. note::

    * API: Sane
    * Culprits: Sane project, OKI
    * Seen on: `OKI MC363 <https://openpaper.work/en/scanner_db/report/56/>`_

This workaround wraps a bunch of options, and try to revert the translations back to English.


Device model name may contain manufacturer name
-----------------------------------------------

.. note::

    * API: Sane, WIA
    * Culprits: too many. Especially HP.

If the model name contains also the manufacturer name, this workaround strips it.

Random example:

    * Manufacturer: Brother
    * Model: Brother MFC-7360N

Will become:

    * Manufacturer: Brother
    * Model: MFC-7360N

Special case: HP. Manufacturer is "hewlett-packard", but
`model names contain the prefix "hp" <https://openpaper.work/en/scanner_db/vendor/7/>`_.


Device model name may contain '_' instead of spaces
---------------------------------------------------

.. note::

    * API: Sane
    * Culprit: HP
    * Seen on: `all HP devices <https://openpaper.work/en/scanner_db/vendor/7/>`_
