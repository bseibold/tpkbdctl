/* 
 * (c) 2012 Bernhard Seibold <bernhard.seibold@googlemail.com>
 * License: GPLv2
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <libgen.h>

#include <linux/types.h>
#include <linux/input.h>
#include <linux/hiddev.h>
#include <linux/hidraw.h>

#ifndef VERSION
#define VERSION (unknown)
#endif

#define QUOTE(x) #x
#define STR(x) QUOTE(x)

int verbosity = 0;


int find_device() {
	int i, fd, ret, descsize;
	char name[255];
	struct hidraw_devinfo devinfo;
	for (i=0; i<255; i++) {
		snprintf(name, 255, "/dev/hidraw%d", i);
		fd = open(name, O_RDWR|O_NONBLOCK);
		if (!fd)
			continue;

		ret = ioctl(fd, HIDIOCGRAWINFO, &devinfo);
		if (ret) {
			close(fd);
			continue;
		}
		if (devinfo.vendor != 0x17ef || devinfo.product != 0x6009) {
			close(fd);
			continue;
		}

		ret = ioctl(fd, HIDIOCGRDESCSIZE, &descsize);
		if (ret || descsize < 100) {
			close(fd);
			continue;
		}

		if (verbosity > 0)
			fprintf(stderr, "Found device at %s\n", name);
		return fd;
	}
	return 0;
}


int main(int argc, char* argv[]) {
	int press_to_select = 0;
	int dragging = 0;
	int release_to_select = 0;
	int select_right = 0;
	int sensitivity = 128;
	int press_speed = 0x38;
	int show_help = 0;
	int show_version = 0;
	int fd, ret;
	char *device = NULL;
	char buf[5];

	int opt, opt_idx;
	static struct option longopts[] = {
		{ "device", required_argument, 0, 0 },
		{ "sensitivity", required_argument, 0, 0},
		{ "press_speed", required_argument, 0, 0},
		{ "help", no_argument, 0, 0},
		{ "press-to-select", no_argument, 0, 0},
		{ "dragging", no_argument, 0, 0},
		{ "release-to-select", no_argument, 0, 0},
		{ "select-with-right", no_argument, 0, 0},
		{ "version", no_argument, 0, 0},
		{ 0 }	
	};
	
	while (1) {
		opt = getopt_long(argc, argv, "d:s:S:pDrRvhV", longopts, &opt_idx );
		if (opt == -1)
			break;

		switch (opt) {
		case 0:
			/* long option */
			switch (opt_idx) {
			case 0: device = optarg; break;
			case 1: sensitivity = atoi(optarg); break;
			case 2: press_speed = atoi(optarg); break;
			case 3: show_help = 1; break;
			case 4: press_to_select = 1; break;
			case 5: dragging = 1; break;
			case 6: release_to_select = 1; break;
			case 7: select_right = 1; break;
			case 8: show_version = 1; break;
			}
			break;

		case 'd':
			if (optarg)
				device = optarg;
			break;

		case 's':
			if (optarg)
				sensitivity = atoi(optarg);
			break;

		case 'S':
			if (optarg)
				press_speed = atoi(optarg);
			break;

		case 'p':
			press_to_select = 1;
			break;

		case 'D':
			dragging = 1;
			break;

		case 'r':
			release_to_select = 1;
			break;

		case 'R':
			select_right = 1;
			break;

		case 'v':
			verbosity++;
			break;

		case 'h':
			show_help = 1;
			break;

		case 'V':
			show_version = 1;
			break;

		default:
			break;


		}
	}

	if (show_version) {
		fprintf(stderr, "%s version " STR(VERSION) "\n", basename(argv[0]));
		exit(0);
	}

	if (show_help) {
		fprintf(stderr, "Usage: %s [OPTIONS]\n\n", basename(argv[0]));
		fprintf(stderr, "  -h, --help               show this\n");
		fprintf(stderr, "  -V, --version            show version\n");
		fprintf(stderr, "  -d, --device=?           set device. Autodiscovered if unset\n");
		fprintf(stderr, "  -v                       increase verbosity\n");
		fprintf(stderr, "  -s, --sensitivity=?      set sensitivity, range 1-255\n");
		fprintf(stderr, "  -S, --press-speed=?      set press-speed, range 1-255\n");
		fprintf(stderr, "  -p, --press-to-select    enable press-to-select\n");
		fprintf(stderr, "  -D, --dragging           enable dragging\n");
		fprintf(stderr, "  -r, --release-to-select  enable release-to-select\n");
		fprintf(stderr, "  -R, --select-with-right  select with right button instead of left\n");
		fprintf(stderr, "\n");
		exit(0);
	}

	if (sensitivity < 1 || sensitivity > 255) {
		fprintf(stderr, "ERROR: sensitivity not within valid range 1-255\n");
		exit(1);
	}

	if (press_speed < 1 || press_speed > 255) {
		fprintf(stderr, "ERROR: press-speed not within valid range 1-255\n");
		exit(1);
	}

	if (device) {
		fd = open(device, O_RDWR|O_NONBLOCK);
	} else {
		fd = find_device();
	}

	if (fd<0) {
		perror("No device found");
		//fprintf(stderr, "No device found.\n");
		exit(1);
	}

	buf[0] = 4;
	buf[1]  = press_to_select   ? 0x01 : 0x02;
	buf[1] |= dragging          ? 0x04 : 0x08;
	buf[1] |= release_to_select ? 0x10 : 0x20;
	buf[1] |= select_right      ? 0x80 : 0x40;
	buf[2] = 0x03;
	buf[3] = sensitivity;
	buf[4] = press_speed;

	if (verbosity > 0) {
#define bool2str(x) ((x)?"True":"False")
		fprintf(stderr, "Setting values:\n");
		fprintf(stderr, "sensitivity:          %d\n", sensitivity);
		fprintf(stderr, "press-to-select:      %s\n", bool2str(press_to_select));
		fprintf(stderr, "press-speed:          %d\n", press_speed);
		fprintf(stderr, "dragging              %s\n", bool2str(dragging));
		fprintf(stderr, "select-with-right     %s\n", bool2str(select_right));
#undef bool2str
	}

	ret = ioctl(fd, HIDIOCSFEATURE(5), buf);

	if (ret < 0) {
		close(fd);
		perror("Could not set values");
		//fprintf(stderr, "Error, could not set values (%d)\n", ret);
		exit(1);
	}

	close(fd);
}
