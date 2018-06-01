#!/usr/bin/env python3

# make
# sudo make install
# export GI_TYPELIB_PATH=/usr/local/lib/girepository-1.0
# export LD_LIBRARY_PATH=/usr/local/lib
# libinsane-gobject/examples/list_devices.py

import gi
gi.require_version('Libinsane', '0.1')
from gi.repository import GObject  # noqa: E402

#! [Logger]
from gi.repository import Libinsane  # noqa: E402


class ExampleLogger(GObject.GObject, Libinsane.Logger):
    def do_log(self, lvl, msg):
        if lvl <= Libinsane.LogLevel.DEBUG:
            return
        print("{}: {}".format(lvl.value_nick, msg))


def main():
    Libinsane.register_logger(ExampleLogger())
    #! [Logger]
    api = Libinsane.Api.new_safebet()
    devs = api.list_devices(Libinsane.DeviceLocations.ANY)
    print("Found {} devices".format(len(devs)))
    for dev in devs:
        print(dev.to_string())


if __name__ == "__main__":
    main()
