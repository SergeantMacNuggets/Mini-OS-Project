
CFlag = -Wno-implicit-function-declaration

terminal: terminal_start

terminal_start: terminal.o
	@clear
	@gcc $(CFlag) terminal.o -o terminal_start && ./terminal_start
	

terminal.o: terminal.c
	@gcc $(CFlag) -c terminal.c

clean:
	@echo "Removing all executable"
	@rm terminal.o terminal.exe terminal_start 