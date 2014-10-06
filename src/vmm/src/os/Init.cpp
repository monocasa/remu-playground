extern void appMain();

namespace os {

static inline void outb(unsigned short port, unsigned char val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

void kputc(char c)
{
	outb(1, c);
}

void kputs(const char *str)
{
	while (*str)
	{
    	kputc(*str);
		str++;
	}
}

void init()
{
	kputs("Hello VMM\n");
	appMain();
}

} /*namespace os*/

