import React from 'react'
import ReactDOM from 'react-dom'

import Index from "./index.jsx"
import Main from "./main.jsx"
import m2h from "./mdToHtml"

global.lang = navigator.language.replace(/-/, '_')
global.path = location.protocol == "http:" ?`http://${location.hostname}:8000`: `file:///usr/share/dman`

let openFile=file=>{
	let xhr=new XMLHttpRequest()
	xhr.open("GET",file)
	xhr.onload=()=>{
		if(xhr.responseText==""){
			return
		}
		let m=new m2h(appName,xhr.responseText)
		ReactDOM.render(<Main appName={appName} hlist={m.hlist()} html={m.html()} />,document.getElementById("app"))
	}
	xhr.send()
}
let open=appName=>{
	console.log("open",appName)
	let path=`${global.path}/${appName}/${global.lang}/`
	let xhr=new XMLHttpRequest()
	xhr.open("GET",path+"index.md")
	xhr.onload=()=>{
		if(xhr.responseText==""){
			return
		}
		let m=new m2h(appName,xhr.responseText)
		ReactDOM.render(<Main appName={appName} hlist={m.hlist()} html={m.html()} />,document.getElementById("app"))
	}
	xhr.send()
}
let index=()=>{
	console.log("index")
	ReactDOM.render(<Index openApp={open}/>,document.getElementById("app"))
}
//浏览器端自动打开浏览器
if(location.protocol == "http:"){
	index()
}

//Qt
let webChannel=null
let delay=null

let state={
	appName:"",
	searchWord:"",
}
global.openFile=file=>{
	if(window.QWebChannel && !webChannel){
		delay=()=>global.openFile(file)
		console.log("延迟执行")
		return
	}
	webChannel.objects.titleBar.setBackButtonVisible(true)
	state.appName=file
	webChannel.objects.search.setCurrentApp(file)

	let xhr=new XMLHttpRequest()
	xhr.open("GET",file)
	xhr.onload=()=>{
		if(xhr.responseText==""){
			return
		}
		let m=new m2h(file,xhr.responseText)
		ReactDOM.render(<Main appName={file} hlist={m.hlist()} html={m.html()} />,document.getElementById("app"))
	}
	xhr.send()
}
global.open=(appName)=>{
	if(window.QWebChannel && !webChannel){
		delay=()=>global.open(appName)
		console.log("延迟执行")
		return
	}
	webChannel.objects.titleBar.setBackButtonVisible(true)
	state.appName=appName
	webChannel.objects.search.setCurrentApp(appName)

	let path=`${global.path}/${appName}/${global.lang}/`
	let xhr=new XMLHttpRequest()
	xhr.open("GET",path+"index.md")
	xhr.onload=()=>{
		if(xhr.responseText==""){
			return
		}
		let m=new m2h(appName,xhr.responseText)
		ReactDOM.render(<Main appName={appName} hlist={m.hlist()} html={m.html()} />,document.getElementById("app"))
	}
	xhr.send()
}

global.index=()=>{
	if(window.QWebChannel && !webChannel){
		delay=()=>global.index()
		console.log("延迟执行")
		return
	}
	webChannel.objects.titleBar.setBackButtonVisible(false)
	ReactDOM.render(<Index openApp={global.open}/>,document.getElementById("app"))
}

if(window.QWebChannel){
	new QWebChannel(qt.webChannelTransport,channel=>{
		webChannel=channel
		global.webChannel=webChannel

		webChannel.objects.titleBar.backButtonClicked.connect(()=>{
			if(state.searchWord){
				state.searchWord=""
				return
			}
			if(state.appName){
				state.appName=""
				global.index()
			}
		})
		if(delay){
			delay()
		}
	})
}
