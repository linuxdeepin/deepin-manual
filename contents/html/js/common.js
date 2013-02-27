function write_to_title(type, data){
    var jsObj = {"type":type, "data":data};
    document.title = JSON.stringify(jsObj, "replacer");
}

function home_item_link(link){
    var type = "home_item_link";
    var data = link.id;
    write_to_title(type, data);
}

function external_link(link){
    var type = "external_link";
    var data = link.href;
    write_to_title(type, data);
}

function after_slider_change(imgObject) {
    type = "slider_change";
    data = imgObject.getAttribute("data-caption");
    write_to_title(type, data);
}

function create_index_img_node(id, img_path){
    var imgObj = document.createElement("img");
    imgObj.src = img_path;
    data_caption = "#"+id;
    imgObj.setAttribute("data-caption", data_caption);
    return imgObj;
}

function create_index_span_content_node(id, content){
    var spanObj = document.createElement("span");
    spanObj.className = "orbit-caption";
    spanObj.id = id;
    spanObj.innerHTML = content;
    return spanObj;
}

function insert_pages_into_subject(pages){
    outer = document.getElementById("outer");
    featured = document.getElementById("featured");
    for (var i=0;i < pages.length;i++){
        featured.appendChild(create_index_img_node(pages[i]["id"], pages[i]["image"]));
        outer.appendChild(create_index_span_content_node(pages[i]["id"], pages[i]["description"]));
    }
}

/***
 * Notice: the percent argument is a number between 0.0~1.0 
 * */
function create_home_icon_node(id, category, title, icon_path, percent){ 
   percent_div = document.createElement("div");
   percent_div.className = "percent";
   percent_div.innerHTML = (Number(percent) * 100).toString()+"%";

   barline_div = document.createElement("div");
   barline_div.className = "barline";
   bar_width = (Number(percent) * 66).toFixed(0)+"px";
   barline_div.style.width=bar_width;

   bar_div = document.createElement("div");
   bar_div.className = "bar";
   bar_div.appendChild(percent_div);
   bar_div.appendChild(barline_div);

   title_span = document.createElement("span");
   title_span.className = "title";
   title_span.innerHTML = title;
   
   icon_img = document.createElement("img");
   icon_img.src = icon_path;

   icon_a = document.createElement("a");
   icon_a.href = "index.html";
   icon_a.id = category;
   icon_a.onclick = function(e){
       home_item_link(this);
       return false;
   }
   icon_a.appendChild(icon_img);

   home_item = document.createElement("li");
   home_item.className = "m0"+id;
   home_item.appendChild(title_span);
   home_item.appendChild(icon_a);
   home_item.appendChild(bar_div);
   return home_item;
}
//var data = [{"category":"indroduction", "title":"简介", "icon_path":"../zh_CN/introduction/icon.png", "percent":"0.5"}];
function home_load(data){
    icon_menu = document.getElementById("icon_menu");
    for (var i=0; i<data.length; i++){
        icon_menu.appendChild(create_home_icon_node(i, data[i]["category"], data[i]["title"], data[i]["icon_path"], data[i]["percent"]));
    }
}

function index_load(data, subject_index){
    subject_index = Number(subject_index);
    pages = data["content"][subject_index]["page"];
    insert_pages_into_subject(pages);
}
