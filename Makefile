PREFIX := /opt/deepin-user-manual

all:
	mkdir -p dist/PageRoot
	cp -r DMan dist/
	cp -r PageRoot/www dist/PageRoot/www
	cp -r PageRoot/node_modules dist/PageRoot/node_modules

install:
	cp -r dist/ $(DESTDIR)/$(PREFIX)
	chomd +x $(DESTDIR)/$(PREFIX)/DMan/main.py


clean:
	rm -rf dist/

.PHONY: all dist install clean