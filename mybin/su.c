#include<pwd.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>
#include<shadow.h>
#include<termios.h>
#include<sys/types.h>

int main(int argc, char *argv[])
{
	char *user = "root";//默认root用户
	if (argc > 1)// 0是命令,>1说明有参数
	{
		user = argv[1];
	}
	printf("Password: ");
	fflush(stdout);

	struct termios oldtm, newtm;
	tcgetattr(0, &oldtm);//第一个，表示文件描述符
	newtm = oldtm;
	newtm.c_lflag &= ~ECHO;//去掉回显功能
	newtm.c_lflag &= ~ICANON;//非标准模式
	tcsetattr(0, TCSANOW, &newtm);
	char passwd[128] = { 0 };
	int j = 0;
	char c = 0;
	while ((c = getchar()) != '\n')
	{
		if (c == 127)//删除键
		{
			if (j <= 0)
			{
				continue;
			}
			passwd[--j]=0;
			printf("\033[1D");//把光标向左移动一列
			printf("\033[K");
			continue;
		}
		passwd[j++] = c;//保存一个, 打印一个星号
		printf("*");
		fflush(stdout);
	}
	// printf("n %s\n",passwd);
	tcsetattr(0, TCSANOW, &oldtm);//恢复原来的,要不后面都不回显了 第二个 立即修改
	printf("\n");
	//fgets(passwd,128,stdin);//标准输入
	// passwd[strlen(passwd)-1]=0;
	struct spwd *sp = getspnam(user);
	assert(sp != NULL);
	//sp->sp_pwdp id  密钥+密文
	                                 
	char salt[128] = { 0 };
	char *p = sp->sp_pwdp;
	int count = 0;
	int i = 0;
	while (*p != 0)
	{
		salt[i++] = *p;
		if (*p == '$')
		{
			count++;
		}
		if (count == 3)
		{
			break;
		}
		p++;
	}
	char *pwdp = crypt(passwd, salt);//明文 id+密明
	if (strcmp(pwdp, sp->sp_pwdp) != 0)//匹配失败
	{
		printf("password is error\n");
		exit(0);
	}

	pid_t n = fork();//匹配成功, 创建子进程
	assert(n != -1);

	if (n == 0)
	{
		struct passwd *pwd = getpwnam(user);//根据用户名获取 默认shell
		assert(pwd!=NULL);

		setuid(pwd->pw_uid);//切换用户（核心的）

		setenv("HOME", pwd->pw_dir, 1);//设置环境变量
		execl(pwd->pw_shell, pwd->pw_shell, (char *)0);//调用户所对应的终端
		printf("su exec fail\n");
		exit(0);
	}
	else
	{
		wait(NULL);
	}
	return 0;
}

