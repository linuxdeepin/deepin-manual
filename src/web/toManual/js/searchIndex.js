// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
	let searchIndex = []
	let key = ""
	for (let i = 0; i < div.children.length; i++) {
		let el = div.children.item(i)
		if (el.nodeName.match(/^H\d$/) != null) {
			key = el.id
			searchIndex.push({
				id: key,
				title: el.innerText,
				content: ""
			})
			continue
		}
		if (key == "") {
			continue
		}
		searchIndex[searchIndex.length - 1].content += el.innerText
	}
	global.qtObjects.search.addSearchEntry(
		file,
		global.lang,
		searchIndex.map(s => s.title),
		searchIndex.map(s => s.id),
		searchIndex.map(s => s.content)
	)
}
