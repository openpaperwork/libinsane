#ifndef __LIBINSANE_WORKAROUNDS_H
#define __LIBINSANE_WORKAROUNDS_H

#include "capi.h"


#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Option 'scan-resolution' --> 'resolution'
 *
 * - API: Sane
 * - Culprit: Lexmark
 * - Seen on: Lexmark MFP
 *
 * The option 'resolution' is mistakenly named 'scan-resolution'.
 * This workaround creates a mirror option 'resolution'.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] api Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_workaround_opt_scan_resolution(
	struct lis_api *to_wrap, struct lis_api **api
);


/*!
 * \brief Option 'mode': Unusual mode values
 *
 * - API: Sane
 * - Culprit: Samsung
 * - Seen on: [Samsung CLX-3300](https://openpaper.work/scanner_db/report/31/)
 *
 * The option 'resolution' is mistakenly named 'scan-resolution'.
 * This workaround creates a mirror option 'resolution'.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] api Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_workaround_opt_doc_source(
	struct lis_api *to_wrap, struct lis_api **api
);


/*!
 * \brief Option 'mode': Unusual mode values
 *
 * - API: Sane
 * - Culprits: Brother, Samsung
 * - Seen on:
 *   - [Brother MFC-7360N](https://openpaper.work/scanner_db/report/20/)
 *   - [Samsung CLX-3300](https://openpaper.work/scanner_db/report/31/)
 *
 * Override the option 'mode' so it changes the following possible values:
 *
 * - Brother
 *   - '24bit Color' --> 'Color'
 *   - 'Black & White' --> 'Lineart'
 *   - 'True Gray' --> 'Gray'
 * - Samsung
 *   - 'Black and White - Line Art' --> 'Lineart'
 *   - 'Grayscale - 256 Levels' --> 'Gray'
 *   - 'Color - 16 Million Colors' --> 'Color'
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] api Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_workaround_opt_mode(
	struct lis_api *to_wrap, struct lis_api **api
);


/*!
 * \brief Extra options 'page-height' and 'page-width'
 *
 * - API: Sane
 * - Culprit: Fujitsu
 * - Seen on: [Fujitsu ScanSnap S1500](https://github.com/openpaperwork/paperwork/issues/230#issuecomment-22792362)
 *
 * This workaround wraps the option `br_x`, `tl_x`, `br_y` and `tl_y` in order to
 * set `page-height` and `page-width` accordingly.
 *
 * \todo Need to figure out how to set them exactly. Currently: set proportionally to (br_x - tl_x,
 *		br_y - tl_y).
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] api Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_workaround_opts_page_size(
	struct lis_api *to_wrap, struct lis_api **api
);


/*!
 * \brief Strip option translations
 *
 * - API: Sane
 * - Culprits: Sane project, OKI
 * - Seen on: [OKI MC363](https://openpaper.work/scanner_db/report/56/)
 *
 * This workaround wraps a bunch of options, and try to revert the translations back to English.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] api Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_workaround_strip_translations(
	struct lis_api *to_wrap, struct lis_api **api
);


/*!
 * \brief Device model name may contain manufacturer name
 *
 * - API: Sane, WIA
 * - Culprits: too many. Especially HP.
 *
 * If the model name contains also the manufacturer name, this workaround strips it.
 *
 * Random example:
 *
 * - Manufacturer: Brother
 * - Model: Brother MFC-7360N
 *
 * Will become:
 *
 * - Manufacturer: Brother
 * - Model: MFC-7360N
 *
 * Special case: HP. Manufacturer is "hewlett-packard", but
 * [model names contain the prefix "hp"](https://openpaper.work/scanner_db/vendor/7/).
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] api Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_workaround_clean_dev_model_from_manufacturer(
	struct lis_api *to_wrap, struct lis_api **api
);


/*!
 * \brief Device model name may contain '_' instead of spaces
 *
 * - API: Sane
 * - Culprit: HP
 * - Seen on: [all HP devices](https://openpaper.work/scanner_db/vendor/7/)
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] api Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_workaround_clean_dev_model_char(
	struct lis_api *to_wrap, struct lis_api **api
);


/*!
 * \brief Do not let application access value of inactive options
 *
 * - API: Sane
 * - Culprit: Can't remember
 *
 * Some drivers allows access to inactive options (even just for reading).
 * Some may even crash if the user application tries to set a value on an inactive option.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] api Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_workaround_no_read_on_inactive_opt(
	struct lis_api *to_wrap, struct lis_api **api
);


/*!
 * \brief Do not let application set value on read-only options
 *
 * - API: Sane
 * - Culprit: Can't remember
 *
 * Behavior is undefined when trying to set read-only values.
 * This workaround makes it defined: it always returns an error.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] api Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_workaround_no_write_on_readonly_opt(
	struct lis_api *to_wrap, struct lis_api **api
);


/*!
 * \brief Do not let application set value on option that can have only one value
 *
 * - API: Sane
 * - Culprit: Epson
 * - Seen on: [Epson DS-310](https://openpaper.work/scanner_db/report/120/)
 *
 * When trying to set a value on a property that accept only one value
 * (ex: source=ADF), Sane driver may return SANE_STATUS_INVAL instead of success.
 * This workaround makes sure the value provided matches the only one possible
 * and doesn't even set it.
 */
extern enum lis_error lis_api_workaround_no_write_on_single_value_opt(
	struct lis_api *to_wrap, struct lis_api **api
);


/*!
 * \brief Thread-safety
 *
 * - API: Sane, WIA, TWAIN
 *
 * Most scanner APIs are not thread-safe. If you're lucky, they may work
 * from different threads anyway. If you're not, they will crash your program.
 *
 * This workaround works around this issue by creating a dedicated thread for
 * the job and making all the request go through this thread.
 *
 * \param[in] to_wrap Base implementation to wrap.
 * \param[out] api Implementation of the API including the workaround.
 */
extern enum lis_error lis_api_workaround_dedicated_thread(
	struct lis_api *to_wrap, struct lis_api **api
);

#ifdef __cplusplus
}
#endif

#endif
