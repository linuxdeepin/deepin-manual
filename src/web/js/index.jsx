import React, { Component } from 'react'
import { FormattedMessage } from 'react-intl'
import { Scrollbars } from 'react-custom-scrollbars'

class Item extends Component {
	constructor(props) {
		super(props)
		this.state = {
			name: "",
			title: "",
			logo: "",
			show: false
		}
		const path = `${global.path}/${this.props.appName}/${global.lang}/`
		const file = path + `index.md`
		global.readFile(file, data => {
			let [title, logo] = data.substr("# ".length, data.indexOf("\n")).split("|")
			logo = `${path}${logo}`
			this.setState({ title, logo, show: true })
		})
	}
	render() {
		return (this.state.show &&
			<div className="item" onClick={() => global.openApp(this.props.appName)}>
				<img draggable="false" src={this.state.logo} alt={this.props.appName} />
				<br />
				<span>{this.state.title}</span>
			</div>
		)
	}
}


export default class Index extends Component {
	constructor(props) {
		super(props)
		let sequence = [
			"dde-file-manager",
			"deepin-appstore",
			"deepin-system-monitor",
			"deepin-terminal",
			"deepin-movie",
			"deepin-music",
			"deepin-image-viewer",
			"deepin-screenshot",
			"deepin-screen-recorder",
			"deepin-voice-recorder",
			"deepin-cloud-print",
			"deepin-cloud-scan",
			'deepin-calculator',
			"deepin-clone",
			"deepin-graphics-driver-manager",
			"deepin-package-manager",
			"deepin-presentation-assistant",
			"deepin-boot-maker"
		]
		this.state = {
			sequence,
			appList: []
		}

		global.readFile(global.path, data => {
			let appList = data.match(/addRow\("([^.][^"]+)"/g)
				.map(r => {
					return r.match(/"([^"]+)"/)[1]
				})
			this.setState({ appList: appList })
		})
	}
	shouldComponentUpdate(nextProps, nextState) {
		if (nextState.appList.toString() == this.state.appList.toString()) {
			return false
		}
		return true
	}
	render() {
		let sysSoft = ['dde']
		let appSoft = this.state.sequence.filter(appName => this.state.appList.indexOf(appName) != -1)
		let otherSoft = this.state.appList.filter(appName => this.state.sequence.indexOf(appName) == -1 && sysSoft.indexOf(appName) == -1)
		return <Scrollbars >
			<div id="index">
				<h2><FormattedMessage id="sys" /></h2>
				<div className="items">
					{sysSoft.map(appName => <Item key={appName} appName={appName} />)}
				</div>
				<h2><FormattedMessage id="app" /></h2>
				<div className="items">
					{appSoft.map(appName => <Item key={appName} appName={appName} />)}
					{otherSoft.map(appName => <Item key={appName} appName={appName} />)}
				</div>
			</div>
		</Scrollbars>
	}
}