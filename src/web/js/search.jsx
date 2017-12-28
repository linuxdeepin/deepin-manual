const React = require("react")

function SearchResult(props) {
	let text=props.value.replace(new RegExp(props.kw,'g'),'<span class="searchWK">$&</span>')
	return <div onClick={props.onClick}>
		<h2><a>{props.title}</a></h2>
		<p dangerouslySetInnerHTML={{__html:text}}></p>
	</div>
}
module.exports=class Search extends React.Component {
	render(){
		const vdiv=this.props.vdiv;
		let result=[]
		let title=null;
		[...vdiv.childNodes].filter(el=>el.innerText).map(el=>{
			if(el.nodeName.indexOf("H")==0&&el.id==el.innerText){
				title=el
				return
			}
			if(el.innerText.indexOf(this.props.kw)!=-1){
				result.push({title,text:el.innerText.match(new RegExp(`.{0,20}?${this.props.kw}.{0,40}`))[0]})
			}
		})
		result=result.map((r,i)=>{
			return <SearchResult key={i} 
						tid={r.title.id}
						title={r.title.innerText}
						kw={this.props.kw}
						value={r.text}
						onClick={()=>this.props.SearchDone(r.title.id)}
						></SearchResult>
		})
		return <div id="search" className="read">
			<h1>{this.props.kw} -- Search 
			<span><a onClick={()=>this.props.SearchDone("")}>↩️</a></span></h1>
			{result}
		</div>
	}
}