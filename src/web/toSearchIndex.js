// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

const cheerio = require('cheerio');
const toHtml = require('./toHtml');

function delHtmlTagAndNewLine(str) {
  let result = str.toString();
  result = result.replace(/[\n]/g, ""); 
  result = result.replace(/<(?!img).*?>/g, '');

  let reg = /<(img.*?)(alt=\".*?\")>/g;
  result = result.replace(reg, '<$1>');
  return result;
}

if (require.main === module) {
  const argv = require('yargs').argv;
  if (argv.f) {
    const $ = cheerio.load(toHtml('', argv.f), {decodeEntities: false});
    let hlist = $(':header');
    let searchIndex = [];
    hlist.map(i => {
      let el = $(hlist[i]);
      searchIndex.push([
        el.attr('id'),
        el.text(),
        delHtmlTagAndNewLine(el.nextUntil(hlist[i + 1]).contents())
      ]);
    });

    console.log(JSON.stringify(searchIndex));

  } else {
    console.error('node toSearchIndex.js -f mdFile');
  }
}
