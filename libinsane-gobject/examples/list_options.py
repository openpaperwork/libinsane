#!/usr/bin/env python3

# make
# sudo make install
# export GI_TYPELIB_PATH=/usr/local/lib/girepository-1.0
# export LD_LIBRARY_PATH=/usr/local/lib
# libinsane-gobject/examples/list_options.py

import gi
gi.require_version('Libinsane', '0.1')
from gi.repository import GObject  # noqa: E402

from gi.repository import Libinsane  # noqa: E402


class ExampleLogger(GObject.GObject, Libinsane.Logger):
    def do_log(self, lvl, msg):
        if lvl <= Libinsane.LogLevel.DEBUG:
           return
        print("{}: {}".format(lvl.value_nick, msg))


def main():
    Libinsane.register_logger(ExampleLogger())
    api = Libinsane.Api.new_safebet()

    devs = api.list_devices(Libinsane.DeviceLocations.ANY)
    print("Found {} devices".format(len(devs)))
    for dev in devs:
        print(dev.to_string())

        dev = api.get_device(dev.get_dev_id())

        for item in [dev] + dev.get_children():
            print("|-- " + dev.get_name())
            opts = item.get_options()
            for opt in opts:
                print("|   |-- {}".format(opt.get_name()))
                print("|   |   |-- Title: {}".format(opt.get_title()))
                print("|   |   |-- Description: {}".format(opt.get_desc()))
                print("|   |   |-- Capabilities: {}".format(
                    opt.get_capabilities())
                )
                print("|   |   |-- Unit: {}".format(opt.get_value_unit()))
                print("|   |   |-- Constraint type: {}".format(
                      opt.get_constraint_type()))
                print("|   |   |-- Constraint: {}".format(
                    opt.get_constraint())
                )
                if opt.is_readable():
                    print("|   |   |-- Value: {}".format(opt.get_value()))
                else:
                    print("|   |   |-- Value: (unavailable)")
        print("")
        dev.close()

if __name__ == "__main__":
    main()
