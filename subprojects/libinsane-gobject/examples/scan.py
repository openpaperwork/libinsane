#!/usr/bin/env python3

# source ./activate_test_env.sh
# subprojects/libinsane-gobject/examples/scan.py

import os
import PIL.Image
import sys
import traceback

import gi
gi.require_version('Libinsane', '1.0')
from gi.repository import GObject  # noqa: E402


from gi.repository import Libinsane  # noqa: E402

#! [ExampleLogger]
class ExampleLogger(GObject.GObject, Libinsane.Logger):
    def __init__(self):
        super().__init__()
        self.prefix = dict()
        self.prefix[Libinsane.LogLevel.DEBUG] = "DBG"
        self.prefix[Libinsane.LogLevel.INFO] = "INF"
        self.prefix[Libinsane.LogLevel.WARNING] = "WRN"
        self.prefix[Libinsane.LogLevel.ERROR] = "ERR"

    def do_log(self, lvl, msg):
        if lvl <= Libinsane.LogLevel.INFO:
            return
        print("{}: {}".format(self.prefix[lvl], msg))
        sys.stdout.flush()
#! [ExampleLogger]


#! [ExampleListGetDevice]
def get_device(api, dev_id=None):
    if dev_id is None:
        print("Looking for scan devices ...")
        devs = api.list_devices(Libinsane.DeviceLocations.ANY)
        print("Found {} devices".format(len(devs)))
        for dev in devs:
            print("[{}] : [{}]".format(dev.get_dev_id(), dev.to_string()))
        dev_id = devs[0].get_dev_id()

    print("Will use device {}".format(dev_id))
    dev = api.get_device(dev_id)
    print("Using device {}".format(dev.get_name()))
    return dev
#! [ExampleListGetDevice]


#! [ExampleListGetScanSource]
def get_source(dev, source_name):
    print("Looking for scan sources ...")
    sources = dev.get_children()
    print("Available scan sources:")
    for src in sources:
        print("- {}".format(src.get_name()))
        if src.get_name() == source_name:
            source = src
            break
    else:
        if source_name is None:
            source = sources[0] if len(sources) > 0 else dev
        elif source_name == "root":
            source = dev
        else:
            print("Source '{}' not found".format(source_name))
            sys.exit(2)
    print("Will use scan source {}".format(source.get_name()))
    return source
#! [ExampleListGetScanSource]


#! [ExampleSetOption]
def set_opt(item, opt_name, opt_value):
    try:
        print("Setting {} to {}".format(opt_name, opt_value))
        opts = item.get_options()
        opts = {opt.get_name(): opt for opt in opts}
        if opt_name not in opts:
            print("Option '{}' not found".format(opt_name))
            return
        print("- Old {}: {}".format(opt_name, opts[opt_name].get_value()))
        print("- Allowed values: {}".format(opts[opt_name].get_constraint()))
        set_flags = opts[opt_name].set_value(opt_value)
        print("- Set flags: {}".format(set_flags))
        opts = item.get_options()
        opts = {opt.get_name(): opt for opt in opts}
        print("- New {}: {}".format(opt_name, opts[opt_name].get_value()))
    except Exception as exc:
        print("Failed to set {} to {}: {}".format(
            opt_name, opt_value, str(exc)
        ))
        traceback.print_exc()
    finally:
        print("")
#! [ExampleSetOption]


#! [ExampleListOptions]
def list_opts(item):
    opts = item.get_options()
    print("Options:")
    for opt in opts:
        try:
            print("- {}={} ({})".format(
                opt.get_name(), opt.get_value(), opt.get_constraint()
            ))
        except Exception as exc:
            print("Failed to read option {}: {}".format(
                opt.get_name(), str(exc)
            ))
    print("")
#! [ExampleListOptions]


#! [ExampleRawToImg]
def raw_to_img(params, img_bytes):
    fmt = params.get_format()
    assert(fmt == Libinsane.ImgFormat.RAW_RGB_24)
    (w, h) = (
        params.get_width(),
        int(len(img_bytes) / 3 / params.get_width())
    )
    print("Mode: RGB : Size: {}x{}".format(w, h))
    return PIL.Image.frombuffer("RGB", (w, h), img_bytes, "raw", "RGB", 0, 1)
#! [ExampleRawToImg]


