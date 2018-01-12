const gulp = require("gulp"),
	sass = require("gulp-sass"),
	concat = require("gulp-concat"),
	rename = require("gulp-rename"),
	source = require("vinyl-source-stream"),
	buffer = require("vinyl-buffer"),
	browserify = require("browserify")

const RootDir = "../../PageRoot/www/"

//编译js到index.js
gulp.task("js", () => {
	// process.env.NODE_ENV = 'production'
	return browserify("js/App.js")
		.transform("babelify", { presets: ["env", "react"] })
		.bundle()
		.pipe(source("bundle.js"))
		.pipe(buffer())
		.pipe(rename("index.js"))
		.pipe(gulp.dest(RootDir))
})
//编译scss到css/
gulp.task("sass", () => {
	return gulp
		.src("sass/*.scss")
		.pipe(sass())
		.pipe(concat("index.css"))
		.pipe(gulp.dest(RootDir))
})
//移动html文件
gulp.task("html", () => {
	return gulp.src("*.html").pipe(gulp.dest(RootDir))
})

const tasks = ["js", "sass", "html"]

//编译全部
gulp.task("build", tasks)

//调试模式，自动刷新浏览器
gulp.task("serve", tasks, () => {
	gulp.watch("js/*.js*", ["js"])
	gulp.watch("sass/*.scss", ["sass"])
	gulp.watch("*.html", ["html"])
})
