#include<regex.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/mman.h>
#include<unistd.h>

int main(){
int fd=open("url.html",O_RDWR);
int size=lseek(fd,0,SEEK_END);
char* mmap_ptr=NULL;
mmap_ptr=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
close(fd);
//printf("%s",mmap_ptr);

char* str="<a[^>]*\\?href=\"\\([^\"]\\+\\?\\)\"[^>]*\\?>\\([^<]\\+\\?\\)</a>";
regex_t reg;
regcomp(&reg,str,0);

int num=3;
regmatch_t match[num];
char link[1024];
char theme[1024];
char* ori=mmap_ptr;

while(regexec(&reg,mmap_ptr,num,match,0)==0){
bzero(link,sizeof(link));
bzero(theme,sizeof(theme));

snprintf(link,match[1].rm_eo-match[1].rm_so+1,"%s",mmap_ptr+match[1].rm_so);
snprintf(theme,match[2].rm_eo-match[2].rm_so+1,"%s",mmap_ptr+match[2].rm_so);

printf("%s\t%s\n",link,theme);

mmap_ptr+=match[0].rm_eo;
}

regfree(&reg);
munmap(ori,size);
return 0;
}
