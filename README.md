# Rebuild the webpages

The webpages are build using the standard build process. Before
building the documentation, you should create the folder `apidoc` in
the `build` directory. Then you clone the current website in this folder:
 
    git clone -b gh-pages git@github.com:TUM-LIS/glip.git html

Afterwards you go back to the `build` directory and run `make
doc`. When doxygen is done, you can commit website changes from the
`html` directory.

You don't need to repeat all steps. It is only required when you
create a new build directory.