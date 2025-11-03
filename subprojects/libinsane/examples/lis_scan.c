#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libinsane/capi.h>
#include <libinsane/constants.h>
#include <libinsane/error.h>
#include <libinsane/log.h>
#include <libinsane/safebet.h>
#include <libinsane/util.h>

#include "../src/bmp.h"


#ifdef DISABLE_DEBUG
static void noop(enum lis_log_level lvl, const char *msg) {
	LIS_UNUSED(lvl);
	LIS_UNUSED(msg);
}
#endif


struct bmp {
	FILE *fp;

	struct lis_scan_parameters params;

	size_t line_length;
	size_t line_padding;

	size_t line_current;
};


static struct lis_log_callbacks g_log_callbacks = {
	.callbacks = {
#ifdef DISABLE_DEBUG
		[LIS_LOG_LVL_DEBUG] = noop,
#else
		[LIS_LOG_LVL_DEBUG] = lis_log_stderr,
#endif
		[LIS_LOG_LVL_INFO] = lis_log_stderr,
		[LIS_LOG_LVL_WARNING] = lis_log_stderr,
		[LIS_LOG_LVL_ERROR] = lis_log_stderr,
	},
};


static int open_bmp(struct bmp *out, const char *file)
{
	memset(out, 0, sizeof(struct bmp));
	out->fp = fopen(file, "wb");
	return (out->fp != NULL);
}


static void write_header(struct bmp *out, const struct lis_scan_parameters *params)
{
	char header[BMP_HEADER_SIZE];
	fpos_t cur_pos;
	memcpy(&out->params, params, sizeof(struct lis_scan_parameters));
	fgetpos(out->fp, &cur_pos);

	/* Normalizers ensure us that we will always get RAW_RGB_24. So this
	 * could actually be an assert. However it's useful for testing
	 * to be able to handle other formats as is.
	 */
	if (params->format != LIS_IMG_FORMAT_RAW_RGB_24) {
		printf(
			"WARNING: Unexpected output format."
			" Won't add BMP header or padding\n"
		);
		return;
	}

	out->line_length = out->params.width * 3;
	out->line_padding = 4 - (out->line_length % 4);
	if (out->line_padding == 4) {
		out->line_padding = 0;
	}

	lis_scan_params2bmp(params, header, 24 /* assuming RGB24 */);
	fseek(out->fp, 0, SEEK_SET );
	fwrite(header, sizeof(header), 1, out->fp);
	fsetpos(out->fp, &cur_pos );

	// TODO(JFlesch): check fwrite() result
}


static void write_pixels(struct bmp *out, const void *data, size_t lng)
{
	static char padding[3] = { 0 };
	size_t to_write;

	if (out->params.format != LIS_IMG_FORMAT_RAW_RGB_24) {
		fwrite(data, lng, 1, out->fp);
		return;
	}

	assert(out->line_padding <= sizeof(padding));

	while(lng > 0) {
		// BMP lines are always padded to be multiples of 4 bytes.

		to_write = MIN(
			lng,
			out->line_length - out->line_current
		);
		if (to_write > 0) {
			fwrite(data, to_write, 1, out->fp);
			// TODO(JFlesch): check fwrite() result
		}

		out->line_current += to_write;
		lng -= to_write;
		data = ((char *)data) + to_write;

		if (out->line_current >= out->line_length) {
			if (out->line_padding > 0) {
				fwrite(padding, out->line_padding, 1, out->fp);
				// TODO(JFlesch): check fwrite() result
			}
			out->line_current = 0;
		}
	}
}


static void close_bmp(struct bmp *out)
{
	fclose(out->fp);
}


