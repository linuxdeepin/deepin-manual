const React = require("react"),
	Nav = require("./nav.jsx"),
	Content = require("./content.jsx")

module.exports=class Main extends React.Component{
	constructor(props){
		super(props)
		this.state={
			hash:""
		}
	}
	// componentWillReceiveProps(nextProps) {
	// 	this.setState({search: nextProps.search});
	// }
	hashChange(hash){
		console.log(hash)
		this.setState({hash})
	}
	render(){
		let vdiv=document.createElement("div")
		vdiv.innerHTML=this.props.html
		return <div>
			<title>{this.props.title} - 帮助手册</title>
			<Nav hList={[...vdiv.querySelectorAll("h2,h3,h4")]}
			hash={this.state.hash}
			hashChange={hash=>this.hashChange(hash)}></Nav>
			<Content title={this.props.title}
			vdiv={vdiv} 
			hash={this.state.hash}
			search={this.props.search}
			hashChange={hash=>this.hashChange(hash)}></Content>
		</div>
	}
}

