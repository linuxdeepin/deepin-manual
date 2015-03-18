"use strict";

var gulp = require('gulp');
var sass = require('gulp-sass');
var gettext = require("gulp-angular-gettext");
var mocha = require('gulp-mocha');
var sourcemaps = require('gulp-sourcemaps');
var traceur = require('gulp-traceur');

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

gulp.task("watch", ['sass'], function() {
    gulp.watch('./www/scss/*.scss', ['sass']);
});

gulp.task('test', function () {
    return gulp.src('./spec/*_spec.js', {
            read: false,
        })
        .pipe(mocha({
            reporter: 'nyan',
        }));
});

var browserify = require("browserify");
var source = require('vinyl-source-stream');

gulp.task('browserify', function() {
    return browserify('./www/jssrc/entry.js')
        .bundle()
        //Pass desired output filename to vinyl-source-stream

        .pipe(source('bundle.js'))
        // Start piping stream to tasks!
        .pipe(gulp.dest('./www/scripts/'));
});

gulp.task('es6', function() {
    return gulp.src('./www/jssrc/**/*.js')
        .pipe(sourcemaps.init())
        .pipe(traceur())
        .pipe(sourcemaps.write('.'))
        .pipe(gulp.dest('./www/scripts/'));
});
