#!/usr/bin/env python3

# make
# make install
# export GI_TYPELIB_PATH=/usr/local/lib/girepository-1.0
# export LD_LIBRARY_PATH=/usr/local/lib
# libinsane-gobject/examples/list_devices.py

import gi
gi.require_version('Libinsane', '0.1')
from gi.repository import Libinsane  # noqa: E402


def main():
    api = Libinsane.Api.new_safebet()
    devs = api.get_devices()
    for dev in devs:
        print(dev)


if __name__ == "__main__":
    main()
