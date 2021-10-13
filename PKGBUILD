pkgname="dwm"
pkgver="6.2"
pkgrel="1"
pkgdesc="Display manager from suckless.org"
arch=("x86_64")
license=("MIT")


depends=(
    "fontconfig" 
    "freetype2" 
    "xorg-server"
)


source=(
     src/Makefile 
     src/config.h 
     src/config.mk 
     src/drw.c 
     src/drw.h 
     src/dwm 
     src/dwm.1 
     src/dwm.c 
     src/transient.c 
     src/util.c 
     src/util.h 
)


sha512sums=(
    "SKIP"
    "SKIP"
    "SKIP"
    "SKIP"
    "SKIP"
    "SKIP"
    "SKIP"
    "SKIP"
    "SKIP"
    "SKIP"
    "SKIP"
)


package=(
    make -C "${srcdir}"

	mkdir -p "${pkgdir}/usr/bin"
	cp -f "${srcdir}/dwm" "${pkgdir}/bin/dwm"
    chmod +x "${pkgdir}/bin/dwm"
)
