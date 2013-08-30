
all:
	cl /MD /LD /Foautoload/ autoload/aeroglass.c USER32.lib
	@echo Move aeroglass.dll to autoload/aeroglass{32,64}.dll

clean:
	DEL aeroglass.*
