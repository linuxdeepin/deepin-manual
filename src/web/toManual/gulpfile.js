// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

const gulp = require('gulp'),
  sass = require('gulp-sass'),
  concat = require('gulp-concat'),
  rename = require('gulp-rename'),
  source = require('vinyl-source-stream'),
  buffer = require('vinyl-buffer'),
  browserify = require('browserify');

let argv = require('yargs').argv;
let RootDir = '../../web_dist/toManual/';

//编译js到index.js
gulp.task('js', () => {
  return browserify('js/App.js')
    .transform('babelify', { presets: ['env', 'react'] })
    .bundle()
    .pipe(source('bundle.js'))
    .pipe(buffer())
    .pipe(rename('index.js'))
    .pipe(gulp.dest(RootDir));
});
//编译scss到css/
gulp.task('sass', () => {
  return gulp
    .src('sass/*.scss')
    .pipe(sass())
    .pipe(concat('index.css'))
    .pipe(gulp.dest(RootDir));
});
//移动html文件
gulp.task('html', () => {
  return gulp.src(['*.html', 'qwebchannel.js']).pipe(gulp.dest(RootDir));
});

const tasks = ['js', 'sass', 'html'];

//编译全部
gulp.task('build', () => {

  gulp.start(tasks);
  // if (argv.d) {
  //   RootDir = argv.d;
  //   gulp.start(tasks);
  // } else {
  //   console.error('gulp build -d DIR');
  // }
});
//调试模式，自动编译
gulp.task('serve', tasks, () => {
  gulp.watch('js/*.js*', ['js']);
  gulp.watch('sass/*.scss', ['sass']);
  gulp.watch('*.html', ['html']);
});
