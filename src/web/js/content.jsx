const React = require("react"),
	ReactDOM = require("react-dom"),
	Search = require("./search.jsx")

import { Scrollbars } from 'react-custom-scrollbars'

module.exports=class Content extends React.Component{
	constructor(props){
		super(props)
		this.state={
			seeImage:"",
			search:"",
		}
	}
	componentWillReceiveProps(nextProps){
		if(nextProps.title!=this.props.title){
			this.hList=null
		}
		if(nextProps.hash!=this.hash){
			this.hash=null
		}
		if(nextProps.search!=this.props.search){
			this.setState({search:nextProps.search})
		}
	}
	componentDidMount(){
		this.hList=[...ReactDOM.findDOMNode(this).querySelectorAll("h2,h3,h4")]
		this.hash=this.hList[0].id
		this.props.hashChange(this.hash)
	}
	componentDidUpdate(){
		if(!this.hList){
			this.hList=[...ReactDOM.findDOMNode(this).querySelectorAll("h2,h3,h4")]
			this.scroll.scrollToTop()
			this.hash=this.hList[this.hList.length-1].id
			console.log(this.hash)
		}
		if(!this.hash){
			this.hash=this.props.hash
			console.log(document.getElementById(this.hash),this.hash)
			let top=document.getElementById(this.hash).getBoundingClientRect().top
			this.scroll.scrollTop(this.scroll.getScrollTop()+top)
		}
	}
	onClick(event){
		event.preventDefault()

		console.log(event.target)
		switch(event.target.nodeName){
			case "IMG":
				let src=event.target.getAttribute("src")
				if(src.indexOf(".png")!=-1){
					this.setState({seeImage:src})
				}
				break
			case "A":
				window.open(event.target.href)
				break
		}
	}
	onScroll(event){
		let top=this.scroll.getScrollTop()
		let height=this.scroll.getScrollHeight()
		let hash
		if(top<height/2){
			hash=this.hList[0].id
			for(let i=0;i<this.hList.length;i++){
				if(this.hList[i].getBoundingClientRect().top>1){
					break
				}
				hash=this.hList[i].id
			}
		}else{
			hash=this.hList[this.hList.length-1].id
			for(let i=this.hList.length-1;i>=0;i--){
				hash=this.hList[i].id
				if(this.hList[i].getBoundingClientRect().top<1){
					break
				}
			}
		}
		if(hash!=this.hash){
			this.hash=hash
			this.props.hashChange(this.hash)
		}
	}
	onSearchDone(hid){
		this.setState({
			search:""
		})
		if(hid){
			this.props.hashChange(hid)
		}
	}
	render(){
		return <div id="content">
				{
					this.state.seeImage &&
						<div id="seeImage" onClick={()=>this.setState({seeImage:""})}>
								<img src={this.state.seeImage}/>
						</div>
				}
				{
					this.state.search &&
					<Search kw={this.state.search}
							vdiv={this.props.vdiv}
							SearchDone={hid=>this.onSearchDone(hid)}></Search>
				}
				<Scrollbars ref={(scroll)=>this.scroll=scroll}
							autoHide autoHideTimeout={500}
							hidden={this.state.search||this.state.seeImage}
							onScroll={()=>this.onScroll()}>
					<div id="read" className="read" tabIndex="0" onClick={event=>this.onClick(event)}
						dangerouslySetInnerHTML={{__html:this.props.vdiv.innerHTML}}>
					</div>
					<div id="fillBlank"></div>
				</Scrollbars>
			</div>
	}
}
