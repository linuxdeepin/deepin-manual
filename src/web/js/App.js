import React, { Component } from "react"
import ReactDOM from "react-dom"

import Index from "./index.jsx"
import Main from "./main.jsx"
import Search from "./search.jsx"

import m2h from "./mdToHtml"
import sIndex from "./searchIndex"

class App extends Component {
	constructor(props) {
		super(props)
		let { searchWord = "", appName = "" } = props
		this.state = { searchWord, appName, searchResult: [] }

		// this.state.searchWord = "test"
		// this.state.searchResult = [
		// 	{
		// 		file: "file:///usr/share/dman/dde-file-manager/zh_CN/index.md",
		// 		keys: ["概述", "压缩文件(夹)"],
		// 		values: [
		// 			"以及一系列深度特色应用。它既能让您体验到丰富多彩的娱乐生活，也可以满足您的日常工作需要。相信随着功能的不断升级和完善，深度操作系统将会被越来越多的用户所喜爱和使用。",
		// 			"您可以通过快速进入控制中心设置显示器的分辨率、亮度。"
		// 		]
		// 	},
		// 	{
		// 		file: "file:///tmp/dde/en_US/index.md",
		// 		keys: ["Dock", "设置显示器"],
		// 		values: [
		// 			"avsdfsdfsdfsdfsdfsdf avsdfsdfsdfsdfsdfsdf avsdfsdfsdfsdfsdfsdf",
		// 			"您可以通过快速进入控制中心设置显示器的分辨率、亮度。"
		// 		]
		// 	}
		// ]

		global.qtObjects.titleBar.backButtonClicked.connect(
			this.backButtonClicked.bind(this)
		)
		global.qtObjects.search.mismatch.connect(() =>
			this.setState({ searchResult: null })
		)
		global.qtObjects.search.onContentResult.connect(
			this.onContentResult.bind(this)
		)
	}
	componentWillReceiveProps(nextProps) {
		let { searchWord = "", appName = "" } = nextProps
		this.setState({ searchWord, appName, searchResult: [] })
	}
	backButtonClicked() {
		let { searchWord, appName } = this.state
		if (searchWord != "") {
			searchWord = ""
		} else if (appName != "") {
			appName = ""
		}
		this.setState({ searchWord, appName })
	}
	onContentResult(appName, keys, values) {
		this.setState({
			searchResult: [...this.state.searchResult, { appName, keys, values }]
		})
	}
	render() {
		console.log(this.state)
		let c = null
		switch (true) {
			case this.state.searchWord != "":
				c = (
					<Search
						kw={this.state.searchWord}
						searchResult={this.state.searchResult}
					/>
				)
				break
			case this.state.appName != "":
				c = (
					<Main
						file={this.state.file}
						hlist={this.props.appData.hlist}
						hash={this.props.appData.hash}
						html={this.props.appData.html}
					/>
				)
				break
			default:
				c = <Index />
		}
		console.log(c)
		return c
	}
}

global.lang = navigator.language.replace("-", "_")
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
global.openSearchPage = keyword => {
	ReactDOM.render(<App searchWord={keyword} />, document.body)
}
global.qtObjects = null
let delay = null

global.openFile = (file, hash) => {
	console.log("Open", file, hash)
	if (global.qtObjects == null) {
		delay = () => global.openFile(file)
		return
	}
	global.qtObjects.titleBar.setBackButtonVisible(true)
	global.qtObjects.search.setCurrentApp(file)
	global.readFile(file, data => {
		let { html, hlist } = m2h(file, data)
		ReactDOM.render(
			<App appName={file} appData={{ hlist, html, hash }} />,
			document.body
		)
		sIndex(file, null, html)
	})
}
global.openApp = appName => {
	if (global.qtObjects == null) {
		delay = () => global.openApp(appName)
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
	global.qtObjects.search.setCurrentApp("")
	ReactDOM.render(<App />, document.body)
}

function searchIndexCheck() {
	global.readFile(global.path, data => {
		let appList = data.match(/addRow\("([^.][^"]+)"/g).map(r => {
			return r.match(/"([^"]+)"/)[1]
		})
		appList.map(appName => {
			const file = `${global.path}/${appName}/${global.lang}/index.md`
			global.readFile(file, data => sIndex(file, data))
		})
	})
}
function qtInit(channel) {
	channel.objects.i18n.getSentences(i18n => {
		global.i18n = i18n
		global.qtObjects = channel.objects
		if (delay != null) {
			delay()
		}
	})
	searchIndexCheck()
	console.log(global.qtObjects)
}
new QWebChannel(qt.webChannelTransport, qtInit)
