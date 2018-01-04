import React, { Component } from 'react'

import Nav from "./nav.jsx"
import Article from "./article.jsx"
import m2h from "./mdToHtml.js"

export default class Main extends Component {
	constructor(props){
		super(props)
		this.state={
			hList:[],
			html:"",
			hash:"",
		}
	}
	init(appName){
		let path=`${localStorage.path}/${appName}/${localStorage.lang}/`
		let xhr=new XMLHttpRequest()
		xhr.open("GET",path+"index.md")
		xhr.onload=()=>{
			if(xhr.responseText==""){
				return
			}
			let m=new m2h(appName,xhr.responseText)
			this.setState({hList:m.hlist(),html:m.html()})
		}
		xhr.send()
	}
	componentWillMount(){
		this.init(this.props.appName)
	}
	componentWillReceiveProps(nextProps){
		if(nextProps.appName!=this.props.appName){
			this.init(nextProps.appName)
		}
	}
	setHash(hash){
		this.setState({hash})
	}
	render() {
		return <div id="main">
				<Nav hList={this.state.hList} hash={this.state.hash} setHash={this.setHash.bind(this)}/>
				<Article html={this.state.html} hash={this.state.hash} setHash={this.setHash.bind(this)}/>
			</div>
	}
}