# Maintainer: Jakob Gruber (jakob dot gruber at kabelnet dot at)
 
pkgname=pacman_sdl
pkgver=0.1.0
pkgdesc="Pacman clone using SDL. Guide Pacman through a maze and collect pac-dots while avoiding ghosts."
pkgrel=1
arch=('i686' 'x86_64')
url="http://github.com/schuay/pacman/"
license=('GPL')
source=("http://github.com/schuay/pacman/tarball/pacman-$pkgver")
md5sums=('dfbc98d324bebd62670a6681271e63c6')
depends=('sdl_ttf' 'sdl_gfx' 'sdl_mixer' 'sdl_image')
makedepends=('boost')

build() {

  # github names downloads after tag commit
  cd $srcdir/schuay-pacman-860f093/src/

  #build
  autoconf
  ./configure || return 1
  make || return 1
  make DESTDIR=$pkgdir install || return 1

}
