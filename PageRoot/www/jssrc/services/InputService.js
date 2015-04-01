"use strict";

angular.module("General")
    .factory("GInput", function($log, $http, $q) {
        let _load = function(url) {
            let parsed = new URL(url);
            return $q(function(resolve, reject) {
                switch (parsed.protocol) {
                    case "http:":
                    case "https:": {
                        return $http.get(url)
                            .success(function(data) {
                                resolve(data);
                            })
                            .error(function(_, status) {
                                reject(`Failed to load with status ${status}`);
                            });
                    }
                    case "file:": {
                        if (typeof XMLHttpRequest !== "undefined") {
                            return $http.get(url)
                                .success(function(data) {
                                    resolve(data);
                                })
                                .error(function() {
                                    reject("Failed to load local file.");
                                });
                        } else {
                            return reject("Cannot handle file scheme.");
                        }
                    }
                    default: {
                        return reject(`Don't know what to do with the protocol(${parsed.protocol})`);
                    }
                }
                throw new Error("Unhandled");
            });
        };

        return {
            load: _load,
        }
});
