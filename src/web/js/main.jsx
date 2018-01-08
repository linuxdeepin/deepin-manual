import React, { Component } from 'react'

import Nav from "./nav.jsx"
import Article from "./article.jsx"
import m2h from "./mdToHtml.js"

export default class Main extends Component {
	constructor(props){
		super(props)
		console.log(props)
		this.state={
			hash:props.hlist[0].id
		}
	}
	setHash(hash){
		this.setState({hash})
	}
	render() {
		return <div id="main">
				<Nav hlist={this.props.hlist} hash={this.state.hash} setHash={this.setHash.bind(this)}/>
				<Article html={this.props.html} hash={this.state.hash} setHash={this.setHash.bind(this)}/>
			</div>
	}
}