import md5 from "md5"
import m2h from "./mdToHtml"

export default function(file, data, html) {
	if (data != null) {
		const hash = md5(data)
		if (localStorage[file + "_data_hash"] == hash) {
			return
		}
		localStorage[file + "_data_hash"] = hash
		html = m2h(file, data).html
	} else if (html != null) {
		const hash = md5(html)
		if (localStorage[file + "_html_hash"] == hash) {
			return
		}
		localStorage[file + "_html_hash"] = hash
	} else {
		return
	}
	let div = document.createElement("div")
	div.innerHTML = html
	let searchIndex = {}
	let key = ""
	let texts = []
	for (let i = 0; i < div.children.length; i++) {
		let el = div.children.item(i)
		if (el.nodeName.match(/^H\d$/) != null) {
			key = el.id
			texts.push(el.innerText)
			searchIndex[key] = ""
			continue
		}
		searchIndex[key] += el.innerText
		searchIndex[key] += "\n"
	}

	global.qtObjects.search.addSearchEntry(
		file,
		global.lang,
		texts,
		Object.keys(searchIndex),
		Object.values(searchIndex)
	)
}
