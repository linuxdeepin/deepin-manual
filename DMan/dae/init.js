(function(){
    //initDAEModule('fs');
    //initDAEModule('sys');
    initDAEModule('app');
    //initDAEModule('codec');

    //initClass('Window');
    //initClass('Dialog');
    //initClass('FileDialog');
    //initClass('File');
    //initClass('Menu');
    //initClass('MenuItem');
    //initClass('TrayIcon');
    //initClass('Process');
    //initClass('SocketServer');
    //initClass('SocketClient');

    for(var i in DAE.pluginList){
        console.log("Plugin: " + DAE.pluginList[i]);
        initModule(DAE.pluginList[i]);
    }

    function initModule(name){
        window[name] = initEventBinder(window[name]);
    }

    function initDAEModule(name){
        var key = 'DAE_' + name;
        DAE[name] = initEventBinder(window[key]);
        delete window[key];
    }

    function initClass(name){
        DAE[name] = function(){
            return initEventBinder(DAE['create' + name].apply(window, arguments));
        };
    }

    function initEventBinder(object){
        object.addEvent = function(event_name, handler){
            if(typeof event_name == 'string'){
                event_name = 'on' + event_name;
                if(event_name in this && this[event_name].connect){
                    this[event_name].connect(handler);
                    //console.log("**** Binding " + event_name);
                }else{
                    console.warn('Warning: This object dose not have "' + event_name + '" event');
                }
            }else if(typeof event_name == 'object'){
                for(var key in event_name){
                    this.addEvent(key, event_name[key]);
                }
            }
        };
        object.removeEvent = function(event_name, handler){
            event_name = 'on' + event_name;
            if(event_name in this && this[event_name].connect){
                this[event_name].disconnect(handler);
            }
        };
        return object;
    }
})();

