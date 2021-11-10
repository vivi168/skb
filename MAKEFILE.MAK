main.exe: main.cpe
	cpe2x main.cpe

main.cpe: main.obj level.obj psxipt.obj psxrdr.obj
	ccpsx -Xm -Xo$80010000 main.obj level.obj psxipt.obj psxrdr.obj -o main.cpe

main.obj: main.c
	ccpsx -c main.c -o main.obj
level.obj: level.c
	ccpsx -c level.c -o level.obj
psxipt.obj: psxipt.c
	ccpsx -c psxipt.c -o psxipt.obj
psxrdr.obj: psxrdr.c
	ccpsx -c psxrdr.c -o psxrdr.obj
