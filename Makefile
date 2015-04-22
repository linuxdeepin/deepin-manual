SHELL   = /bin/bash
PREFIX := /opt/deepin-manual

all: nodejs
	echo "Copying icon"
	cp DMan/DManual.svg PageRoot/www/favicon.svg

dist:
	# Installing node modules and transpile scripts
	echo "Installing dependencies"
	cd PageRoot && \
	    PATH="$(shell pwd)/symdir/:$$PATH" npm install

	echo "Transpiling"
	mkdir -p PageRoot/www/scripts
	cd PageRoot && \
	    PATH="$(shell pwd)/symdir/:$$PATH" node --harmony ./node_modules/gulp/bin/gulp.js dist

nodejs:
	# Debian-based distros use nonstandard node executable name
	mkdir -p symdir
	ln -sf /usr/bin/nodejs ./symdir/node

install:
	mkdir -p $(DESTDIR)$(PREFIX)/{PageRoot,DMan}
	cp -r PageRoot/{po,www} $(DESTDIR)$(PREFIX)/PageRoot/
	cp -r DMan/* $(DESTDIR)$(PREFIX)/DMan/
	rm -r $(DESTDIR)$(PREFIX)/PageRoot/www/{jssrc,scss}
	find $(DESTDIR)$(PREFIX) -name "__pycache__" -print0 | xargs -0 rm -rf

	cp {main.js,package.json} $(DESTDIR)$(PREFIX)/
	chmod +x $(DESTDIR)$(PREFIX)/DMan/main.py

	# Atom-shell script
	chmod +x $(DESTDIR)$(PREFIX)/main.js
	# Remove Atom-shell things.
	rm $(DESTDIR)$(PREFIX)/{main.js,package.json}

	install -d $(DESTDIR)/usr/bin
	ln -s $(PREFIX)/main.js $(DESTDIR)/usr/bin/dman

	# Copy icons
	mkdir -p $(DESTDIR)/usr/share/icons/hicolor/scalable/apps
	cp DMan/DManual.svg $(DESTDIR)/usr/share/icons/hicolor/scalable/apps/deepin-manual.svg

clean:
	rm -rf symdir/
	rm -rf debian/deepin-manual
	rm -rf debian/files
	rm -rf debian/*.debhelper
	rm -rf debian/*.debhelper.log
	rm -rf debian/*.substvars

deepclean:
	rm -rf PageRoot/www/{style,scripts,nls,favicon.svg}

.PHONY: all dist nodejs install clean deepclean
