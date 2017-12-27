const React = require("react")
module.exports=class Nav extends React.Component{
	render(){
		let hList=this.props.hList
		hList=hList.map(el=>{
			let Name=el.nodeName.toLocaleLowerCase()
			return <a key={el.id} href={`#${el.id}`} onClick={this.props.onSearchCancel}>
				<Name className={el.id==this.props.hash?"hash":undefined}>{el.innerText}</Name>
			</a>
		})
		return <div id="nav">{hList}</div>
	}
}