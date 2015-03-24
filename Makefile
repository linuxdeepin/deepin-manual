PREFIX := /opt/deepin-user-manual

all:
	mkdir -p dist
	mkdir -p dist/PageRoot

	cp -r PageRoot/www dist/PageRoot/www
	cp -r PageRoot/po dist/PageRoot/po
	cp PageRoot/Gulpfile.js dist/PageRoot/
	cp PageRoot/package.json dist/PageRoot/
	cp -r DMan dist/

	echo "Installing dependencies"
	cd dist/PageRoot && \
        npm --registry=https://r.cnpmjs.org --disturl=https://cnpmjs.org/mirrors/node install

	echo "Transpiling"
	cd dist/PageRoot && \
        node --harmony ./node_modules/gulp/bin/gulp.js dist

install:
	cp -r dist/ $(DESTDIR)/$(PREFIX)
	chmod +x $(DESTDIR)/$(PREFIX)/DMan/main.py


clean:
	rm -rf dist/

.PHONY: all dist install clean