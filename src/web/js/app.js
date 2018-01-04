import React from 'react'
import ReactDOM from 'react-dom'

import Index from "./index.jsx"
import Main from "./main.jsx"
import m2h from "./mdToHtml"


localStorage.lang = navigator.language.replace(/-/, '_')
localStorage.path = location.protocol == "http:" ?`http://${location.hostname}:8000`: `file:///usr/share/dman`

global.open=(appName)=>{
	console.log("open",appName)
	ReactDOM.render(<Main appName={appName}/>,document.getElementById("app"))
}
global.index=()=>{
	console.log("index")
	ReactDOM.render(<Index openApp={global.open}/>,document.getElementById("app"))
}
// global.index()
// global.open("dde")