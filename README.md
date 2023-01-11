# Deepin Manual

## For packagers

### Packaging environment

Nodejs >= 0.10 is needed; iojs should work too.

Note that because of the node/nodejs name conflict on debian-based distros,
it's recommended to use run `make dist` instead of to run `npm install` to install
dependencies.

### Dependency handling

Since we need dependencies from npm and pulling those during packaging would be a problem when
the internet situation is not ideal. Also, including those dependencies in the git tree would
cost too much space.

This is how we do it: the genereated javascripts and locale files
are included in the git source tree. That means: in order to make a distro package, it's not
necessary to go through "installing npm packages", "running gulp tasks" and such.

If you want to re-generate those, run `make deepclean`; and run `gulp dist`.

## For developers

### Development environment

In order to run full tests, iojs is required; even nodejs 0.12 with `--harmony`
won't work!

### Dependencies

It is worth noting that we included a `npm-shrinkwrap.json` file. If you ever want to make a dependency
adjustment, be sure to re-genereate that file too.

### Gulp tasks

Several gulp tasks are provided. You may want to use the following:

* `gulp watch`. It watches file changes of the source code.
* `gulp dist`. It produces files ready for deployment.
* `gulp translations`. It produces UI language json files in the `www/nls` folder.

For more tasks, please consult the source code of `Gulpfile.js`.

### Debug mode

When DManual starts with environment variable `DEBUG` being a non-empty value, the program will run in debug mode.
It doesn't do much, except it will color most of the HTML blocks and give you some hints about layout problems.

## For Application Writers

### Manuals installation requirements

Be sure to install the manual for your application to `/usr/share/deepin-manual/manual/<Application-Name>/<Language-Code>/`.

### Invocation

To invoke the DMan for a specific application, run:

```
dman <Application-Name>
```

Then DMan will open with manual in the most applicable language available.
To override this, set `LANGUAGE` environment variable.

** Note **, you are responsible for making the invoker not hang. So for example in python, do:

```
import subprocess
subprocess.Popen("dman myApp", stdin = None, stdout = None, stderr = None)
```

## For document writers

Run the following in the terminal: `dman dman`

## License 

Deepin Manual is licensed under [GPLv3](LICENSE) or any later version.