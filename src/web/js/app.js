const React = require("react"),
    ReactDOM = require("react-dom"),
    Request = require("browser-request"),
    Marked = require("marked"),
    Main = require("./main.jsx")

function Index(props) {
    return <div id="index">
            {props.appLists.map(list=>
                <div key={list.name}>
                    <h2>{list.name}</h2>
                    {list.child.map(app=>
                        <div key={app.name} className="app">
                            <img src={`${arge.origin}/${app.name}/common/${app.icon}`}/>
                            <br/>
                            {app.title}
                        </div>
                    )}
                </div>
            )}
        </div>
}

//从URL query初始化参数
function parseArge(query) {
    let queryArge={}
    query.split("&").map(s=>{
        let arge=s.split("=")
        queryArge[arge[0]]=arge[1]
    })
    return queryArge
}
//渲染App组件
function renderApp() {
    ReactDOM.render(<Main html={arge.html} app={arge.app} search={arge.search}></Main>,document.getElementById("app"))
}
//初始化参数
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
//首页
global.index=function (appLists) {
    ReactDOM.render(<Index appLists={appLists}></Index>,document.getElementById("app"))
}
global.testIndex=function () {
//首页JSON例子
global.index([
    {
        name: "系统",
        child:[
            {name: "dde", title: "深度桌面DDE", icon: "deepin-system.svg"},
            {name: "dde-file-manager", title: "深度文件管理器", icon: "deepin-file-manager.svg"}
        ]
    },
    {
        name: "应用",
        child: [
            {name: "deepin-system-monitor", title: "深度系统监视器", icon: "monitor.svg"},
            {name: "deepin-terminal", title: "深度终端", icon: "deepin-terminal.svg"},
            {name: "youdao-dict", title: "有道词典", icon: "deepin-youdaodict.svg"},
            {name: "deepin-appstore", title: "深度应用商店", icon: "deepin-appstore.svg"},
            {name: "deepin-system-monitor", title: "深度系统监视器", icon: "monitor.svg"},
            {name: "deepin-terminal", title: "深度终端", icon: "deepin-terminal.svg"},
            {name: "deepin-terminal", title: "深度终端", icon: "deepin-terminal.svg"},
            {name: "youdao-dict", title: "有道词典", icon: "deepin-youdaodict.svg"},
            {name: "deepin-appstore", title: "深度应用商店", icon: "deepin-appstore.svg"},
            {name: "deepin-system-monitor", title: "深度系统监视器", icon: "monitor.svg"},
            {name: "deepin-terminal", title: "深度终端", icon: "deepin-terminal.svg"},
            {name: "youdao-dict", title: "有道词典", icon: "deepin-youdaodict.svg"}
        ]
    }])
}
// global.testIndex()
//open(arge.app)