function link_to_title(link){
    var jsObj = {"type":"link", "data":"http://www.baidu.com"};
    jsObj.data = link.href;
    document.title = JSON.stringify(jsObj, "replacer");
    return false;
}
