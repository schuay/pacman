# Maintainer: schuay <jakob.gruber@gmail.com>
 
pkgname=pacman_sfml
pkgver=20110331
pkgdesc="Pacman clone using SDL. Guide Pacman through a maze and collect pac-dots while avoiding ghosts."
pkgrel=1
arch=('i686' 'x86_64')
url="http://github.com/schuay/pacman/"
license=('GPL')
depends=('sfml')
makedepends=('boost' 'git')

from=~/src/$pkgname

_gitroot="git://github.com/schuay/pacman.git"
_gitname=$pkgname
_gitbranch='working'

build() {
  cd "$srcdir"
  msg "Connecting to GIT server...."

  if [ -d $_gitname ] ; then
    cd $_gitname && git pull origin $_gitbranch
    msg "The local files are updated."
  else
    git clone $_gitroot $_gitname
    cd $_gitname && git checkout -t origin/$_gitbranch
  fi

  msg "GIT checkout done or server timeout"
  msg "Starting make..."

  rm -rf "$srcdir/$_gitname-build"
  git clone "$srcdir/$_gitname" "$srcdir/$_gitname-build"
  cd "$srcdir/$_gitname-build/src"

  autoconf
  ./configure
  make
}
package() {
  cd $"$srcdir/$_gitname-build/src"

  make DESTDIR=${pkgdir} install
}
