# Maintainer: Jakob Gruber (jakob dot gruber at kabelnet dot at)
 
pkgname=pacman_sdl
pkgver=0.1
pkgdesc="Pacman clone using SDL. Guide Pacman through a maze and collect pac-dots while avoiding ghosts."
pkgrel=3
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
  git clone git://github.com/schuay/pacman.git || return 1
  cd $srcdir/pacman/src


  #build
  make || return 1


  #install

  dst="$pkgdir/opt/pacman_v4/"

  mkdir -p $pkgdir/{usr/share/applications/,opt/pacman_v4/}

  #.desktop file
  install -m666 pacman_sdl.desktop \
	$pkgdir/usr/share/applications/pacman_sdl.desktop

  #binary
  install -m777 pacman_v4 \
	${dst}

  install -m666 		\
	../icon.png 		\
	../arial.ttf 		\
	../pacman.cfg 		\
	../pacman.conf 		\
	../settings.conf 	\
	${dst}

  cp -rp 		\
	../levels 	\
	../skins 	\
	../sound 	\
	${dst}

  touch ${dst}log.txt
  chmod 666 ${dst}log.txt

  #fix hscore list permissions
  for FILE in $(find ${dst} -name hscore) ; do 
	chmod 666 $FILE
  done

  #generate start script
  echo '#!/bin/bash' > pacman_start_script
  echo "cd /opt/pacman_v4/" >> pacman_start_script
  echo './pacman_v4' >> pacman_start_script

  install -D -m777 pacman_start_script "$pkgdir/usr/bin/pacman_sdl"
}
