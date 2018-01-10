import React from 'react'
import ReactDOM from 'react-dom'
import { addLocaleData, IntlProvider } from 'react-intl'
import zh from 'react-intl/locale-data/zh'
import en from 'react-intl/locale-data/en'
addLocaleData([...zh, ...en])

import zh_CN from './zh_CN.js'
import en_US from './en_US.js'

import Index from "./index.jsx"
import Main from "./main.jsx"
import m2h from "./mdToHtml"
import sIndex from "./searchIndex"

global.lang = navigator.language.replace("-", "_")
global.path = "/usr/share/dman"
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
	searchWord: "",
	appName: "",
}
function stateBack() {
	console.log("stateBack", state)
	switch (true) {
		case state.searchWord != "":
			break
		case state.appName != "":
			index()
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
		let {html, hlist} = m2h(file, data)
		ReactDOM.render(<Main appName={file} hlist={hlist} html={html} />, document.body)
		sIndex(file, null, html)
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
	ReactDOM.render(<IntlProvider locale={navigator.language} messages={{ zh_CN, en_US }[global.lang]}><Index /></IntlProvider>, document.body)
}
function searchIndexCheck() {
	global.readFile(global.path, data => {
		let appList = data.match(/addRow\("([^.][^"]+)"/g)
			.map(r => {
				return r.match(/"([^"]+)"/)[1]
			})
		appList.map(appName => {
			const file = `${global.path}/${this.props.appName}/${global.lang}/index.md`
			global.readFile(file, data => sIndex(file, data))
		})
	})
}
function qtInit(channel) {
	global.qtObjects = channel.objects
	global.qtObjects.titleBar.backButtonClicked.connect(stateBack)
	if (delay != null) {
		delay()
	}
	searchIndexCheck()
	console.log(global.qtObjects)
}
new QWebChannel(qt.webChannelTransport, qtInit)