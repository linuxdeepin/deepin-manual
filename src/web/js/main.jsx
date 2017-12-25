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
        //设置标题删除h1
        let title=vdiv.querySelector("h1").innerText
        vdiv.removeChild(vdiv.querySelector("h1"))
        //去除标题图标
        ;[...vdiv.querySelectorAll("h2")].map(el=>el.innerText=el.innerText.split("|")[0])
        //设置唯一ID
        let hList=[...vdiv.querySelectorAll("h2,h3,h4")]
        let ids={}
        hList.map(el=>{
            let text=el.innerText
            if(ids[text]==undefined) {
                el.id = text
                ids[text]=0
            }else {
                el.id = `${text}-${ids[text]++}`
            }
        })

        return <div>
            <title>{title}</title>
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

