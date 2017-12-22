const React = require("react"),
    ReactDOM = require("react-dom"),
    Marked = require("marked"),
    Request = require("browser-request"),
    Nav = require("./nav.jsx"),
    Content = require("./content.jsx")
class App extends React.Component{
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

function parseArge(query) {
    let queryArge={}
    query.split("&").map(s=>{
        let arge=s.split("=")
        queryArge[arge[0]]=arge[1]
    })
    return queryArge
}

function renderApp() {
    ReactDOM.render(<App html={arge.html} app={arge.app} search={arge.search}></App>,document.getElementById("app"))
}

let arge=parseArge(location.search.substr(1))
arge.search=arge.search?decodeURI(arge.search):""
arge.lang = navigator.language.replace(/-/,'_')
arge.origin = location.protocol=="http:"?
    `http://${location.hostname}:8000`
    :`file:///usr/share/dman`

//搜索
global.search=function(kw) {
    arge.search=kw
    renderApp()
}
//打开应用帮助
global.open=function (app) {
    arge.app=app
    Request.get(`${arge.origin}/${arge.app}/${arge.lang}/index.md`,(error, response,body)=>{
        //图片资源重定向
        arge.html=Marked(body).replace(/src="/g,`$&${arge.origin}/${arge.app}/${arge.lang}/`)
        renderApp()
    })
}
//open(arge.app)