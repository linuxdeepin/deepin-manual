SHELL   = /bin/bash
PREFIX := /opt/deepin-user-manual

all: nodejs
	echo "Installing dependencies"
	cd PageRoot && \
	    PATH="$(shell pwd)/symdir/:$$PATH" npm install

	mkdir -p PageRoot/www/scripts

	echo "Transpiling"
	mkdir -p PageRoot/www/scripts
	cd PageRoot && \
	    PATH="$(shell pwd)/symdir/:$$PATH" node --harmony ./node_modules/gulp/bin/gulp.js dist

nodejs:
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
	chmod +x $(DESTDIR)$(PREFIX)/main.js
	install -d $(DESTDIR)/usr/bin
	ln -s $(PREFIX)/main.js $(DESTDIR)/usr/bin/dman

clean:
	rm -rf symdir/
	rm -rf debian/deepin-user-manual
	rm -rf debian/files
	rm -rf debian/*.debhelper
	rm -rf debian/*.debhelper.log
	rm -rf debian/*.substvars

deepclean:
	rm -rf PageRoot/www/{style,scripts}

.PHONY: all nodejs install clean deepclean