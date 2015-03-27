"use strict";

var gulp = require('gulp');
var sass = require('gulp-sass');
var gettext = require("gulp-angular-gettext");
var mocha = require('gulp-mocha');
var fs = require("fs");
var babelify = require("babelify");
var browserify = require("browserify");


gulp.task('sass', function () {
    gulp.src('./www/scss/*.scss')
        .pipe(sass({
            errLogToConsole: true,
        }))
        .pipe(gulp.dest('./www/style'));
});

gulp.task('pot', function () {
    return gulp.src(['www/partials/**/*.html', 'www/**/*.js'])
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
        .pipe(gulp.dest('translations/'));
});

var entryPath = './www/jssrc/entry.js';
var jsPath = './www/jssrc/**/*.js';
var adapterPath = './www/jssrc/adapter.js';

gulp.task("watch", ['sass', 'browserify', 'adapter'], function() {
    gulp.watch('./www/scss/*.scss', ['sass']);
    gulp.watch(jsPath, ['browserify']);
    gulp.watch(adapterPath, ['adapter']);
});

gulp.task('test', function () {
    return gulp.src('./spec/*_spec.js', {
            read: false,
        })
        .pipe(mocha({
            reporter: 'nyan',
        }));
});

gulp.task('browserify', function() {
    browserify({ debug: true })
        .transform(babelify)
        .require(entryPath, { entry: true })
        .bundle()
        .on("error", function (err) {
            console.error(err.message);
        })
        .pipe(fs.createWriteStream("./www/scripts/bundle.js"));
});

gulp.task('adapter', function() {
    gulp.src([
        adapterPath,
    ]).pipe(gulp.dest('./www/scripts/'));
});

gulp.task('dist', ['sass', 'browserify', 'adapter'], function() {
    console.log("Gulp::dist done.");
});
