const cheerio = require('cheerio');
const toHtml = require('./toHtml');

if (require.main === module) {
  const argv = require('yargs').argv;
  if (argv.f) {
    const $ = cheerio.load(toHtml('', argv.f));
    let hlist = $(':header');
    let searchIndex = hlist
      .map(i => {
        let el = $(hlist[i]);
        return {
          id: el.attr('id'),
          title: el.text(),
          content: el.nextUntil(hlist[i + 1]).text()
        };
      })
      .get();
    console.log(JSON.stringify(searchIndex));
  } else {
    console.error('node toSearchIndex.js -f mdFile');
  }
}
