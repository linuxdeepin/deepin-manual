import React, { Component } from "react"

import Nav from "./nav.jsx"
import Article from "./article.jsx"
import m2h from "./mdToHtml.js"

export default class Main extends Component {
	constructor(props) {
		super(props)
		this.state = {
			init: false
		}
		let { file, hash } = this.props.match.params
		this.init(decodeURIComponent(file), hash ? decodeURIComponent(hash) : null)
	}
	init(file, hash) {
		if (file.indexOf("/") == -1) {
			file = `${global.path}/${file}/${global.lang}/index.md`
		}
		global.readFile(file, data => {
			let { html, hlist } = m2h(file, data)
			this.setState({
				html,
				hlist,
				init: true,
				hash: hash ? hash : hlist[0].id
			})
		})
	}
	setHash(hash) {
		this.setState({ hash })
	}
	componentWillReceiveProps(nextProps) {
		let { file, hash } = nextProps.match.params
		this.init(decodeURIComponent(file), hash ? decodeURIComponent(hash) : null)
	}
	render() {
		return (
			<div>
				{this.state.init && (
					<div id="main">
						<Nav
							hlist={this.state.hlist}
							hash={this.state.hash}
							setHash={this.setHash.bind(this)}
						/>
						<Article
							file={this.props.match.params.file}
							html={this.state.html}
							hash={this.state.hash}
							setHash={this.setHash.bind(this)}
						/>
					</div>
				)}
			</div>
		)
	}
}
