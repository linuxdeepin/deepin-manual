import marked from 'marked'
import md5 from 'md5'

export default class M2H{
	constructor(appName,md){
		this.appName=appName
		
		let hash=md5(md)
		if(localStorage[appName+"_hash"]==hash){
			console.log(appName,"cache")
			return
		}
		console.log(appName,"update")
		localStorage[appName+"_hash"]=hash

		let path=`${localStorage.path}/${appName}/${localStorage.lang}/`
		let div=document.createElement("div")
		div.innerHTML=marked(md).replace(/src="/g, `$&${path}`)

		let [title,logo]=div.querySelector("h1").innerText.split("|")
		logo=`${path}${logo}`
		div.removeChild(div.querySelector("h1"))
		localStorage[appName+"_info"]=JSON.stringify({title,logo})

		;[...div.querySelectorAll("h2")].map(el => el.innerText = el.innerText.split("|")[0])
		let ids = {}
		let hList=[...div.querySelectorAll("h2,h3")].map(el => {
			let text = el.innerText
			if (ids[text] == undefined) {
				el.id = text
				ids[text] = 0
			} else {
				el.id = `${text}-${ids[text]++}`
			}
			return {id:el.id,text,type:el.nodeName.toLocaleLowerCase()}
		})
		localStorage[appName+"_hlist"]=JSON.stringify(hList)
		localStorage[appName+"_html"]=div.innerHTML

		let searchIndex={}
		let key=""
		;[...div.children].map(el=>{
			if(el.localName.match(/^h\d$/)!=null){
				key=`${appName}#${el.id}`
				searchIndex[key]=""
			}else{
				searchIndex[key]+=el.innerText
				searchIndex[key]+="\n"
			}
		})
		// updateSearchIndex(appName,JSON.stringify(searchIndex))
	}
	info(){
		return JSON.parse(localStorage[this.appName+"_info"])
	}
	hlist(){
		return JSON.parse(localStorage[this.appName+"_hlist"])
	}
	html(){
		return localStorage[this.appName+"_html"]
	}
}