#!/bin/bash
mkdir -p /usr/share/Kfb
install  -m 755 TARBALL/Kfb.png /usr/share/Kfb/
install  -m 755 TARBALL/Kfb.desktop  /usr/share/applications/
install -m 755 -s kgfilebrowser /usr/bin/Kfb
