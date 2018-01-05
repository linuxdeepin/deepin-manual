import React, { Component } from 'react'
import ReactDOM from 'react-dom'
import { Scrollbars } from 'react-custom-scrollbars'

export default class Article extends Component {
	constructor(props){
		super(props)
		this.state={
			seeImg:""
		}
		this.hash=this.props.hash
	}
	componentDidUpdate(){
		if(this.hash!=this.props.hash){
			this.hash=this.props.hash
			console.log(this.hash,ReactDOM.findDOMNode(this))
			let hashDOM=document.getElementById(this.hash)
			if(hashDOM){
				this.scrollbars.scrollTop(this.scrollbars.getScrollTop()+hashDOM.getBoundingClientRect().top)
			}
		}
	}
	scroll(){
		let hList=ReactDOM.findDOMNode(this).querySelectorAll("h2,h3")
		let hash=hList[0].id
		for(let i=0;i<hList.length;i++){
			if(hList[i].getBoundingClientRect().top>1){
				break
			}
			hash=hList[i].id
		}
		if(this.hash!=hash){
			console.log("hash update")
			this.hash=hash
			this.props.setHash(hash)
		}
	}
	click(e){
		switch(e.target.nodeName){
			case "IMG":
				e.preventDefault()
				let src=e.target.getAttribute("src")
				if(src.indexOf("svg")==-1){
					console.log("图片全屏查看")
					this.setState({seeImg:src})
				}
			break
		}
	}
	render() {
		return <div id="article">
					<Scrollbars onScroll={e=>this.scroll(e)} ref={s => { this.scrollbars = s }}>
						<div id="read" onClick={this.click.bind(this)} dangerouslySetInnerHTML={{__html:this.props.html}}></div>
						<div id="fillblank" />
					</Scrollbars>
					{
						this.state.seeImg!="" && 
						<div id="seeImg" onClick={()=>this.setState({seeImg:""})}>
							<img src={this.state.seeImg} />
						</div>
					}
				</div>
	}
}
