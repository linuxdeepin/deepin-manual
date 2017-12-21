const React = require("react"),
    ReactDOM = require("react-dom"),
    Marked = require("marked"),
    FileApi = require("./file"),
    Nav = require("./nav.jsx"),
    Content = require("./content.jsx")
class App extends React.Component{
    constructor(props){
        super(props)

        let vdiv=this.props.vdiv
        ;[...vdiv.querySelectorAll("h1,h2")].map(el=>el.innerText=el.innerText.split("|")[0])

        //设置标题删除h1
        let title=vdiv.querySelector("h1").innerText
        vdiv.removeChild(vdiv.querySelector("h1"))

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
        this.state={
            vdiv,hList,title,
            hash:hList[0].id,
            search:this.props.queryArge.search,
        }
    }
    hashChange(hash){
        this.setState({hash,search:""})
    }
    render(){
        return <div>
            <title>{this.state.title}</title>
            <Nav hList={this.state.hList}
                 hash={this.state.hash}
                 onSearchCancel={()=>this.setState({search:""})}
            ></Nav>
            <Content vdiv={this.state.vdiv}
                     search={this.state.search}
                     hash={this.state.hash}
                     onHashChange={this.hashChange.bind(this)}
                     onSearchDone={()=>this.setState({search:""})}
            ></Content>
        </div>
    }
}

let queryArge={"app":"dde",search:""}
location.search.substr(1).split("&").map(s=>{
    let arge=s.split("=")
    queryArge[arge[0]]=arge[1]
})
console.log(queryArge)

let appName=queryArge.app
let language=navigator.language.replace(/-/,"_")

FileApi.read(`${appName}/${language}/index.md`).then(data=>{
    let html=Marked(data)
    //图片资源代理
    html=html.replace(/src="/g,`$&file:///usr/share/dman/${appName}/${language}/`)

    //去除标题图标
    let vdiv=document.createElement("div")
    vdiv.innerHTML=html

    ReactDOM.render(<App vdiv={vdiv} queryArge={queryArge}></App>,document.getElementById("app"))
})