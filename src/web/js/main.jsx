const React = require("react"),
	Nav = require("./nav.jsx"),
	Content = require("./content.jsx")

module.exports=class Main extends React.Component{
	constructor(props){
		super(props)
		this.state={
			search:this.props.search,
		}
	}
	componentWillReceiveProps(nextProps) {
		this.setState({search: nextProps.search});
	}
	hashChange(hash){
		this.setState({hash,search:""})
	}
	render(){
		let vdiv=document.createElement("div")
		vdiv.innerHTML=this.props.html
		let hList=[...vdiv.querySelectorAll("h2,h3,h4")]
		return <div>
			<title>{this.props.title} - 帮助手册</title>
			<Nav hList={hList}
				 hash={this.state.hash}
				 onSearchCancel={()=>this.setState({search:""})}
			></Nav>
			<Content app={this.props.app}
					 vdiv={vdiv}
					 search={this.state.search}
					 hash={this.state.hash}
					 onHashChange={this.hashChange.bind(this)}
					 onSearchDone={()=>this.setState({search:""})}
			></Content>
		</div>
	}
}

