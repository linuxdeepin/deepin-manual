const
    gulp = require("gulp"),
    sass = require('gulp-sass'),
    concat = require('gulp-concat'),
    uglify = require('gulp-uglify'),
    rename = require('gulp-rename'),
    cleanCSS = require('gulp-clean-css'),
    gulpIf = require('gulp-if'),
    source = require('vinyl-source-stream'),
    buffer = require('vinyl-buffer'),
    browserify = require('browserify'),
    browserSync = require('browser-sync').create()

const RootDir="../../PageRoot/www/"

//编译js到index.js
gulp.task('js', ()=>{
    return browserify("js/app.js")
        .transform("babelify", {presets: ["env", "react"]})
        .bundle()
        .pipe(source('bundle.js'))
        .pipe(buffer())
        .pipe(gulpIf(Release,uglify()))
        .pipe(rename("index.js"))
        .pipe(gulp.dest(RootDir))
})
//编译scss到css/
gulp.task('sass',()=>{
    return gulp.src("sass/*.scss")
        .pipe(sass())
        .pipe(concat("index.css"))
        .pipe(gulpIf(Release,cleanCSS()))
        .pipe(gulp.dest(RootDir))
})
//移动html文件
gulp.task('html',()=>{
    return gulp.src("*.html")
        .pipe(gulp.dest(RootDir))
})
const tasks=['js','sass','html']

//编译全部
const Release=process.argv[2]=="build"
gulp.task('build',tasks)

//监视任务
tasks.map(task=>gulp.task(task+'-watch', [task], ()=>browserSync.reload()))

//调试模式，自动刷新浏览器
gulp.task('serve',tasks,()=>{
    browserSync.init({
        server: RootDir
    })
    gulp.watch("js/*.js*", ['js-watch'])
    gulp.watch("sass/*.scss", ['sass-watch'])
    gulp.watch("*.html",['html-watch'])
})