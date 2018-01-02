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
	http = require('http'),
	fs = require('fs'),
	path = require('path'),
	browserSync = require('browser-sync').create()
const RootDir="../../PageRoot/www/"

//编译js到index.js
gulp.task('js', ()=>{
	if(Release){
		process.env.NODE_ENV = 'production'
	}
	return browserify("js/app.js")
		.transform("babelify", {presets: ["env","react"]})
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

//浏览器调试代理
gulp.task('proxy',()=>{
	var httpServer = http.createServer((req, res) => {
		res.setHeader('Access-Control-Allow-Origin', '*')
		let path=`/usr/share/dman${req.url}`
		console.log(path)
		fs.readFile(path,(err,data)=>{
			if(err){
				fs.readdir(path,(err,files)=>{
					if(err){
						res.writeHead(404)
						res.end()
						return
					}
					if(req.url.charAt(req.url.length-1)!='/'){
						console.log("跳转")
						res.writeHead(302,{location:req.url+"/"})
						res.end()
						return
					}
					let data='<a href="..">..</a><br>'
					data+=files.map(f=>`<a href="${f}">${f}</a><br>`).join("\n")
					data+="<script>"
					data+=files.map(f=>`addRow("${f}")\n`).join("\n")
					data+="</script>"
					res.end(data,"utf-8")
				})
				return
			}
			// console.log(data)
			if(path.indexOf("svg")!=-1){
				res.setHeader("Content-Type","image/svg+xml")
			}
			res.end(data)
		})
	})
	httpServer.listen(8000,function () {
		console.log("file server listen: 0.0.0.0:8000 ...")
	})
})

//调试模式，自动刷新浏览器
gulp.task('serve',tasks,()=>{
	browserSync.init({
		server: RootDir
	})
	gulp.watch("js/*.js*", ['js-watch'])
	gulp.watch("sass/*.scss", ['sass-watch'])
	gulp.watch("*.html",['html-watch'])
	gulp.start("proxy")
})