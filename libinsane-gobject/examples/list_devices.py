#!/usr/bin/env python3

# make
# make install
# export GI_TYPELIB_PATH=/usr/local/lib/girepository-1.0
# export LD_LIBRARY_PATH=/usr/local/lib
# libinsane-gobject/examples/list_devices.py

import gi
gi.require_version('Libinsane', '0.1')
from gi.repository import GObject  # noqa: E402
from gi.repository import Libinsane  # noqa: E402


class ExampleLogger(GObject.Object, Libinsane.Logger):
    def __init__(self):
        super().__init__()

    def log(self, *args, **kwargs):
        print("%s: %s" % (str(args), str(kwargs)))


GObject.type_register(ExampleLogger)


def main():
    Libinsane.register_logger(ExampleLogger())
    api = Libinsane.Api.new_safebet()
    devs = api.get_devices(False)  # !local_only
    for dev in devs:
        print(dev.to_string())


if __name__ == "__main__":
    main()
