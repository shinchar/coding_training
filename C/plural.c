#include <stdio.h>
#include <string.h>

#define LINE_BUF 20

/* 最大LINE_BUF分の文字数の単語の複数形をそれとなく
 * 表示するプログラム
 * 文字列の末尾のヌル文字や２文字前なんかを
 * 意識するとポインタの復讐になって良い
 */
int main(void){

	int i,n;
	char buf[LINE_BUF + 1];
	char word[LINE_BUF + 4];

	size_t length;
	//以下３パターンの複数形表示しか考えていない
	char str1[3] = "es"; 
	char str2[4] = "ves";
	char str3[4] = "ies";

	fgets(buf, sizeof(buf), stdin);
	sscanf(buf,"%s\n", word);
	length = strlen(word);

	char *last = &word[length - 1];
	char *l_o_b = &word[length -2];
	//esをつけるパターン
	if( *last  == 's' ||
   	   (*l_o_b == 's' && *last == 'h') ||
   	   (*l_o_b == 'c' && *last == 'h') ||
        *last  == 'o' ||
        *last  == 'x' )
	{
		strcat(word,str1);
	}else
	//fがvesになるパターン
	if(*last == 'f'){
		*last = '\0';
		strcat(word,str2);
	}else
	if(*l_o_b == 'f' && *last == 'e'){
		*l_o_b = '\0';
		strcat(word,str2);
	}else
	//yがiesになったりしたりしなかったりするパターン
	if(*last == 'y' &&
		!(*l_o_b == 'a' ||
		   *l_o_b == 'i' ||
		   *l_o_b == 'u' ||
		   *l_o_b == 'e' ||
		   *l_o_b == 'o' ))
	{
		*last = '\0';
		strcat(word,str3);
	}else{
		*(last + 1) = 's';
		*(last + 2) = '\0';
	}

	printf("%s\n",word);
	return 0;
}
