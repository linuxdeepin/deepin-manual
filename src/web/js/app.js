import React from 'react'
import ReactDOM from 'react-dom'

import Index from "./index.jsx"
import Main from "./main.jsx"
import m2h from "./mdToHtml"

global.lang = navigator.language.replace(/-/, '_')
global.path = getSystemManualDir("")
global.readFile = (fileName, callback) => {
	let xhr = new XMLHttpRequest()
	xhr.open("GET", fileName)
	xhr.onload = () => {
		if (xhr.responseText != "") {
			callback(xhr.responseText)
		}
	}
	xhr.send()
}

let state = {
	appName: "",
	searchWord: "",
}
function stateBack() {
	switch (true) {
		case state.searchWord != "":
			break
		case state.appName != "":
			break
	}
}

global.qtObjects = null
let delay = null
global.openFile = file => {
	if (global.qtObjects == null) {
		delay = () => global.openFile(file)
		return
	}
	global.qtObjects.titleBar.setBackButtonVisible(true)
	global.qtObjects.search.setCurrentApp(file)
	state.appName = file

	global.readFile(file, data => {
		let m = new m2h(file, data)
		ReactDOM.render(<Main appName={file} hlist={m.hlist()} html={m.html()} />, document.getElementById("app"))
	})
}
global.openFolder = folder => {
	if (global.qtObjects == null) {
		delay = () => global.openFolder(folder)
		return
	}
	let file = `${folder}/${global.lang}/index.md`
	global.openFile(file)
}
global.openApp = (appName) => {
	if (global.qtObjects == null) {
		delay = () => global.open(appName)
		return
	}
	let file = `${global.path}/${appName}/${global.lang}/index.md`
	global.openFile(file)
}

global.index = () => {
	if (global.qtObjects == null) {
		delay = () => global.index()
		return
	}
	global.qtObjects.titleBar.setBackButtonVisible(false)
	state.appName = ""
	global.qtObjects.search.setCurrentApp("")
	ReactDOM.render(<Index />, document.getElementById("app"))
}

function qtInit(channel) {
	global.qtObjects = channel.objects
	global.qtObjects.titleBar.backButtonClicked.connect(stateBack)
	if (delay != null) {
		delay()
	}
}
new QWebChannel(qt.webChannelTransport, qtInit)