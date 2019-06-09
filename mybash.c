#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include<pwd.h>
#include<sys/utsname.h>

#define STRLEN 128

char OLDPWD[STRLEN] = { 0 };//上一级目录

void OutPutFlag()//输出提示符信息
{
	char flag = '$';
	struct passwd *pw = getpwuid(getuid());//通过函数getuid来获得当前用户的UID
    assert(pw != NULL);
	if (pw->pw_uid == 0)
	{
           flag = '#';
	}
	struct utsname hostname;
	uname(&hostname);
	char node[STRLEN] = { 0 };
	strcpy(node, hostname.nodename);
	char *name = strtok(node, ".");
	char path[STRLEN] = { 0 };
	getcwd(path, STRLEN - 1);//获取当前工作目录的绝对路径

	char *p = path + strlen(path);
	while (*p != '/')
	{
		p--;
	}
	if (strlen(path) != 1)// ==1 是/
	{
		p++;//在 ++之前 P指向 /
	}
	if (strcmp(path, pw->pw_dir) == 0)
	{
		p = "~";// 家目录
	}
	printf("mybash[%s@%s %s]%c ", pw->pw_name, name, p, flag);
	fflush(stdout);//刷出缓冲区内容  用户  主机名 当前工作目录 符号 #$
}

void Mycd(char* path)
{
	if (path == NULL)
	{
		return;
	}

	if (strcmp(path,"~") == 0)
	{
		struct passwd *pw = getpwuid(getuid());
		path = pw->pw_dir;
	}
	if (strcmp(path,"-") == 0)
	{
		if (strlen(OLDPWD) == 0)
		{
			printf("mybash: cd: OLDPWD not set\n");
			return;
		}
		path = OLDPWD;
	}
	char oldpwd[STRLEN] = { 0 };
	getcwd(oldpwd, STRLEN - 1);
	if(- 1 == chdir(path))// 不等于 -1就执行 chdir()
	{
		char err[128] = "mybash: cd: ";
                strcat(err, path);
		perror(err);//错误信息输出
		return;
	}
	strcpy(OLDPWD, oldpwd);
}

void Incisionstring(char* cmd, char* argv[]) //argv里面存放的字符指针argv[[0]   argv[1]  
{
	int count = 0;
	char *p = strtok(cmd, " ");
	while (p != NULL)
	{
		argv[count++] = p;
		p = strtok(NULL, " ");
	}
}

void Zombie(int sign)//异步
{
	wait(NULL);//等待子进程退出。NULL的意思是退出状态不关注。
}

int main()
{
	signal(SIGCHLD, Zombie);//注册函数
	while (1)
	{
		OutPutFlag();
		char cmd[STRLEN] = { 0 };//  cmd输入的的东西
		fgets(cmd, STRLEN, stdin);
		cmd[strlen(cmd) - 1] = 0;//将 \n 改为0
		int back = 0;//没有取地址符号

		if (strstr(cmd, "&") != NULL)//strstr(str1,str2) 函数用于判断字符串str2是否是str1的子串。如果是，则该函数返回str2在str1中首次出现的地址；否则，返回NULL。)
		{
			back = 1;
		}

		char *argv[STRLEN] = { 0 };//把拆的放入字符指针数组中
		Incisionstring(cmd, argv);//切割字符串   argv[0]为 比如 cd  ls  等
//  将输入的东西拆分  通过argv[]数组带出来 
		if (strncmp(argv[0], "cd", 2) == 0)
		{
			Mycd(argv[1]);// - .. ~  为 argv[1]
			continue; 
		}

		if (strncmp(argv[0], "exit", 4) == 0)
		{
			exit(0);
		}

		pid_t n = fork();
		assert(n != -1);

		if (0 == n)
		{
			char path[STRLEN] = "/home/free/Desktop/shell/mybin/";//用绝对路径
			if (strstr(argv[0], "/") != NULL)//  bin/ps
			{
				strcpy(path, argv[0]);// 有路径 ，不需要在默认路径下寻找
			}
			else// 若没有路径  路径后面带命令 
			{
				strcat(path, argv[0]);// 字符串链接   这个路径下面 寻找命令 路径
			}
			execv(path, argv);//此时的argv仍是一个字符指针数组 里面存放的是字符串 
			printf("%s:Command Not Found\n", argv[0]);
			exit(0);//防止子进程继续运行
		}
		else
		{
			if (!back)//  back=0 说明是正常  说明不是后台 有&时不进去
			{
				pause();//父进程
			}
		}
	 }
}

