import React, { Component } from 'react'
import { Scrollbars } from 'react-custom-scrollbars'

import m2h from './mdToHtml.js'

class Item extends Component {
	constructor(props){
		super(props)
		this.state={
			name:"",
			title:"",
			logo:"",
			show:false
		}
	}
	componentWillMount(){
		let xhr=new XMLHttpRequest()
		let path=`${localStorage.path}/${this.props.appName}/${localStorage.lang}/`
		xhr.open("GET",path+"index.md")
		xhr.onload=()=>{
			if(xhr.responseText==""){
				return
			}
			let m=new m2h(this.props.appName,xhr.responseText)
			let {title,logo}=m.info()
			this.setState({title,logo,show:true})
		}
		xhr.send()
	}
	render() {
		return ( this.state.show && 
			<div className="item" onClick={()=>this.props.openApp(this.props.appName)}>
				<img src={this.state.logo} alt={this.props.appName}/>
				<br/>
				<span>{this.state.title}</span>
			</div>
		)
	}
}


export default class Index extends Component {
	constructor(props){
		super(props)
		this.state={
			appList:[]
		}
	}
	componentWillMount(){
		let xhr=new XMLHttpRequest()
		xhr.onload=()=>{
			let appList=xhr.responseText.match(/addRow\("([^.][^"]+)"/g)
							.map(r => {
								return r.match(/"([^"]+)"/)[1]
							})
			this.setState({appList:appList})
		}
		xhr.open("GET",localStorage.path)
		xhr.send()
	}
	shouldComponentUpdate(nextProps,nextState){
		if(nextState.appList.toString()==this.state.appList.toString()){
			return false
		}
		return true
	}
	render() {
		let sysSoft=this.state.appList.filter(appName=>appName.indexOf('dde')!=-1)
		let appSoft=this.state.appList.filter(appName=>appName.indexOf('dde')==-1)
		return <Scrollbars>
			<div id="index">
				<h2>系统</h2>
					{ sysSoft.map(appName=><Item key={appName} appName={appName} openApp={this.props.openApp}/>) }
				<h2>应用</h2>
					{ appSoft.map(appName=><Item key={appName} appName={appName} openApp={this.props.openApp}/>) }
			</div>
		</Scrollbars>
	}
}