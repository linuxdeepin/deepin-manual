"use strict";

angular.module("General")
    .factory('localeService', function(gettextCatalog){
    return {
        setLocale: function(locales){
            // TODO: locale filter
            let locale = locales[0];
            gettextCatalog.setCurrentLanguage(locale);
            gettextCatalog.loadRemote("./nls/"+locale+".json");
        }
    }
})