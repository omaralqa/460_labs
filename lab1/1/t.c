
/************** t.c file **************************/
int prints(char *s)
{
   while (*s)
   {
      putc(*s++);
   }
}

int gets(char s[])
{
   char a;
   int i = 0, j;
   a = getc();
   
   if (a=='\r'){
      s[0] = 0;
      return s;
   }
   
   while (a != '\r'){
      s[i]=a;
      a=getc();
      i++;
   }
   s[i]='\n';
   return s;
}

main()
{
   char name[64];
   while(1){
     prints("What's your name? ");
     gets(name);
     if (name[0]==0)
         break;
     prints("Welcome "); prints(name); prints("\n\r");
   }
   prints("return to assembly and hang\n\r");
} 
