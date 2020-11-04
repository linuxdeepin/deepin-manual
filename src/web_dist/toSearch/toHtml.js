const fs = require('fs'),
  marked = require('marked');

function toHtml(dir, file) {
  let data = fs.readFileSync(file).toString();

  let count = 0;
  let renderer = new marked.Renderer();
  renderer.heading = (text, level) => {
    if (level <= 2) {
      text = text.split('|')[0];
    }
    let id = 'h' + count;
    count++;
    let type = 'h' + level;
    return `<${type} id="${id}">${text}</${type}>\n`;
  };
  return marked(data, { renderer }).replace(/src="/g, `$&${dir}`);
}

if (require.main === module) {
  const argv = require('yargs').argv;
  if (argv.d && argv.f) {
    console.log(toHtml(argv.d, argv.f));
  } else {
    console.error('gulp toHtml -d DIR -f mdFile');
  }
}

module.exports = toHtml;
