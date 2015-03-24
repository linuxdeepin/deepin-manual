PREFIX := /opt/deepin-user-manual

all: nodejs
	mkdir -p dist
	mkdir -p dist/PageRoot

	cp -r PageRoot/www dist/PageRoot/www
	cp -r PageRoot/po dist/PageRoot/po
	cp PageRoot/Gulpfile.js dist/PageRoot/
	cp PageRoot/package.json dist/PageRoot/
	cp -r DMan dist/

	echo "Installing dependencies"
	cd dist/PageRoot && \
        PATH="../../symdir/:$$PATH" npm --registry=https://r.cnpmjs.org --disturl=https://cnpmjs.org/mirrors/node install

	echo "Transpiling"
	cd dist/PageRoot && \
        PATH="../../symdir/:$$PATH" node --harmony ./node_modules/gulp/bin/gulp.js dist

nodejs:
	mkdir -p symdir
	ln -s /usr/bin/nodejs ./symdir/node

install:
	cp -r dist/ $(DESTDIR)/$(PREFIX)
	chmod +x $(DESTDIR)/$(PREFIX)/DMan/main.py


clean:
	rm -rf symdir/
	rm -rf dist/

.PHONY: all dist install clean