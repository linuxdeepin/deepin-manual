import React, { Component } from "react"
import { Link } from "react-router-dom"

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
			let [title, logo] = data
				.substr("# ".length, data.indexOf("\n"))
				.split("|")
			logo = `${path}${logo}`
			this.setState({ title, logo, file, show: true })
		})
	}
	render() {
		return (
			this.state.show && (
				<Link to={"/open/" + encodeURIComponent(this.state.file)}>
					<div className="item">
						<img
							draggable="false"
							src={this.state.logo}
							alt={this.props.appName}
						/>
						<br />
						<span lang={global.lang}>{this.state.title}</span>
					</div>
				</Link>
			)
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
			"deepin-calculator",
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
		global.qtObjects.manual.getSystemManualList(appList =>
			this.setState({ appList })
		)
	}
	shouldComponentUpdate(nextProps, nextState) {
		if (nextState.appList.toString() == this.state.appList.toString()) {
			return false
		}
		return true
	}
	render() {
		let sysSoft = ["dde"].filter(
			appName => this.state.appList.indexOf(appName) != -1
		)
		let appSoft = this.state.sequence.filter(
			appName => this.state.appList.indexOf(appName) != -1
		)
		let otherSoft = this.state.appList.filter(
			appName =>
				this.state.sequence.indexOf(appName) == -1 &&
				sysSoft.indexOf(appName) == -1
		)
		return (
			<div id="index">
				{sysSoft.length > 0 && (
					<div className="items">
						<h2>{global.i18n["System"]}</h2>
						{sysSoft.map(appName => <Item key={appName} appName={appName} />)}
					</div>
				)}
				<div className="items">
					<h2>{global.i18n["Applications"]}</h2>
					{appSoft.map(appName => <Item key={appName} appName={appName} />)}
					{otherSoft.map(appName => <Item key={appName} appName={appName} />)}
				</div>
			</div>
		)
	}
}
