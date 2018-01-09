import md5 from 'md5'

export default function (file, html) {
	// let hashName = file + "_searchIndex_hash"
	// let hash = md5(html)
	// console.log(hash)
	// if (localStorage[hashName] == hash) {
	// 	return
	// }
	// localStorage[hashName] = hash
	let div = document.createElement("div")
	div.innerHTML = html
	let searchIndex = {}
	let key = ""
	for (let i = 0; i < div.children.length; i++) {
		let el = div.children.item(i)
		if (el.nodeName.match(/^H\d$/) != null) {
			key = el.id
			searchIndex[key] = ""
		}
		searchIndex[key] += el.innerText
		searchIndex[key] += "\n"
	}
	let keys = Object.keys(searchIndex)
	let values = keys.map(key => searchIndex[key])
	console.log(file, keys, values)
	global.qtObjects.search.addSearchEntry(file, keys, values)
}