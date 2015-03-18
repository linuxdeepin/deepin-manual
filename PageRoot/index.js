(function(){
    if (typeof DAE === "undefined") {
        return false;
    }
    var app = DAE.app;
    
    var main = function(){
        initApp();
        initEvent();
        app.showCenter();
    };

    var initApp = function(){
        app.setMinSize(680, 480);
    };

    var initEvent = function(){
        var stateChangeEvent = function(){
        };
        var closeEvent = function(){
        };
        app.addEvent({
			show: function(){
			},
			hide: function(){
			},
            statechange: stateChangeEvent,
            close: closeEvent
        });

        /***
        $(window).bind({
            beforeunload: function(){
                //app.removeEvent('close', closeEvent);
                app.removeEvent('statechange', stateChangeEvent);
            }
        });
        $('a').bind({
            click: function (event) {
                if(this.href.slice(-1) == "#"){
                    return false;
                }
            }
        });
        $('#globalNavBar').bind({
            mousedown: function(event){
                if(event.which == 1 && ! $(event.target).is('button')){
                    app.dragStart();
                }
            },
            mouseup: function(event){
                if(event.which == 1){
                    app.dragStop();
                }
            },
            dblclick: function(){
                if(app.isMaximized()){
                    app.normalize();
                }else{
                    app.maximize();
                }
            }
        });
        $('#tranButton').bind({
            click: function(e){
                //alert("test");
                //e.preventDefault();
                return false;
            }
        });
        $('#nav').delegate('.item', {
            click: function(e){
                var index = $(this).data('index');
                if(index !== undefined && ! $(this).hasClass('item-cur')){
                    var $article = $('#main .article').hide().addClass('invisible').eq(index).show();
                    $('#nav .item').removeClass('item-cur');
                    setTimeout(function(){
                        $article.removeClass('invisible');
                    });
                    $(this).addClass('item-cur');
                    $(this).parents('.item-parent').addClass('item-cur');
                    e.stopPropagation();
                }
            }
        });
        $('#main').bind({
            scroll: function(){
                if($(this).scrollTop() > 0){
                    $(this).addClass('inset-shadow');
                }else{
                    $(this).removeClass('inset-shadow');
                }
            }
        });
        $('.demos a').bind({
            click: function(event){
                var href = $(this).data('href');
                if(href){
                    new DAE.Window(href);
                    event.preventDefault();
                }
            }
        });
        ***/
    };

    main();
})();
