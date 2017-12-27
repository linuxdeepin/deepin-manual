const React = require("react"),
	ReactDOM = require("react-dom"),
	Search = require("./search.jsx")
module.exports=class Content extends React.Component{
	constructor(props){
		super(props)
		this.state={
			seeImage:"",
		}
	}
	componentDidMount(){
		this.setState({app:this.props.app})
		let content=ReactDOM.findDOMNode(this)
		let read=content.querySelector("#read")
		read.focus()
		this.hList=[...read.querySelectorAll("h2,h3,h4")].reverse()
		location.hash=`#${this.hList[this.hList.length-1].id}`
	}
	componentDidUpdate(){
		if(this.state.app!=this.props.app){
			this.componentDidMount()
		}
	}
	readScroll(){
		let hList=this.hList
		for(let i=0;i<hList.length;i++){
			if(hList[i].getBoundingClientRect().bottom<hList[i].clientHeight*2){
				if(hList[i].id!=this.props.hash) {
					console.log(hList[i].id)
					this.props.onHashChange(hList[i].id)
					console.log(hList[i])
				}
				return
			}
		}
	}
	click(event){
		console.log(event)
		switch(event.target.nodeName){
			case "IMG":
				let src=event.target.getAttribute("src")
				if(src.indexOf(".png")!=-1){
					this.setState({seeImage:src})
					event.preventDefault()
				}
				break
			case "A":
				console.log(event.target.href)
				break
		}
	}
	render(){
		return <div id="content" onScroll={this.readScroll.bind(this)}>
			{
				this.state.seeImage &&
				<div id="seeImage" onClick={()=>this.setState({seeImage:""})}><img src={this.state.seeImage}/></div>
			}
			{
				this.props.search &&
				<Search kw={this.props.search} vdiv={this.props.vdiv} onSearchDone={this.props.onSearchDone}></Search>
			}
			<div id="read" className="read" tabIndex="0" onClick={event=>this.click(event)} hidden={this.props.search}
				 dangerouslySetInnerHTML={{__html:this.props.vdiv.innerHTML}}>
			</div>
			<div id="fillBlank"></div>
		</div>
	}
}
