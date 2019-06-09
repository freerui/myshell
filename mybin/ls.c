#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>

/*选项*/
#define IS_A 1
#define IS_I 9
#define IS_L 12
#define IS_OPTION(option,flag)((option)& (1<<(flag-1)))//宏很不安全

char CurrentPath[128] = {0};
int Option = 0;//选项

//寻找路径, 井且解析选项部分
void AnayPara(char *argv[], int argc, char *path[])
{
	int i = 1;
	int count = 0;
	for (; i<argc; i++)
	{
		if (strncmp(argv[i], "-", 1) != 0)
		{
			path[count++] = argv[i];
			continue;
		}

		/*if(strstr(argv[i],"a")!=NULL)//有a
		{
		   Option |= 1<<0;// 4*8=32 26位代表 a b c..
		}*/

		char*p = argv[i] + 1;//p-->第一个有效选项
		
		while (*p != 0)
		{
			//perfect
			Option |= 1 << (*p - 'a');//a i l ai al il ail
			p++;
		}
	}
	if (count == 0)//用户没有給定路径
	{
		getcwd(CurrentPath, 127);//获取当前路程
		path[0] = CurrentPath;
	}
}
void PrintfFileInfo(char *path, struct dirent *d) //隐蔵&非隐藏
{
	if (!IS_OPTION(Option, IS_A) && strncmp(d->d_name, ".", 1)== 0)
	{	
		return;  // 如果 有a 或者 文件名中  .  则不显示
	}	
	struct stat st;
	char pathname[128] = { 0 };
	strcpy(pathname, path);
	strcat(pathname, "/");
	strcat(pathname, d->d_name);
	int res = stat(pathname, &st);
	assert(res != -1);

	if (IS_OPTION(Option, IS_I))//有i
	{
		printf("%d ", d->d_ino);
	}

	if (IS_OPTION(Option, IS_L))//有1 信息多
	{
		//UNDO
	}
	if (S_ISDIR(st.st_mode))//目录文件 S_ISDIR
	{
		printf("\033[1;34m%s\033[0m ", d->d_name);//蓝色
		return;
	}
	//有一个有执行权限, 绿色
	if (st.st_mode & S_IXUSR//用户执行
		|| st.st_mode & S_IXGRP//组执行
		|| st.st_mode & S_IXOTH) //其他执行
	{
		printf("\033[1;32m%s\033[0m ", d->d_name);//绿色
		return;
	}
	printf("%s ",d->d_name);//显示文件 
}

int main(int argc, char *argv[])
{
	char *path[128] = { 0 };
	AnayPara(argv, argc, path);

	int i = 0;
	while (path[i] != NULL)
	{
		DIR *dirp = opendir(path[i]);//打开第一个路径
		if (dirp == NULL)
		{
			printf("%s Not found\n", path[i]);
			break;
		}
		struct dirent *d = NULL;
		while ((d = readdir(dirp)) != NULL)//d获取
		{
			PrintfFileInfo(path[i], d);
		}
		printf("\n");
		closedir(dirp);//关闭
		i++;
	}
	return 0;
}
