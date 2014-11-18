EXTRA_DIST = doxygen.cfg.in DoxygenLayout.xml
CLEANFILES = doxyfile.stamp

apidoc: doxyfile.stamp

doxyfile.stamp: doxygen.cfg
	doxygen $^
	echo "timestamp for doxygen" > doxyfile.stamp

clean-local:
	rm -rf apidoc
