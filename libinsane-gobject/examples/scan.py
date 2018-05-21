#!/usr/bin/env python3

# make
# make install
# export GI_TYPELIB_PATH=/usr/local/lib/girepository-1.0
# export LD_LIBRARY_PATH=/usr/local/lib
# libinsane-gobject/examples/scan.py

import sys

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

    if len(sys.argv) > 1 and (sys.argv[1] == "-h" or sys.argv[1] == "--help"):
        print(
            "Syntax: {}"
            " [<output file>]"
            " [<scan dev id> [<scan source name>]]"
        )
        sys.exit(1)

    output_file = sys.argv[1] if len(sys.argv) > 1 else "/dev/null"
    dev_id = None
    source = None
    source_name = None
    if len(sys.argv) > 2:
        dev_id = sys.argv[2]
    if len(sys.argv) > 3:
        source = sys.argv[3]

    print("Will write the scan result into {}".format(output_file))

    print("Looking for scan devices ...")
    api = Libinsane.Api.new_safebet()
    if dev_id is None:
        devs = api.list_devices(Libinsane.DeviceLocations.ANY)
        print("Found {} devices".format(len(devs)))
        for dev in devs:
            print("[{}] : [{}]".format(dev.get_dev_id(), dev.to_string()))
        dev_id = devs[0].get_dev_id()
    print("Will use device {}".format(dev_id))
    dev = api.get_device(dev_id)
    print("Using device {}".format(dev.get_name()))

    print("Looking for scan sources ...")
    sources = dev.get_children()
    print("Available scan sources:")
    for src in sources:
        print("- {}".format(src.to_string()))
        if src.get_name() == source_name:
            source = src
    if source == "root":
        source = dev
    if source_name is None:
        source = sources[0] if len(sources) > 0 else dev
    if source is None:
        print("Failed to find scan source \"{}\"".format(source_name))
        sys.exit(2)
    print("Will use scan source {}".format(source.get_name()))

    # set the options
    # TODO

    # scan
    # TODO

if __name__ == "__main__":
    main()
