const
    gulp = require("gulp"),
    browserify = require('browserify'),
    fs = require("fs"),
    sass = require('gulp-sass'),
    browserSync = require('browser-sync').create()

//编译js到index.js
gulp.task('js', ()=>{
    return browserify("js/app.js")
        .transform("babelify", {presets: ["env", "react"]})
        .bundle()
        .pipe(fs.createWriteStream("index.js"))
})
//编译scss到css/
gulp.task('scss',()=>{
    return gulp.src("scss/*.scss")
        .pipe(sass())
        .pipe(gulp.dest("css"))
})
//编译全部
gulp.task('build',['js','scss'])

gulp.task('js-watch', ['js'], ()=>browserSync.reload())
gulp.task('scss-watch', ['scss'], ()=>browserSync.reload())

gulp.task('serve',['js','scss'],()=>{
    browserSync.init({
        server: "./"
    })
    gulp.watch("js/*.js*", ['js-watch'])
    gulp.watch("scss/*.scss", ['scss-watch'])
    gulp.watch("*.html").on('change', browserSync.reload)
})