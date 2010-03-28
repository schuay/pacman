# Maintainer: Jakob Gruber (jakob dot gruber at kabelnet dot at)
 
pkgname=pacman_sdl
pkgver=0.1
pkgdesc="Pacman clone using SDL. Guide Pacman through a maze and collect pac-dots while avoiding ghosts."
pkgrel=2
arch=('i686' 'x86_64')
url="http://github.com/schuay/pacman/"
license=('GPL')
depends=('sdl' 'sdl_ttf' 'sdl_gfx' 'sdl_mixer')
makedepends=('git')

build() {

  #clean source directory
  rm -rf $srcdir/*

  #git checkout
  cd $srcdir
  git clone git://github.com/schuay/pacman.git
  cd $srcdir/pacman/src

  #build
  make || return 1

  #install
  install -D -m666 pacman_sdl.desktop $pkgdir/usr/share/applications/pacman_sdl.desktop
  install -D -m666 ../icon.png "$pkgdir/opt/pacman_v4/icon.png"
  install -D -m777 pacman_v4 "$pkgdir/opt/pacman_v4/pacman_v4"
  install -D -m644 ../arial.ttf "$pkgdir/opt/pacman_v4/"
  install -D -m666 ../log.txt "$pkgdir/opt/pacman_v4/"
  install -D -m666 ../pacman.cfg "$pkgdir/opt/pacman_v4/"
  install -D -m666 ../pacman.conf "$pkgdir/opt/pacman_v4/"
  install -D -m666 ../settings.conf "$pkgdir/opt/pacman_v4/"
  cp -rp ../levels "$pkgdir/opt/pacman_v4/"
  cp -rp ../skins "$pkgdir/opt/pacman_v4/"
  cp -rp ../sound "$pkgdir/opt/pacman_v4/"
  touch $pkgdir/opt/pacman_v4/log.txt
  chmod 666 $pkgdir/opt/pacman_v4/log.txt

  #fix hscore list permissions
  for FILE in $(find $pkgdir/opt/pacman_v4/ -name hscore) ; do chmod 666 $FILE; done

  #generate start script
  echo '#!/bin/bash' > pacman_start_script
  echo 'cd /opt/pacman_v4/' >> pacman_start_script
  echo './pacman_v4' >> pacman_start_script
  install -D -m777 pacman_start_script "$pkgdir/usr/bin/pacman_sdl"
}
