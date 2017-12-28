const React = require("react")
import { Scrollbars } from 'react-custom-scrollbars'
module.exports=class Nav extends React.Component{
	componentDidUpdate(){
		let nav=document.getElementById("nav")
		let hashDOM=document.querySelector(`[hid="${this.props.hash}"`)
		let rect=hashDOM.getBoundingClientRect()
		if(rect.top<0){
			this.scroll.scrollTop(this.scroll.getScrollTop()+rect.top)
		}else if(rect.bottom>nav.offsetHeight){
			this.scroll.scrollTop(this.scroll.getScrollTop()+(rect.bottom-nav.offsetHeight))
		}
	}
	render(){
		let hList=this.props.hList
		hList=hList.map(el=>{
			let Name=el.nodeName.toLocaleLowerCase()
			return <Name key={el.id} hid={el.id}
			onClick={()=>this.props.hashChange(el.id)}
			className={el.id==this.props.hash?"hash":undefined}>{el.innerText}</Name>
		})
		return <div id="nav">
			<Scrollbars ref={(scroll)=>this.scroll=scroll}
			autoHide autoHideTimeout={500}>{hList}</Scrollbars>
		</div>
	}
}