import React, { Component } from 'react'
import marked from 'marked'
import md5 from 'md5'

import Nav from './nav.jsx'
import Article from './article.jsx'

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
		if(localStorage[appName+"_md5"]){
			let newState={
				hList:JSON.parse(localStorage[appName+"_hlist"]),
				html:localStorage[appName+"_div"]
			}
			newState.hash=newState.hList[0].id
			this.setState(newState)
		}

		let path=`${localStorage.path}/${appName}/${localStorage.lang}/`
		let xhr=new XMLHttpRequest()
		xhr.open("GET",path+"index.md")
		xhr.onload=()=>{
			let md5Hash=md5(xhr.responseText)
			if(xhr.responseText && localStorage[appName+"_div"] && 
				localStorage[appName+"_md5"]==md5Hash
			){
				console.log(appName,"cache")
				return
			}
			console.log(appName,"update")
			localStorage[appName+"_md5"]=md5Hash

			let div=document.createElement("div")
			div.innerHTML=marked(xhr.responseText).replace(/src="/g, `$&${path}`)
			let [title,logo]=div.querySelector("h1").innerText.split("|")
			logo=`${path}${logo}`
			localStorage[appName+"_info"]=JSON.stringify({title,logo,name:appName})
			div.removeChild(div.querySelector("h1"))
			;[...div.querySelectorAll("h2")].map(el => el.innerText = el.innerText.split("|")[0])
			let ids = {}
			let hList=[...div.querySelectorAll("h2,h3,h4")].map(el => {
				let text = el.innerText
				if (ids[text] == undefined) {
					el.id = text
					ids[text] = 0
				} else {
					el.id = `${text}-${ids[text]++}`
				}
				return {id:el.id,text,type:el.nodeName.toLocaleLowerCase()}
			})
			localStorage[appName+"_div"]=div.innerHTML
			localStorage[appName+"_hlist"]=JSON.stringify(hList)
			this.setState({hList,html:div.innerHTML})
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