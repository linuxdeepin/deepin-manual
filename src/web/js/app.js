const React = require("react"),
	ReactDOM = require("react-dom"),
	Marked = require("marked"),
	md5 = require('md5'),
	Main = require("./main.jsx")

function Index(props) {
	return <div id="index">
			{props.appLists.map(list=>
				<div key={list.name}>
					<h2>{list.name}</h2>
					{list.child.map(app=>
						<div key={app.name} className="app" onClick={e=>renderApp(app.name)}>
							<img src={`${arge.origin}/${app.name}/common/${app.logo}`}/>
							<br/>
							{app.title}
						</div>
					)}
				</div>
			)}
		</div>
}
//渲染App组件
function renderApp(appName,search) {
	arge.appName=appName
	let appInfo=JSON.parse(localStorage[appName+"_info"])
	ReactDOM.render(<Main html={localStorage[appName+"_vdiv"]} app={appName} search={search?search:""} title={appInfo.title}></Main>,
		document.getElementById("app"))
}
//初始化参数
let arge= {
	lang : navigator.language.replace(/-/, '_'),
	origin : location.protocol == "http:" ?`http://${location.hostname}:8000`: `file:///usr/share/dman`
}
//首页
global.index=function (fromCacheAll) {
	if(!localStorage.appList) {
		cacheAll()
		return
	}
	let appList=JSON.parse(localStorage.appList)
		.filter(appNmae=>localStorage[appNmae+"_info"])
		.map(appNmae=>JSON.parse(localStorage[appNmae+"_info"]))
	let appLists=[
		{
			name:"系统",
			child:appList.filter(app=>app.name.indexOf("dde")!=-1)
		},
		{
			name:"应用",
			child:appList.filter(app=>app.name.indexOf("dde")==-1)
		}
	]
	ReactDOM.render(<Index appLists={appLists}></Index>,document.getElementById("app"))
	if(!fromCacheAll){
		cacheAll()
	}
}
//打开App帮助
global.open=function (appName) {
	if(localStorage[appName+"_vdiv"]){
		renderApp(appName)
	}else{
		cache(appName,()=>renderApp(appName))
	}
}
global.search=function (kw) {
	renderApp(arge.appName,kw)
}
//读取文件
function read(path,callback) {
	const xhr = new XMLHttpRequest()
	xhr.open("GET", path)
	xhr.onload = () => callback(xhr)
	xhr.onerror = () => callback(xhr)
	xhr.send()
}
//缓存已解析app
function cache(appName,callback) {
	read(`${arge.origin}/${appName}/${arge.lang}/index.md`,xhr=>{
		let data=xhr.responseText
		if(data==""){
			console.log(appName,xhr.statusText)
			return
		}
		let m=md5(data)
		if(localStorage[appName+"_hash"]==m) {
			console.log(appName,"cache")
			return
		}
		console.log(appName,"update")
		let vdiv = document.createElement("div")
		vdiv.innerHTML = Marked(data).replace(/src="/g, `$&${arge.origin}/${appName}/${arge.lang}/`)
		let [title, logo] = vdiv.querySelector("h1").innerText.split("|")
		vdiv.removeChild(vdiv.querySelector("h1"))
		;[...vdiv.querySelectorAll("h2")].map(el => el.innerText = el.innerText.split("|")[0])
		let ids = {}
		;[...vdiv.querySelectorAll("h2,h3,h4")].map(el => {
			let text = el.innerText
			if (ids[text] == undefined) {
				el.id = text
				ids[text] = 0
			} else {
				el.id = `${text}-${ids[text]++}`
			}
		})
		localStorage[appName + "_hash"] = m
		localStorage[appName + "_vdiv"] = vdiv.innerHTML
		localStorage[appName + "_info"] = JSON.stringify({name: appName, title, logo})
		if(callback){
			callback()
		}
	})
}
//缓存所有app
function cacheAll() {
	read(arge.origin,xhr=>{
		let appList = xhr.responseText.match(/addRow\("([^.][^"]+)"/g)
			.map(r => {
				return r.match(/"([^"]+)"/)[1]
			})
		localStorage["appList"]=JSON.stringify(appList)
		appList.map(appName=>cache(appName,()=>index(true)))
	})
}

if(location.search){
	open(location.search.slice(1))
}else{
	index()
}