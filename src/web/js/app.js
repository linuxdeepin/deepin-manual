import React from 'react'
import ReactDOM from 'react-dom'

import Index from "./index.jsx"
import Main from "./main.jsx"
import m2h from "./mdToHtml"

localStorage.lang = navigator.language.replace(/-/, '_')
localStorage.path = location.protocol == "http:" ?`http://${location.hostname}:8000`: `file:///usr/share/dman`

let open=appName=>{
	console.log("open",appName)
	ReactDOM.render(<Main appName={appName}/>,document.getElementById("app"))
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

global.open=(appName)=>{
	if(window.QWebChannel && !webChannel){
		delay=()=>global.open(appName)
		console.log("延迟执行")
		return
	}
	webChannel.objects.titleBar.setBackButtonVisible(true)
	state.appName=appName
	webChannel.objects.search.setCurrentApp(appName)
	ReactDOM.render(<Main appName={appName}/>,document.getElementById("app"))
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
