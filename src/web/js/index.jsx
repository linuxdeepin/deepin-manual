import React, { Component } from 'react'
import { Scrollbars } from 'react-custom-scrollbars'

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
		const storageKey=this.props.appName+"_info"
		if(localStorage[storageKey]){
			let appInfo=JSON.parse(localStorage[storageKey])
			appInfo.show=true
			this.setState(appInfo)
		}else{
			let xhr=new XMLHttpRequest()
			let path=`${localStorage.path}/${this.props.appName}/${localStorage.lang}/`
			xhr.open("GET",path+"index.md")
			xhr.onload=()=>{
				if(xhr.responseText==""){
					return
				}
				let infoString=xhr.responseText.slice(0,xhr.responseText.indexOf("\n"))
				let [title,logo]=infoString.slice(2).split("|")
				logo=`${path}${logo}`
				localStorage[storageKey]=JSON.stringify({title,logo,name:this.props.appName})
				this.setState({title,logo,show:true})
			}
			xhr.send()
		}
	}
	render() {
		return ( this.state.show && 
			<div className="item" onClick={()=>this.props.openApp(this.props.appName)}>
				<img src={this.state.logo} alt={this.props.appName}/>
				<h3>{this.state.title}</h3>
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
		if(localStorage.appList){
			let appList=JSON.parse(localStorage.appList)
			this.setState(appList)
		}
		let xhr=new XMLHttpRequest()
		xhr.onload=()=>{
			let appList=xhr.responseText.match(/addRow\("([^.][^"]+)"/g)
							.map(r => {
								return r.match(/"([^"]+)"/)[1]
							})
			localStorage.appList=JSON.stringify(appList)
			this.setState({appList:JSON.parse(localStorage.appList)})
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