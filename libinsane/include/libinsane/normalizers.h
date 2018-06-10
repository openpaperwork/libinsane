#ifndef __LIBINSANE_NORMALIZERS_H
#define __LIBINSANE_NORMALIZERS_H

#include "capi.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \todo TWAIN
 */

/*!
 * \brief Ensure that sources are represented as node
 *
 * WIA2: For each scanner, provide a device tree (see :ref:`WIA2` description).
 *
 * Sane: There is no tree (see :ref:`Sane` description). Children nodes (sources) must be
 * simulated.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_source_nodes(
		struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Ensure there is at least one source
 *
 * - Culprits: Microsoft, Sane project
 *
 * Sane: [Some scanner do not provide the option "source"](https://openpaper.work/en-us/scanner_db/report/57).
 *
 * WIA2: [Some scanners do not always provide a source](https://openpaper.work/en/scanner_db/report/28). Scan is done directly on them.
 *
 * If there is no source at all, this normalization will create a fake one.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_min_one_source(
	struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Ensure all options are available on sources
 *
 * - Culprits: Microsoft, Sane project
 *
 * Goal: Makes sure the application can find easily options by making
 * the scanner options available on all its sources.
 *
 * Sane: Only the scanner itself has options. Options must all be mapped
 * too on all its sources.
 *
 * WIA: Some options are on the scanner itself, some options are on the
 * sources. Scanner options must be mapped on all its sources.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_all_opts_on_all_sources(
	struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Ensure scan area option names are all the same
 *
 * Sane: [Sane scan area options](http://sane.alioth.debian.org/sane2/0.08/doc014.html#s4.5.4)
 * are used as reference.
 *
 * WIA2: Fake options are added to simulate Sane options. They act like Sane
 * options, and any change to these options is applied back to the WIA2 options.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_scan_area_opts(
	struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Ensure resolution constraint is always in the same format
 *
 * - Culprits: Microsoft, Sane project
 *
 * Sane and WIA: Resolution constraint can be expressed as a range or as a list
 * of possible values. This normalization makes sure they are always expressed as
 * a list. If the range has an interval < 25dpi, the interval used to generate the
 * list will be 25dpi.
 *
 * Sane: Resolution can be expressed as integers or as SANE_Fixed values
 * (16 bits integer / 16 bits non-integer) (converted as float for the
 * C API)
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_resolution_format(
	struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Ensure source types are clearly identified
 *
 * - Culprit: Sane project
 *
 * Sane: Sources have "name", but the exact names are up to the drivers.
 *
 * WIA2: source types are already clearly defined.
 *
 * See \ref lis_item.type.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_source_types(
	struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Ensure there is only supported devices
 *
 * - Culprits: Sane project, Microsoft
 *
 * Sane: Remove v4l devices (video for linux ; Camera).
 *
 * WIA: Video devices are directly stripped by the WIA support module of LibInsane.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_strip_non_scanners(
	struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Ensure the output format is RAW
 *
 * - Culprit: Microsoft.
 *
 * Always getting the image as RAW24 is much more handy if you want to
 * display the scan on-the-fly.
 *
 * Sane: Image is always returned as RAW (unless some scanner-specific
 * options are set to non-default values).
 *
 * WIA2: Drivers may return the image in a variety of file formats: RAW, BMP,
 * JPEG, PNG, etc. Not all drivers support returning the image as RAW24.
 * LibInsane supports only BMP (luckily, all WIA drivers appear to support
 * this one) and will output the image as RAW24.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_raw(
	struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Ensure the output format is RAW24.
 *
 * - Culprit: Sane
 *
 * Sane can return the image as various raw formats:
 * RAW1 (B&W), RAW8 (Grayscale), RAW24 (RGB), etc.
 *
 * This normalization ensures the output image is always in RAW24 (RGB).
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_raw24(
	struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Ensure the behavior is always the same whenever we scan from a flatbed or a feeder.
 *
 * - Culprits: Sane project, Microsoft
 *
 * On both Sane and WIA:
 *
 * - When scanning from feeder: at the end of a scan, we get an error code
 *   (or a function call) telling us when the whole page has been scanned.
 *   When requesting another scan, it will scan the next page. It no next
 *   page is available, another error code (or function call) will tell
 *   us we have reached the end.
 * - When scanning from flatbed: at the end of a scan, we get an error code
 *   (or a function call) telling us when the whole page has been scanned.
 *   Requesting another scan, it will scan again the very same page. It will
 *   never tell us that there is no paper left to scan
 *
 * Problem: Behavior should be independent of the actual source. Having 2 behaviors
 * is just bug-prone.
 *
 * Wanted behavior:
 *
 * - When scanning from feeder: unchanged
 * - When scanning from flatbed: the first scan is unchanged. When requesting
 *   a second scan, it must return an error code indicating that there is
 *   no paper left to scan.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_flatbed_and_feeder_behavior(
	struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Ensure the scan area is set to the maximum by default.
 *
 * By default, some drivers don't have the scan area set to the maximum.
 * This workaround just make sure the default area is the maximum area.
 * It may be handy if you don't want to scan a specific area.
 *
 * Requires: \ref lis_api_normalizer_scan_area_opts
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_maximize_scan_area(
	struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Ensure the scan mode by default is 24bits colors.
 *
 * By default, some drivers don't have the mode set to color.
 * This workaround just make sure the default mode is 24bits color.
 *
 * Requires: \ref lis_api_workaround_opt_mode
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_default_mode_to_color(
	struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Ensure end of feed errors occurs only on \ref lis_scan_session.scan_read
 *	and never on \ref lis_item.scan_start .
 *
 * Depending on the driver, LIS_END_OF_FEED may happen either on
 * \ref lis_item.scan_start or on the first \ref lis_scan_session.scan_read.
 * This workaround makes sure that is always happen only on
 * \ref lis_scan_session.scan_read`.
 *
 * Example: https://openpaper.work/en-us/scanner_db/report/124/
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_end_of_feed(
	struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Makes sure the source names all look the same accross OSes
 *
 * - All source names will be lower-case
 * - No WIA prefix
 * - All ADF will be called "feeder"
 *
 * Exanples when using WIA:
 * - '0000\\Root\\Flatbed' --> 'flatbed'
 * - '0000\\Root\\Feeder' --> 'feeder'
 *
 * Examples when using Sane:
 * - 'Automatic document Feeder (left aligned)' --> 'feeder (left aligned)'
 * - 'ADF Duplex' --> 'feeder duplex'
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_source_names(
	struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Make sure the source order is always the same.
 *
 * Order:
 * - "flatbed"
 * - "feeder duplex"
 * - "feeder"
 * - "feeder (left aligned)"
 * - "feeder front"
 * - "feeder back"
 * - "feeder (centrally aligned)"
 *
 * Dependency: \ref lis_api_normalizer_source_names
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_sources_order(
	struct lis_api *to_wrap, struct lis_api **impl
);


/*!
 * \brief Automatically reload options when required
 *
 * When setting an option, the flag LIS_SET_FLAG_MUST_RELOAD_OPTIONS may be returned,
 * in which case, options must be reloaded through \ref lis_item.get-options.
 * This normalizer makes it happen automatically. However, it means previously
 * returned option descriptors may be updated automatically.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] impl Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_normalizer_auto_opt_reload(
	struct lis_api *to_wrap, struct lis_api **impl
);


#ifdef __cplusplus
}
#endif

#endif
