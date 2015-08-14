"use strict";

angular.module("General")
    .factory('localeService', function(gettextCatalog){
    let _locale = null;
    return {
        setLocale: function(locales){
            // TODO: locale filter
            _locale = locales[0];
            gettextCatalog.setCurrentLanguage(_locale);
            gettextCatalog.loadRemote("./nls/"+_locale+".json");
        },
        getLocale: () => {
            return _locale;
        }
    }
});