//! [ExampleLisScan]
static void lets_scan(struct bmp *out, const char *dev_id)
{
#define CHECK_ERR(call) do { \
		err = call; \
		if (LIS_IS_ERROR(err)) { \
			fprintf( \
				stderr, "%s(L%d): ERROR: %X, %s\n", \
				__FILE__, __LINE__, \
				err, lis_strerror(err) \
			); \
			fflush(stderr); \
			goto end; \
		} \
	} while(0)


	enum lis_error err;
	struct lis_api *impl = NULL;
	struct lis_device_descriptor **dev_infos;
	struct lis_item *device = NULL;
	struct lis_item **sources;
	struct lis_scan_parameters parameters;
	struct lis_scan_session *scan_session;
	char img_buffer[32*1024];
	size_t bufsize;
	size_t obtained = 0;

	CHECK_ERR(lis_safebet(&impl));
	printf("Will use API %s\n", impl->base_name);

	if (dev_id != NULL) {
		printf("Will use device %s\n", dev_id);
	} else {
		CHECK_ERR(impl->list_devices(
			impl, LIS_DEVICE_LOCATIONS_ANY, &dev_infos
		));

		if (dev_infos[0] == NULL) {
			fprintf(stderr, "No scan device found\n");
			return;
		}

		// let's use the first scan device found, because it looks cool.
		printf("Will use device %s %s (%s ; %s)\n",
				dev_infos[0]->vendor, dev_infos[0]->model,
				dev_infos[0]->type,
				dev_infos[0]->dev_id);
		dev_id = dev_infos[0]->dev_id;
	}
	CHECK_ERR(impl->get_device(impl, dev_id, &device));

	CHECK_ERR(device->get_children(device, &sources));

	// Normalizers ensure us that there is at least one source,
	// so let's use the first one because it looks cool too.
	printf("Will use source '%s'\n", sources[0]->name);

	// Setting resolution: This one may or may not work, depending on
	// the scanner
	printf("Setting resolution to 300\n");
	lis_set_option(sources[0], OPT_NAME_RESOLUTION, "300");

	// Normalizers ensure us that the mode option has the value "Color"
	// by default (still put here for the example)
	printf("Setting mode to Color\n");
	CHECK_ERR(lis_set_option(sources[0], OPT_NAME_MODE, "Color"));
	// Normalizers ensure us that by default, the maximum scan area will
	// be used

	CHECK_ERR(sources[0]->scan_start(sources[0], &scan_session));

	memset(img_buffer, 0, sizeof(img_buffer));

	while (!scan_session->end_of_feed(scan_session)) {
		// scan parameters must be obtained *after* the scan session has been
		// started and before getting the content of a new page.
		// DO NOT assume that 2 pages in the same feed will have the same size.
		CHECK_ERR(scan_session->get_scan_parameters(
			scan_session, &parameters
		));
		printf(
			"Scan will be: %d px x %d px (%ld bytes)\n",
			parameters.width, parameters.height,
			(long)parameters.image_size
		);
		write_header(out, &parameters);

		while (!scan_session->end_of_page(scan_session)) {
			bufsize = sizeof(img_buffer);
			err = scan_session->scan_read(
				scan_session, img_buffer, &bufsize
			);
			CHECK_ERR(err);

			if (err == LIS_WARMING_UP) {
				// old scanners need warming time.
				// No data has been returned.
				assert(bufsize == 0);
				sleep(1);
				continue;
			}

			obtained += bufsize;
			printf(
				"\r%ld KB / %ld KB",
				(long)(obtained / 1024),
				(long)(parameters.image_size / 1024)
			);
			fflush(stdout);

			// do something with the chunk of the image/page that
			// has just been scanned

			// here for example we write simply to a BMP file
			write_pixels(out, img_buffer, bufsize);
		}
	
		// do something with the whole image/page that has just been scanned
		CHECK_ERR(scan_session->get_scan_parameters(
			scan_session, &parameters
		));
		parameters.height = obtained / 3 / parameters.width;
		parameters.image_size = obtained;
		printf(
			"Final: Scan will be: %d px x %d px (%ld bytes)\n",
			parameters.width, parameters.height,
			(long)parameters.image_size
		);
		write_header(out, &parameters);
	}

	// do something with all the images/pages that have just been scanned
	printf("\nAll done !\n");

end:
	if (device != NULL) {
		device->close(device);
	}
	if (impl != NULL) {
		impl->cleanup(impl);
	}

#undef CHECK_ERR
}
//! [ExampleLisScan]


int main(int argc, char **argv)
{
	const char *dev_id = NULL;
	const char *output_bmp = NULL;
	struct bmp out;

	if (argc <= 1 || strcasecmp(argv[1], "-h") == 0
			|| strcasecmp(argv[1], "help") == 0) {
		fprintf(stderr, "Syntax:\n");
		fprintf(stderr, "  %s <output file> [<dev id>]\n", argv[0]);
		return EXIT_FAILURE;
	}

	output_bmp = argv[1];
	if (argc >= 3) {
		dev_id = argv[2];
	}

	if (!open_bmp(&out, output_bmp)) {
		fprintf(stderr, "Failed to open '%s'", output_bmp);
		return EXIT_FAILURE;
	}

	lis_set_log_callbacks(&g_log_callbacks);
	lets_scan(&out, dev_id);

	close_bmp(&out);
	return EXIT_SUCCESS;
}
