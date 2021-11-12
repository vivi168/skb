main.exe: main.cpe
	cpe2x main.cpe

main.cpe: main.obj psxio.obj level.obj psxipt.obj psxrdr.obj lvllut.obj
	ccpsx -Xm -Xo$80010000 -O2 main.obj psxio.obj level.obj psxipt.obj psxrdr.obj lvllut.obj -o main.cpe

main.obj: main.c
	ccpsx -c main.c -o main.obj
level.obj: level.c
	ccpsx -c level.c -o level.obj
psxio.obj: psxio.c
	ccpsx -c psxio.c -o psxio.obj
psxipt.obj: psxipt.c
	ccpsx -c psxipt.c -o psxipt.obj
psxrdr.obj: psxrdr.c
	ccpsx -c psxrdr.c -o psxrdr.obj
lvllut.obj: lvllut.c
	ccpsx -c lvllut.c -o lvllut.obj
