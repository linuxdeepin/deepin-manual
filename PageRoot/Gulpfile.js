"use strict";

var gulp = require('gulp');
var rename = require('gulp-rename');
var sass;
try {
    sass = require('gulp-sass');
} catch(e) {
    sass = null;
}
var gettext = require("gulp-angular-gettext");
var mocha = require('gulp-mocha');
var fs = require("fs");
var babelify = require("babelify");
var browserify = require("browserify");
var ngannotate = require("gulp-ng-annotate");
var uglify = require("gulp-uglify");

gulp.task('sass', function () {
    if (!sass) {
        console.error("You must install gulp-sass!");
    }
    gulp.src('./www/scss/*.scss')
        .pipe(sass({
            errLogToConsole: true,
        }))
        .pipe(gulp.dest('./www/style'));
});

gulp.task('pot', function () {
    return gulp.src([
            'www/index.html',
            'www/partials/**/*.html',
            'www/**/*.js',
        ])
        .pipe(gettext.extract('template.pot', {
            // options to pass to angular-gettext-tools...
        }))
        .pipe(gulp.dest('po/'));
});

gulp.task('translations', function () {
    return gulp.src('po/**/*.po')
        .pipe(gettext.compile({
            // options to pass to angular-gettext-tools...
            format: 'json',
        }))
        .pipe(gulp.dest('./www/nls/'));
});

gulp.task('copyMouseTrap', function() {
    return gulp.src("./node_modules/mousetrap/mousetrap.min.js")
            .pipe(rename('mousetrap.js'))
            .pipe(gulp.dest('./www/scripts/'));
});

var entryPath = './www/jssrc/entry.js';
var jsPath = './www/jssrc/**/*.js';
var bundlePath = './www/scripts/bundle.js';

gulp.task("watch", ['sass', 'copyMouseTrap', 'browserify'], function() {
    gulp.watch('./www/scss/*.scss', ['sass']);
    gulp.watch(jsPath, ['browserify']);
});

gulp.task('test', function () {
    return gulp.src('./spec/*_spec.js', {
            read: false,
        })
        .pipe(mocha({
            reporter: 'nyan',
        }));
});

var browserifyDone = false;
gulp.task('browserify', function() {
    browserifyDone = false;
    // transform(babelify) and then transform(browserify-ngannotate) loses sourcemap
    // bind a finish event, to use gulp-ng-annotate instead.
    var fsWriter = fs.createWriteStream(bundlePath);
    fsWriter.on("finish", function() {
        return gulp.src(bundlePath)
            .pipe(ngannotate())
            // Do not really minify it.
            // uglifying is slow, and removes sourcemaps.
            // minifying is really done in dist task.
            //.pipe(uglify())
            .pipe(rename('bundle.min.js'))
            .pipe(gulp.dest("./www/scripts/"))
            .on("finish", function() {
                browserifyDone = true;
            });
    });
    browserify({ debug: true })
        .transform(babelify)
        .require(entryPath, { entry: true })
        .bundle()
        .on("error", function (err) {
            console.error(err.message);
        })
        .pipe(fsWriter);
});

gulp.task('dist', ['copyMouseTrap', 'browserify', 'translations'], function() {
    var timer = setInterval(function() {
        if (!browserifyDone) {
            return;
        }
        clearTimeout(timer);
        console.log("Minifying bundle.js");
        gulp.src("./www/scripts/bundle.min.js")
            .pipe(uglify())
            .pipe(rename("bundle.min.js"))
            .pipe(gulp.dest("./www/scripts/"))
            .on("finish", function() {
                console.log("Gulp::dist done.");
            });
    }, 200);
});
