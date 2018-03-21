What is LibInsane ?
===================

LibInsane is a library allowing access to image scanners. It is the successor of
`Pyinsane`_.

* Cross-platform: Linux, \*BSD and Windows.
* Cross-API: `Sane`_ (Linux, \*BSD), `WIA2`_ (Windows), `TWAIN`_ (Windows).
* Cross-language: Thanks to `GObject introspection`_.
* Cross-scanner and cross-driver: Because a lot of them all have their quirks (and sometimes bugs).
* Cross-compiled: Windows version is built on a GNU/Linux system.

Let the insanity begin !

.. _Pyinsane: https://github.com/openpaperwork/pyinsane/
.. _Sane: http://sane-project.org/
.. _WIA2: https://msdn.microsoft.com/en-us/library/windows/desktop/ms630368(v=vs.85).aspx
.. _TWAIN: https://www.twain.org/
.. _GObject introspection: https://wiki.gnome.org/action/show/Projects/GObjectIntrospection?action=show&redirect=GObjectIntrospection


Overall view
============

.. image:: design.png
    :alt: Design of Libinsane


Most of the components in LibInsane provides an implementation of a normalized
C API. This API provides a generic way to access scanners and their options.

* Component for WIA support implements the normalized API.
* Component for Twain support implements the normalized API.
* Component for Sane support component implements the normalized API.
* Workaround components wraps the normalized API provided by others, and
  provide themselves an implementation of the normalized API.
* Behavior normalization components wraps too the normalized API provided
  by others, and provide themselves an implementation of the normalized API.
* The GObject Introspection support wraps the normalized API.

While this may sound like a lot of wrapping and layers, this approach allows
modularity. Any workaround or behavior normalization component can be enabled
or disabled easily and clear `separation of concerns`_ is ensured.

.. _separation of concerns: https://en.wikipedia.org/wiki/Separation_of_concerns
