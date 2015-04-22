"use strict";

var gulp = require('gulp');
var rename = require('gulp-rename');
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

gulp.task('dist', ['sass', 'copyMouseTrap', 'browserify', 'translations'], function() {
    console.log("Gulp::dist done.");
});
