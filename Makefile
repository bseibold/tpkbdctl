
DESTDIR?=/

VERSION=$(shell git describe --tags --always 2>/dev/null)
VERSION?=0.1

CFLAGS+=-std=c99 -Wall -Werror -DVERSION=$(VERSION)


all: tpkbdctl

clean:
	rm -f tpkbdctl

install: all
	mkdir -p $(DESTDIR)/usr/bin
	cp tpkbdctl $(DESTDIR)/usr/bin/

.PHONY: clean install