#! [ExampleScan]
def scan(source, output_file):
    session = source.scan_start()

    try:
        page_nb = 0
        while not session.end_of_feed() and page_nb < 20:
            # Do not assume that all the pages will have the same size !
            scan_params = session.get_scan_parameters()
            total = scan_params.get_image_size()
            if scan_params.get_height() < 0:
                total = "unknown"
            else:
                total = f"{total} B"
            print(
                "Expected scan parameters:"
                f" {scan_params.get_format()} ;"
                f" {scan_params.get_width()}x{scan_params.get_height()}"
                f" = {total}"
            )

            img = []
            r = 0
            if output_file is not None:
                out = output_file.format(page_nb)
            else:
                out = None
            print("Scanning page {} --> {}".format(page_nb, out))
            while not session.end_of_page():
                data = session.read_bytes(128 * 1024)
                data = data.get_data()
                img.append(data)
                r += len(data)
                print(f"Got {len(data)} bytes => {r} B / {total}")

            img = b"".join(img)
            print("Got {} bytes".format(len(img)))
            if out is not None:
                print("Saving page as {} ...".format(out))
                if scan_params.get_format() == Libinsane.ImgFormat.RAW_RGB_24:
                    img = raw_to_img(scan_params, img)
                    img.save(out, format="PNG")
                else:
                    print("Warning: output format is {}".format(
                        scan_params.get_format()
                    ))
                    with open(out, 'wb') as fd:
                        fd.write(img)
            page_nb += 1
            print("Page {} scanned".format(page_nb))
        if page_nb == 0:
            print("No page in feeder ?")
    finally:
        session.cancel()
#! [ExampleScan]


def main():
    if False:
        os.environ['LIBINSANE_NORMALIZER_ALL_OPTS_ON_ALL_SOURCES'] = "0"
        os.environ['LIBINSANE_NORMALIZER_BMP2RAW'] = "0"
        os.environ['LIBINSANE_NORMALIZER_CLEAN_DEV_DESCS'] = "0"
        os.environ['LIBINSANE_NORMALIZER_MIN_ONE_SOURCE'] = "0"
        os.environ['LIBINSANE_NORMALIZER_OPT_ALIASES'] = "0"
        os.environ['LIBINSANE_NORMALIZER_RAW24'] = "0"
        os.environ['LIBINSANE_NORMALIZER_RESOLUTION'] = "0"
        os.environ['LIBINSANE_NORMALIZER_SAFE_DEFAULTS'] = "0"
        os.environ['LIBINSANE_NORMALIZER_SOURCE_NAMES'] = "0"
        os.environ['LIBINSANE_NORMALIZER_SOURCE_NODES'] = "0"
        os.environ['LIBINSANE_NORMALIZER_SOURCE_TYPES'] = "0"
        os.environ['LIBINSANE_WORKAROUND_CACHE'] = "0"
        os.environ['LIBINSANE_WORKAROUND_CHECK_CAPABILITIES'] = "0"
        os.environ['LIBINSANE_WORKAROUND_DEDICATED_THREAD'] = "0"
        os.environ['LIBINSANE_WORKAROUND_LAMP'] = "0"
        os.environ['LIBINSANE_WORKAROUND_ONE_PAGE_FLATBED'] = "0"
        os.environ['LIBINSANE_WORKAROUND_OPT_NAMES'] = "0"
        os.environ['LIBINSANE_WORKAROUND_OPT_VALUES'] = "0"

#! [ExampleSetLogger]
    Libinsane.register_logger(ExampleLogger())
#! [ExampleSetLogger]

    if len(sys.argv) <= 1 or (sys.argv[1] == "-h" or sys.argv[1] == "--help"):
        print(
            "Syntax: {}"
            " <PNG output file>"
            " [<scan dev id> [<scan source name>]]".format(sys.argv[0])
        )
        sys.exit(1)

    output_file = sys.argv[1]
    dev_id = None
    source = None
    source_name = None
    if len(sys.argv) > 2:
        dev_id = sys.argv[2]
    if len(sys.argv) > 3:
        source_name = sys.argv[3]

    print("Will write the scan result into {}".format(output_file))

#! [ExampleInit]
    api = Libinsane.Api.new_safebet()
#! [ExampleInit]

    dev = get_device(api, dev_id)
    try:
        source = get_source(dev, source_name)

        list_opts(source)

        # set the options
#! [ExampleOptsToSet]
        set_opt(source, 'resolution', 300)
        # set_opt(source, 'mode', "LineArt")
        # set_opt(source, 'depth', 1)
#! [ExampleOptsToSet]

        print("Scanning ...")
        scan(source, output_file)
        print("Scan done")
    finally:
        dev.close()


if __name__ == "__main__":
    main()
