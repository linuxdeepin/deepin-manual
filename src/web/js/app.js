import React from 'react'
import ReactDOM from 'react-dom'

import Index from "./index.jsx"
import Main from "./main.jsx"
import m2h from "./mdToHtml"


localStorage.lang = navigator.language.replace(/-/, '_')
localStorage.path = location.protocol == "http:" ?`http://${location.hostname}:8000`: `file:///usr/share/dman`

global.webChannel=null
let delay=null

global.open=(appName)=>{
	if(window.QWebChannel && !global.webChannel){
		delay=()=>global.open(appName)
		return
	}
	console.log("open",appName)
	ReactDOM.render(<Main appName={appName}/>,document.getElementById("app"))
}


global.index=()=>{
	if(window.QWebChannel && !global.webChannel){
		delay=()=>global.index()
		return
	}
	console.log(global.webChannel)
	console.log("index")
	ReactDOM.render(<Index openApp={global.open}/>,document.getElementById("app"))
}

if(window.QWebChannel){
	new QWebChannel(qt.webChannelTransport,channel=>{
		global.webChannel=channel
		if(delay){
			delay()
		}
	})
}

if(location.protocol == "http:"){
	global.index()
}

global.updateSearchIndex=(appName,searchIndex)=>{
	localStorage[appName+"_searchIndex"]=searchIndex
}
