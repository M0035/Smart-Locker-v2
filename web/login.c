#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USERNAME "admin"
#define PASSWD "123"

int main(int argc, const char *argv[])
{
	//获取收到的数据
	char *data = getenv("QUERY_STRING");
	char username[50], password[50];

    //%49[^&]:表示从当前位置读取最多 49 个字符，直到遇到 &
	sscanf(data, "username=%49[^&]&password=%49s", username, password);

    //由于告诉浏览器，将发送html的文本页面
	printf("Content-Type: text/html\n\n");

    //输出完整的html页面的head标签内容
	printf("<html> <head>");
	printf("<meta charset=\"UTF-8\"> </head> <body>");

    //判断用户名、密码是否正确
	if(strncmp(username, USERNAME, 49) || strncmp(password, PASSWD, 49)) {
		printf("<h3>用户名或者密码错误<a/h3>");
		printf("<meta http-equiv=\"refresh\" content=\"1; url=/index.html\">");
	} else {
		printf("<h3>登录成功！</h3>");
		printf("<meta http-equiv=\"refresh\" content=\"1; url=/cgi-bin/home.cgi\">"); //注意这里切换的是html，非cgi
	}

    //输出结束标签
	printf("</body> </html>");
	return 0;
}
